#include"ThreadPool.h"

//Thread类构造函数
Thread::Thread(ThreadFunc _func) :func_(_func),id(threadId++) {

}

//Thread类析构函数
Thread::~Thread() {

}

//Thread执行线程函数
void Thread::start() {
	std::thread t(func_,id);  //定义线程，传入其执行的函数
	t.detach();            //线程分离
}
std::atomic_int Thread::threadId = 0;


//返回规定的线程值
int Thread::GetId()
{
	return this->id;
}


//ThreadPool构造函数
ThreadPool::ThreadPool() :
	taskSize_(0),
	threadInitSize_(THREAD_INIT_SIZE),
	taskQueMaxHold_(TASK_MAX_THRESHOLD),
	threadsMaxSize_(THREAD_CACHED_MAX_SIZE),
	curthreadSize_(0),
	freeThreadSize_(0),
	mode(PoolMode::FIXED_MODE),
	isRunning(false) {

}

//ThreadPool析构函数
ThreadPool::~ThreadPool() {
	isRunning = false;

	std::unique_lock < std::mutex >_lock(mtxQue_);
	not_Empty.notify_all();
	cond_exit.wait(_lock,
		[&]()->bool {
			return threads_.size() == 0;
		});


}

//设置任务队列最大任务数量
void ThreadPool::SetTaskQueMaxHold(int maxhold) {
	if (isPoolRunning())
	{
		LOG(WARNING, "ThreadPool is already running,can not set Taskque max size");
		return;
	}
	this->taskQueMaxHold_ = maxhold;
}

//设置线程池模式
void ThreadPool::SetPoolMode(PoolMode _mode) {
	if (isPoolRunning())
	{
		LOG(WARNING, "ThreadPool is already running,can not set ThreadPool Mode");
		return;
	}
	this->mode = _mode;
}

//设置cached模式最大线程数量
void ThreadPool::SetThreadsMaxSize(int size)
{
	//判断线程池是否已经开始使用
	if (isPoolRunning())
	{

		LOG(WARNING, "ThreadPool is already running,can not set cached Threads Max Size");
		return;
	}
	this->threadsMaxSize_ = size;
}


bool ThreadPool::isPoolRunning()
{
	return isRunning;
}

//启动线程池
void ThreadPool::start(int size) {
	//初始化线程数量
	//判断是否为cached模式，在cached模式中线程初始化数量必须小于最大线程数量，如果初始化线程数量大于最大线程
	//我们手动改变其变成默认值
	if (this->mode == PoolMode::CACHED_MODE)
	{
		if (size > this->threadsMaxSize_)
		{
			LOG(WARNING, "ThreadPool::CACHED_MODE threadInitSize must less than threadsMaxSize_,change threadInitSize become default");
			this->threadInitSize_ = THREAD_INIT_SIZE;
		}
		else
		{
			this->threadInitSize_ = size;
		}
	}
	else
	{
		this->threadInitSize_ = size;
	}

	std::cout << "size=" << size << std::endl;

	//设置线程池状态
	this->isRunning = true;
	//初始化线程
	for (int i = 0; i < threadInitSize_; i++)
	{
		auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this,std::placeholders::_1));
		threads_.emplace(ptr->GetId(),std::move(ptr));
		this->curthreadSize_++;          //当前线程数量++
		this->freeThreadSize_++;         //剩余线程数量++
	}

	for (int i = 0; i < threadInitSize_; i++)
	{
		threads_[i]->start();
	}
}

//向任务队列中push数据
Result ThreadPool::submitTask(std::shared_ptr<Task> sp) {

	//获取锁
	std::unique_lock<std::mutex>_lock(this->mtxQue_);
	//代表此时已经满了,我们在条件变量not_Full下面等待2s,看是否不满
	if ((not_Full.wait_for(_lock, std::chrono::seconds(1), [&]()->bool {
		return taskQue_.size() < taskQueMaxHold_;
		})) == false)
	{
		LOG(WARNING, "TaskQue is full!");
		return   Result(sp, false);
	}
		taskQue_.push(sp);
		taskSize_++;
		//此时放了新任务，在notEmpty上进行通知其它消费者线程进行消费
		not_Empty.notify_all();

		//如果是cached模式，当前线程数量少于任务数量并且线程数量少于最大cached线程数量
		//我们就创建新线程
		if (this->mode == PoolMode::CACHED_MODE && taskSize_>curthreadSize_ && curthreadSize_ < threadsMaxSize_)
		{
			//当前线程数量::包括正在执行任务的线程数量 
			//空闲线程数量::不执行任务的线程数量
			auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1));
			int id = ptr->GetId();
			threads_.emplace(ptr->GetId(), std::move(ptr));
			this->curthreadSize_++;          //当前线程数量++
			this->freeThreadSize_++;         //剩余线程数量++
			threads_[id]->start(); //启动线程
			LOG(INFO, "Add a new thread:" + std::to_string(id));
		}
		return Result(sp);

}

void ThreadPool::threadFunc(int threadId) {
	auto LastTime = std::chrono::high_resolution_clock().now();
	printf("threadId=%d\n", threadId);
	while(isPoolRunning())
	{
		std::shared_ptr<Task>task;
		{
			std::unique_lock<std::mutex>_lock(this->mtxQue_);
			std::cout << std::this_thread::get_id() << "获取任务..." << std::endl;
			while (taskQue_.empty())
			{
				if (mode == PoolMode::CACHED_MODE)
				{
					//代表此时访问时间超时了
					if (std::cv_status::timeout == not_Empty.wait_for(_lock, std::chrono::seconds(2)))
					{
						auto now = std::chrono::high_resolution_clock().now();
						auto dur = std::chrono::duration_cast<std::chrono::seconds>(now-LastTime);
						//超过60s代表超时，删除该节点
						if (dur.count() > 5&&curthreadSize_>this->threadInitSize_)
						{
							threads_.erase(threadId);
							curthreadSize_--;   //当前线程数量--
							freeThreadSize_--;  //空闲线程数量--
							std::stringstream ss;
							ss << "thread:";
							ss << std::this_thread::get_id();
							ss << " deleted";
							LOG(INFO, ss.str());
							cond_exit.notify_all();
							return;
						}

					}
				}
				else
				{
					not_Empty.wait(_lock);
				}
				if (!isPoolRunning())
				{
					threads_.erase(threadId);
					curthreadSize_--;   //当前线程数量--
					freeThreadSize_--;  //空闲线程数量--
					std::stringstream ss;
					ss << "thread:";
					ss << std::this_thread::get_id();
					ss << " deleted";
					LOG(INFO, ss.str());
          cond_exit.notify_all();
					return;
				}
			}
		/*	if (!isPoolRunning())
			{
				break;
			}*/
			task = taskQue_.front();
			taskQue_.pop();
			taskSize_--;
			freeThreadSize_--;   //执行任务时空闲线程--

			std::cout << std::this_thread::get_id() << "获取任务成功" << std::endl;
			if (taskQue_.size() > 0)
			{
				not_Empty.notify_all();
			}
			//消费之后说明队列不满了
			not_Full.notify_all();
		}
		if (task != nullptr)
		{
			task->exec();
		}
		//执行完任务之后空闲任务数量++
		freeThreadSize_++;
		LastTime = std::chrono::high_resolution_clock().now();
	}
	std::cout << std::this_thread::get_id() << std::endl;
	threads_.erase(threadId);
	curthreadSize_--;   //当前线程数量--
	freeThreadSize_--;  //空闲线程数量--
	std::stringstream ss;
	ss << "thead:";
	ss << std::this_thread::get_id();
	ss << " deleted";
	LOG(INFO, ss.str());
	cond_exit.notify_all();
	return;
}



//Result默认构造函数
Result::Result(std::shared_ptr<Task>sp, bool isVaild) :
	task_(sp),
	isVaild_(isVaild)
{
	task_->SetResult(this);
}

//Result::get用户调用这个函数获得返回值
Any Result::get()
{
	if (!isVaild_)
	{
		return "";
	}
	sem_.wait();  //task任务如果没有执行完，这里会阻塞用户线程
	return std::move(any_);
}

//Result::setVal子线程调用该函数设置返回值any
void Result::SetVal(Any any)
{
	any_ = std::move(any);
	sem_.post();
}
