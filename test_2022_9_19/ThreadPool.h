#pragma once
#include<iostream>
#include<queue>
#include<vector>
#include<functional>
#include<memory>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<chrono>
#include<string>
#include<unordered_map>
#include<sstream>
#include<atomic>
#include<memory>
#define INFO 1
#define WARNING 2
#define ERROR_ 3
#define FALTA 4
using namespace std;
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

const int TASK_MAX_THRESHOLD = 10;   //任务队列最大上限
const int THREAD_INIT_SIZE = 8;        //线程初始化数量
const int THREAD_CACHED_MAX_SIZE = 128;  //cached模式最大线程数量


//线程池模式：固定模式和动态增长模式
enum class PoolMode
{
	FIXED_MODE,      //固定数量的线程池
	CACHED_MODE      //线程数量可以动态增长的线程池
};



//Any类：可以接受任意类型的返回值
class Any
{
public:

	//删除左值引用的拷贝构造和赋值重载
	Any() = default;
	Any(const Any&) = delete;
	Any& operator=(const Any&) = delete;

	//保留右值引用的移动构造和赋值重载
	Any& operator=(Any&&) = default;
	Any(Any&&) = default;

	template<class T>
	Any(T data) :
		_ptr(std::make_unique<Derive<T>>(data))
	{

	}
	template<class T>
	T cast_()
	{
		Derive<T>* p = dynamic_cast<Derive<T>*>(_ptr.get());
		if (p == nullptr)
		{
			LOG(ERROR_, "type is incompatiable!");
			throw "type is incompatiable!";
		}
		return p->getval();

	}


private:

	class Base {
	public:
		virtual ~Base() = default;
	};
	template<class T>
	class Derive :public Base
	{
	public:
		Derive(T data) :_data(data)
		{
		}
		T getval()
		{
			return _data;
		}
	private:
		T _data;
	};

	std::unique_ptr<Base>_ptr;
};


//信号量模拟实现
class Sempahore
{
public:

	void wait()
	{
		std::unique_lock<std::mutex>_lck(mtx_);
		cond_.wait(_lck, [&]()->bool {
			return sLimited_ > 0;
			});
		sLimited_--;
	}
	void post()
	{
		std::unique_lock<std::mutex>_lck(mtx_);
		sLimited_++;
		cond_.notify_all();
	}
private:
	int sLimited_ = 0;
	std::mutex mtx_;
	std::condition_variable cond_;
};


//前置声明Task
class Task;
//返回值类型
class Result
{
public:
	Result(std::shared_ptr<Task>sp, bool isVaild = true);

	//setVal设置任务的返回值
	void SetVal(Any any);
	//get方法，用户调用这个方法获得任务的返回值
	Any get();

private:
	Any any_;    //存储Any返回值
	Sempahore sem_; //信号量
	std::shared_ptr<Task>task_;
	bool isVaild_;
};


//任务类
class Task
{
public:
	//利用继承，执行线程池中的任务
	virtual Any run() = 0;

	void exec()
	{
		if (res != nullptr)
			res->SetVal(run());
	}


	void SetResult(Result* res_)
	{
		res = res_;
	}


private:
	Result* res = nullptr;
};

//线程类
class Thread
{
public:
	using ThreadFunc = std::function<void(int)>;
	Thread(ThreadFunc _func);
	~Thread();

	void start();
	int GetId();
private:
	Thread::ThreadFunc func_;
	static std::atomic_int threadId;  // 线程id
	int id;
};

//线程池
class ThreadPool
{
public:
	ThreadPool();             //构造函数
	~ThreadPool();            //析构函数

	void SetTaskQueMaxHold(int maxHold = TASK_MAX_THRESHOLD);  //设置任务队列数量上限
	void SetPoolMode(PoolMode _mode);     //设置线程池模式，默认为FIXED_MODE
	void SetThreadsMaxSize(int size= THREAD_CACHED_MAX_SIZE);     //设置cached模式最大线程数量

	Result submitTask(std::shared_ptr<Task> sp);  //向任务队列中提交任务
	void start(int size = std::thread::hardware_concurrency());                          //启动线程池

	ThreadPool& operator=(const ThreadPool&) = delete; //禁止线程池进行拷贝构造和赋值重载
	ThreadPool(const ThreadPool&) = delete;
private:
	std::unordered_map<int,std::unique_ptr<Thread>>threads_;    //线程容器，存储线程
	std::queue<std::shared_ptr<Task>>taskQue_;       //任务队列，存储任务

	std::atomic_int taskSize_;      //记录任务队列中任务地数量
	int threadInitSize_;            //线程初始数量
	int taskQueMaxHold_;            //任务队列任务最大值

	int threadsMaxSize_;            //cached模式时threadpool线程的最大值
	std::atomic_int curthreadSize_;             //当前线程的数量
	std::atomic_int freeThreadSize_;            //当前空闲线程的数量

	std::mutex mtxQue_;                  //保证任务队列线程安全
	std::condition_variable not_Empty;   //任务队列不空
	std::condition_variable not_Full;    //任务队列不满
	std::condition_variable cond_exit;   //退出时的条件变量

	PoolMode mode;                       //代表线程池的模式
	std::atomic_bool isRunning=false;          //代表线程池是否运行

private:
	void threadFunc(int threadId);               //线程函数

	bool isPoolRunning();             //判断当前线程池是否运行
};

