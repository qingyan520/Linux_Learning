#pragma once
#pragma once
#include<iostream>
#include<unordered_map>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<functional>
#include<memory>
#include<queue>
#include<sstream>
#include<atomic>
#include<future>
using std::cout;
using std::endl;
using namespace std;
//日志打印
#define INFO 1
#define WARNING 2
#define ERROR_ 3
#define FALTA 4

#define LOG(level,message)  log(#level,message,__FILE__,__LINE__)
static void log(std::string level, std::string message, std::string file_name, int line) {
	//显示当前事件
	char now[1024] = { 0 };
	time_t tt = time(nullptr);
	struct tm* ttime;
	ttime = localtime(&tt);
	strftime(now, 1024, "%Y-%m-%d %H:%M:%S", ttime);
	// cout << "[" << now << "" << "][" << level << "]" << "[" << message << "]" << "[" << file_name << "]" << "[" << line << "]" << endl;

	printf("[%s][%s][%s][%s][%d]\n", now, level.c_str(), message.c_str(), file_name.c_str(), line);

}

const int TASKQUE_MAX_SIZE_DEFAULT = 2;
const int THREAD_INIT_SIZE_DEFAULT = 5;
const int THREAD_MAX_SIZE_DEFAULT = 20;

enum class PoolMode {
	FIXED_MODE,
	CACHED_MODE
};

class Thread {
public:
	using Func = std::function<void(int)>;
	Thread(Func _func) :id_(ID++), func_(_func) {

	}


	void start()
	{
		std::thread t(func_, id_);
		t.detach();
	}
	int get_id()
	{
		return id_;
	}
private:
	int id_;
	static std::atomic_int ID;
	Func func_;
};
std::atomic_int Thread::ID(0);

class ThreadPool {
public:
	//构造函数初始化
	ThreadPool() :threadInitSize_(THREAD_INIT_SIZE_DEFAULT),
		taskQueMaxSize_(TASKQUE_MAX_SIZE_DEFAULT),
		threadMaxSize_(THREAD_MAX_SIZE_DEFAULT),
		taskQueSize_(0),
		curThreadSize_(0),
		freeThreadSize_(0),
		mode_(PoolMode::FIXED_MODE),
		IsRunning_(false) {

	}

	~ThreadPool() {
		IsRunning_ = false;
		std::unique_lock<std::mutex>lock(taskMtx_);
		Not_Empty.notify_all();
		cond_exit.wait(lock, [&]()->bool {
			return threads_.size() == 0;
			});
	}

	//设置任务队列最大值
	void SetTaskQueMaxSize(int size = TASKQUE_MAX_SIZE_DEFAULT)
	{
		if (IsPoolRunning())
		{
			LOG(WARNING, "ThreadPool is already Runing,can not set taskQue max size!");
			return;
		}
		this->taskQueMaxSize_ = size;
	}

	//设置线程池模式
	void SetThreadPoolMode(PoolMode mode = PoolMode::FIXED_MODE) {
		if (IsPoolRunning())
		{
			LOG(WARNING, "ThreadPool is already Runing,can not set ThreadPool Mode!");
		}
		this->mode_ = mode;
	}

	//设置cached模式下线程的最大值
	void SetThreasMaxSize(int size = THREAD_MAX_SIZE_DEFAULT) {
		if (mode_ == PoolMode::FIXED_MODE)
		{
			LOG(WARNING, "ThreadPool is FIXED_MODE,can not set threads max size!");
			return;
		}
		if (IsPoolRunning())
		{
			LOG(WARNING, "ThreadPool is already Runing,can not threads mas size!");
			return;
		}
		this->threadMaxSize_ = size;
	}

	//start启动线程池
	void start(int size = std::thread::hardware_concurrency()) {
		IsRunning_ = true;
		this->threadInitSize_ = size;
		this->curThreadSize_ = size;
		this->freeThreadSize_ = size;
		for (int i = 0; i < threadInitSize_; i++)
		{
			auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::func, this, std::placeholders::_1));
			int id = ptr->get_id();
			//std::cout << id << std::endl;
			threads_.emplace(id, std::move(ptr));
		}

		for (int i = 0; i < threadInitSize_; i++)
		{
			//cout << i << endl;
			threads_[i]->start();
		}
	}

	//向线程池提交任务
	template<typename Func,typename...Args>
	auto submitTask(Func&& func,Args&&... args) -> std::future<decltype(func(args...))>
	{
		//先打包好任务，然后获取锁
		using RTtype = decltype(func(args...));
		auto ptr = std::make_shared<std::packaged_task<RTtype()>>(
			std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
		std::future<RTtype>ret = ptr->get_future();

		std::unique_lock<std::mutex>lock(this->taskMtx_);
		if (Not_Full.wait_for(lock, std::chrono::seconds(3), [&]()->bool {
			return taskQue_.size() < taskQueMaxSize_;
			}) == false)
		{
			LOG(WARNING, "taskQue is full");
			auto task = std::make_shared<std::packaged_task<RTtype()>>(
				[]()->RTtype {
					return RTtype();
				}
			);
			(* task)();
			return  task->get_future();
		}
			//std::cout << "打包任务成功" << endl;
			//现在获取锁之后，向任务队列添加任务，同时判断是否为cached模式，如果是cached模式，那么还需要我们
			//判断当前空闲线程数量是不是小于任务的数量同时线程数量小于cached模式最大线程数量

			//向任务队列添加元素
			taskQue_.emplace([ptr]()->void {
				(*ptr)();
				});
		//	cout << "添加任务成功" << endl;
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
			return ret;
	}

private:
	using Task = std::function<void()>;
	std::unordered_map<int, std::unique_ptr<Thread>>threads_;       //存放线程的容器
	std::queue <Task>taskQue_;                    //存放任务的队列

	int threadInitSize_;              //线程池初始化线程个数
	int taskQueMaxSize_;              //任务队列中任务最大数量，一般为INT_MAX
	int threadMaxSize_;               //cached模式下最大线程个数，防止因为线程一直创建导致系统资源占用过多
	std::atomic_int taskQueSize_; //标记taskQue_的size

	std::atomic_int curThreadSize_;               //当前线程个数，即工作线程+睡眠线程
	std::atomic_int freeThreadSize_;              //记录当前空闲的睡眠线程

	std::mutex taskMtx_;                  //互斥锁，使得对taskQue_队列操作是原子的
	std::condition_variable Not_Empty;    //条件变量，代表当前任务队列不为空，可以有线程进行消费了
	std::condition_variable Not_Full;     //条件变量，代表当前taskQue_不满，我们可以向任务队列中添加数据了
	std::condition_variable cond_exit;    //条件变量，退出时判断任务队列是否为空，此时如果有任务还在执行，那么就执行完当前任务后再退出

	PoolMode mode_;                  //记录线程池当前工作状态
	bool IsRunning_;                 //判断线程池是否在运行

	bool IsPoolRunning()    //判断线程池是否在运行
	{
		return IsRunning_;
	}
	//执行线程任务
	void func(int id)
	{
		//cout << std::this_thread::get_id() << "执行线程" << endl;
		//标记每个线程最后一次访问时间
		auto LastTime = std::chrono::high_resolution_clock::now();
		Task task;
		for (;;)
		{

			//cout << std::this_thread::get_id() << ":taskQueSIze:" << taskQue_.size() << endl;

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
							//cout << std::this_thread::get_id() << " " << curThreadSize_ << " " << threadInitSize_ << endl;
							if (mul.count() >= 3 && curThreadSize_ > threadInitSize_)
							{
								//cout << "杀死线程" << std::this_thread::get_id() << " " << curThreadSize_ << " " << threadInitSize_ << endl;
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

			//cout << "执行任务" << endl;
			if(task!=nullptr)
			task();

			freeThreadSize_++;
			LastTime = std::chrono::high_resolution_clock::now(); //更新线程最后一次访问时间
		}
	}
};
