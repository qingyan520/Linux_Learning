#include"threadpool.h"

double sum1(double l, double r)
{
	double sum = 0;
	for (double i = l; i <= r; i++)
	{
		sum += i;
	}
	return sum;
}
int main()
{
	ThreadPool p;
	p.start(4);
	auto LasTime = std::chrono::high_resolution_clock().now(); 
	std::future<double>res1=p.submitTask(sum1, 1000000000, 2000000000);
	std::future<double>res2 = p.submitTask(sum1, 2000000001, 3000000000);
	std::future<double>res3 = p.submitTask(sum1, 3000000001, 4000000000);
	std::future<double>res4 = p.submitTask(sum1, 4000000001, 5000000000);
	std::future<double>res5 = p.submitTask(sum1, 5000000001, 6000000000);
	std::future<double>res6 = p.submitTask(sum1, 6000000001, 7000000000);
	double sum1 = res1.get();
	double sum2 = res2.get();
	double sum3 = res3.get();
	double sum4 = res4.get();
	double sum5 = res5.get();
	double sum6 = res6.get();
	auto now = std::chrono::high_resolution_clock().now();
	auto time = std::chrono::duration_cast<std::chrono::seconds>(now - LasTime);
	cout << "sum_1=" << sum1 + sum2 + sum3 + sum4 + sum5 + sum6 << endl;
	cout << "time_1=" << time.count() <<"s" << endl;

	LasTime = std::chrono::high_resolution_clock().now();
	double sum_2 = 0;
	for (double i = 1000000000; i <=7000000000 ; i++)
	{
		sum_2 += i;
	}
	now = std::chrono::high_resolution_clock().now();
	time = std::chrono::duration_cast<std::chrono::seconds>(now - LasTime);
	cout << "sum_2=" << sum_2 << endl;
	cout << "time_2=" << time.count() <<"s" << endl;
	int c = getchar();
	return 0;
}
