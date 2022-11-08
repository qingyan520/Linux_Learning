//利用gdb分析死锁问题
#include"ThreadPool.h"
#include<chrono>

/*

ThreadPool Pool;
Pool.start(4);
class Mytask:public

*/


class MyTask :public Task
{
public:
	MyTask(unsigned long long  l, unsigned long long  r) :begin(l), end(r)
	{

	}
	Any run()
	{
		double  sum = 0;
		for (unsigned long long i = begin; i <= end; i++)
		{
			sum += i;
		}
		std::cout << std::this_thread::get_id() << ":" << sum << std::endl;
		//std::this_thread::sleep_for(std::chrono::seconds(10));
		return sum;
	}
private:
	unsigned long long  begin;
	unsigned long long end;
};
int main()
{
	{
		ThreadPool p;
		p.SetPoolMode(PoolMode::CACHED_MODE);
		p.SetThreadsMaxSize(20);
		p.start(5);
		clock_t begin = clock();
		Result res = p.submitTask(std::make_unique<MyTask>(1, 1000000000));
		Result res1 = p.submitTask(std::make_unique<MyTask>(1000000001, 2000000000));
		Result res2 = p.submitTask(std::make_unique<MyTask>(2000000001, 3000000000));
		Result res3 = p.submitTask(std::make_unique<MyTask>(3000000001, 4000000000));
		Result res4 = p.submitTask(std::make_unique<MyTask>(4000000001, 5000000000));
		Result res5 = p.submitTask(std::make_unique<MyTask>(5000000001, 6000000000));
		Result res6 = p.submitTask(std::make_unique<MyTask>(6000000001, 7000000000));
		double sum = res.get().cast_<double>();
		double sum2 = res1.get().cast_<double>();
		double sum3 = res2.get().cast_<double>();
		double sum4 = res3.get().cast_<double>();
		double sum5 = res4.get().cast_<double>();
		double sum6 = res5.get().cast_<double>();
		double sum7 = res6.get().cast_<double>();

		clock_t end = clock();
		std::cout << "-------------线程池-------:" << end - begin << "ms" << std::endl;
		std::cout << "sum1=" << sum << std::endl;
		std::cout << "sum2=" << sum2 << std::endl;
		std::cout << "sum1=" << sum + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 << std::endl;
		begin = clock();
		double ret = 0;
		for (unsigned long long i = 1; i <= 7000000000; i++)
		{
			ret += i;
		}
		end = clock();
		std::cout << "--------------:" << end - begin << "ms" << std::endl;
		std::cout << "sum2=" << ret << std::endl;
	}
	std::cout << "main over" << std::endl;
	
	int c = getchar();

}
