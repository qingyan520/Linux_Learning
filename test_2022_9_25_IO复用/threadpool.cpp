#pragma once
#include"threadpool.h"

std::atomic_int Thread::ID = 0;
Result::Result(std::shared_ptr<Task> s, bool isSet) :task_(s), isSet_(isSet)
{
	task_->setRes(this);
}

void Result::SetVal(Any val) {
	//cout << "id:" << std::this_thread::get_id() << "执行Result::SetVal" << endl;
	this->any_ = std::move(val);
	sem_.post();
}

Any Result::get()
{
	if (isSet_ == false)
	{
		return "";
	}
//	cout << "[" << std::this_thread::get_id() << "]" << " :Any::Result::get()" << endl;
	sem_.wait();
	//cout << "any:" << any_.get_cast<double>() << endl;
	//cout << "[" << std::this_thread::get_id() << "]" << " :Any::Result::get() success" << endl;

	return std::move(any_);
}

void Task::exec()
{
	if (res_ != nullptr)
	{
		//cout << "id:" << std::this_thread::get_id() << "执行--exec" << endl;
		res_->SetVal(Run());

		//cout << "id:" << std::this_thread::get_id() << "执行--exec----完成" << endl;

	}
}
void Task::setRes(Result* res)
{
	res_ = res;
}

//向线程池提交任务
Result ThreadPool::submitTask(std::shared_ptr<Task>sp)
{
	std::unique_lock<std::mutex>lock(this->taskMtx_);
	if (Not_Full.wait_for(lock, std::chrono::seconds(3), [&]()->bool {
		return taskQue_.size() < taskQueMaxSize_;
		}) == false)
	{
		LOG(WARNING, "taskQue is full");
		return Result(sp, false);
	}
	
		//现在获取锁之后，向任务队列添加任务，同时判断是否为cached模式，如果是cached模式，那么还需要我们
		//判断当前空闲线程数量是不是小于任务的数量同时线程数量小于cached模式最大线程数量

		//向任务队列添加元素
		taskQue_.push(sp);
		taskQueSize_++;
		//通知其它线程进行消费
		Not_Empty.notify_all();


		//判断是不是需要添加线程
		if (mode_ == PoolMode::CACHED_MODE 
			&& freeThreadSize_ < taskQueSize_ 
			&& curThreadSize_ < threadMaxSize_)
		{
			//创建新线程，加入到unordered_map中，并且启动新线程
			auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::func, this, std::placeholders::_1));
			int id = ptr->get_id();
			threads_.emplace(id, std::move(ptr));
			threads_[id]->start();
			this->curThreadSize_++;
			this->freeThreadSize_++;
			LOG(INFO, "Add a new thread:" + std::to_string(id));
		}
		return Result(sp);
}

//线程处理任务
void ThreadPool::func(int id)
{
	
	//标记每个线程最后一次访问时间
	auto LastTime =std::chrono::high_resolution_clock::now();
	for (;;)
	{
	
		cout << std::this_thread::get_id() << ":taskQueSIze:" << taskQue_.size() << endl;
		std::shared_ptr<Task>task=nullptr;

		{
			//获取锁
			std::unique_lock<std::mutex>lock(taskMtx_);
			cout << std::this_thread::get_id() << " 获取锁" << endl;
			//阻塞等待，直到taskQue里面有元素了再进行消费
			while (IsPoolRunning() && taskQue_.size() == 0)
			{
				//如果是cached模式，就阻塞等待一段时间，判断是否超时，如果超时，那么我们就看这个线程的存在时间是否
				//超过指定时间，我们就杀死当前进程
				if (mode_ == PoolMode::CACHED_MODE)
				{
					//阻塞等待，判断等待时间是否超时了，如果超时，就杀死当前进程
					if (std::cv_status::timeout == Not_Empty.wait_for(lock, std::chrono::seconds(3)))
					{
						//cout << "jejej" << endl;
						auto now = std::chrono::high_resolution_clock::now();
						//如果超过规定时间就杀死线程
						auto mul = std::chrono::duration_cast<std::chrono::seconds>(now - LastTime);
						//cout<<std::this_thread::get_id() <<" " << curThreadSize_ << " " << threadInitSize_ << endl;
						if (mul.count() >= 3 && curThreadSize_ > threadInitSize_)
						{
							//cout <<"杀死线程" << std::this_thread::get_id() << " " << curThreadSize_ << " " << threadInitSize_ << endl;
							auto it = threads_.find(id);
							if (it != threads_.end())
							{
								threads_.erase(it);
								curThreadSize_--;
								freeThreadSize_--;
								//删除之后通知主线程看是否退出
								std::stringstream ss;
								ss << "id:";
								ss << std::this_thread::get_id();
								ss << " deleted";
								LOG(INFO, ss.str());
							}
							cond_exit.notify_all();
							return;
						}
					}
				}
				else
				{
					//cout << "heeeeheh" << endl;
					//FIXED模式下就一直等待是否有任务到来
					Not_Empty.wait(lock);
				}
			}
			if (IsPoolRunning() == false && taskQue_.size() == 0)
			{
				auto it = threads_.find(id);
				if (it != threads_.end())
				{
					threads_.erase(it);
					curThreadSize_--;
					freeThreadSize_--;
					//删除之后通知主线程看是否退出
					std::stringstream ss;
					ss << "id:";
					ss << std::this_thread::get_id();
					ss << " deleted";
					LOG(INFO, ss.str());
				}
				cond_exit.notify_all();
				return;
			}
			//此时代表任务队列有任务了，消费任务队列
			task = taskQue_.front();
			taskQue_.pop();
			taskQueSize_--;
			freeThreadSize_--;


			std::cout << std::this_thread::get_id() << "获取任务成功" << std::endl;
			//如果此时还有任务，就通知其它消费者线程进行消费
			if (taskQue_.size() > 0)
				Not_Empty.notify_all();
			//此时取出任务之后代表任务队列不满了，我们就可以通知生产者线程(submitTask)进行生产
			Not_Full.notify_all();
		}

		if (task != nullptr)
		{
			//cout << std::this_thread::get_id() << " 执行任务" << endl;
			task->exec();
		}

		freeThreadSize_++;
		LastTime= std::chrono::high_resolution_clock::now(); //更新线程最后一次访问时间
	}

}
