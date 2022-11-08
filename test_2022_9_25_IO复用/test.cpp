#include"threadpool.h"
class Mytask :public Task
{
public:
	Mytask(double l, double r) {
		_l = l;
		_r = r;
	}
	Any Run()
	{
		double sum = 0;
		for (double i = _l; i <= _r; i++)
		{
			sum += i;
		}
		
		//std::this_thread::sleep_for(std::chrono::seconds(10));
		//cout << std::this_thread::get_id() << " 执行任务完成:" << endl;
		//std::this_thread::sleep_for(std::chrono::seconds(30));
		//cout << std::this_thread::get_id() << ":" << "Hello World!" << endl;
		return sum;
	}
private:
	double _l;
	double _r;
};
int main()
{
	
	{
		ThreadPool p;
		p.SetThreadPoolMode(PoolMode::CACHED_MODE);
		p.start(3);
		auto LasTime=std::chrono::high_resolution_clock().now();
		Result res1 = p.submitTask(std::make_shared<Mytask>(1000000000, 2000000000));
		Result res2 = p.submitTask(std::make_shared<Mytask>(2000000001, 3000000000));
		Result res3 = p.submitTask(std::make_shared<Mytask>(3000000001, 4000000000));
		Result res4 = p.submitTask(std::make_shared<Mytask>(4000000001, 5000000000));
		Result res5 = p.submitTask(std::make_shared<Mytask>(5000000001, 6000000000));

		double sum1 = res1.get().get_cast<double>();
		double sum2 = res2.get().get_cast<double>();
		double sum3 = res3.get().get_cast<double>();
		double sum4 = res4.get().get_cast<double>();
		double sum5 = res5.get().get_cast<double>();
		double sum_1 = sum1 + sum2 + sum3 + sum4 + sum5;
		auto now=std::chrono::high_resolution_clock().now();
    auto time=std::chrono::duration_cast<std::chrono::seconds>(now-LasTime);
		cout << "sum_1=" << sum_1<< endl;
		cout << "time_1=" << time.count() << "s" << endl;

		LasTime=std::chrono::high_resolution_clock().now();
		double sum_2 = 0;
		for (double i = 1000000000; i <= 6000000000; i++)
		{
			sum_2 += i;
		}
	  now=std::chrono::high_resolution_clock().now();
    time=std::chrono::duration_cast<std::chrono::seconds>(now-LasTime);
		cout << "sum_2=" << sum_2 << endl;
		cout << "time_2=" << time.count() << endl;
		getchar();
	}
	cout << "main over" << endl;



	//{
	//	ThreadPool p;
	//	p.start();
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//	p.submitTask(std::make_shared<Mytask>(1, 1));
	//}
	//cout << "main over" << endl;


	return 0;
}
