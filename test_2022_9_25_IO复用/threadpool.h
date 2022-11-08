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
using std::cout;
using std::endl;

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

const int TASKQUE_MAX_SIZE_DEFAULT = 10;
const int THREAD_INIT_SIZE_DEFAULT = 5;
const int THREAD_MAX_SIZE_DEFAULT = 20;

enum class PoolMode {
	FIXED_MODE,
	CACHED_MODE
};


//可以用来接受任意返回值类型的Any类型
class Any {
public:
	Any() {

	}
	//删除拷贝构造和复制运算符重载
	Any(const Any&) = delete;
	Any& operator=(const Any&) = delete;

	//右值引用的拷贝构造和赋值运算符重载利用默认生成的就可以了
	Any(Any&&) = default;
	Any& operator= (Any&&) = default;
	template<class T>
	Any(T data) :ptr_(std::make_unique<Derive<T>>(data)) {

	}
	template<class T>
	T get_cast()
	{
		auto*p = dynamic_cast<Derive<T>*>(ptr_.get());
		if (p == nullptr)
		{
			//std::throw "type is error";
		}
		return p->get();
	}

	
private:
	class Base {
	public:
		virtual ~Base() {

		}
	};

	template<class T>
	class Derive :public Base {
	public:
		Derive(T data_) :data(data_) {

		}
		T get() {
			return data;
		}
	private:
		T data;
	};

	std::unique_ptr<Base>ptr_;
};

//用于主线程和子线程之间进行通信的Semaphore信号量
class Semaphore {
public:
	//等待资源
	void wait() {
		std::unique_lock<std::mutex>lock(mtx_);
		cond_.wait(lock, [&]()->bool {
			return limited_ > 0;
			});
		limited_--;
		//cout <<std::this_thread::get_id() <<"Wait:" << limited_ << "-------------------" << endl;
	
	}
	//得到资源
	void post() {
		std::unique_lock<std::mutex>lock(mtx_);
		limited_++;
		//cout <<std::this_thread::get_id() << " Post:" << limited_ << "+++++++++++++++++++" << endl;
		cond_.notify_all();
	}

private:
	int limited_=0;    
	std::condition_variable cond_;
	std::mutex mtx_;
};


class Task;  //前置声明Task;
class Result {
public:

	//传入当前执行的task智能指针
	Result(std::shared_ptr<Task> s, bool isSet = true);

	void SetVal(Any val);  //在task_中设置回调

	Any get();               //得到线程执行的返回值any
private:
	Any any_;  //存储线程执行完任务的返回值any
	Semaphore sem_;//存储
	std::shared_ptr<Task>task_;
	bool isSet_;
};



class Task {
public:

	virtual Any Run() = 0;      //提供给子类进行重写的接口
	void exec();                //专门用来执行Run任务并且将返回值写入res.setVal中

	void setRes(Result* res);   //设置res
private:

	Result* res_=nullptr;
};




class Thread {
public:
	using Func = std::function<void(int)>;
	Thread(Func _func) :id_(ID++),func_(_func) {

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


class ThreadPool{
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
			auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::func,this,std::placeholders::_1));
			int id = ptr->get_id();
			//std::cout << id << std::endl;
			threads_.emplace(id, std::move(ptr));
		}

		for (int i = 0; i < threadInitSize_; i++)
		{
			threads_[i]->start();
		}
	}

	//向线程池提交任务
	Result submitTask(std::shared_ptr<Task>sp);

private:
	std::unordered_map<int, std::unique_ptr<Thread>>threads_;       //存放线程的容器
	std::queue < std::shared_ptr<Task> >taskQue_;                    //存放任务的队列

	int threadInitSize_;              //线程池初始化线程个数
	int taskQueMaxSize_;              //任务队列中任务最大数量，一般为INT_MAX
	int threadMaxSize_;               //cached模式下最大线程个数，防止因为线程一直创建导致系统资源占用过多
	std::atomic_int taskQueSize_ = 0; //标记taskQue_的size

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
	void func(int id);
};
