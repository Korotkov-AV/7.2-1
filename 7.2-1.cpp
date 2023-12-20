// 7.2-1.cpp 



#include <iostream>
#include<thread>
#include<chrono>
#include<mutex>
#include<atomic>

using namespace std::chrono_literals;

std::mutex m;
std::atomic_uint count_clients{ 0 };

void Clients()
{
	do {
		std::unique_lock l(m);

		count_clients.store(count_clients.load() + 1);
		std::cout << "Client " << count_clients << " came" << std::endl;

		l.unlock();

		std::this_thread::sleep_for(1000ms);
	} while (count_clients < 10);

}

void Operator()
{
	std::this_thread::sleep_for(10ms);

	while (count_clients > 0) {

		std::unique_lock l(m);

		std::cout << "Client " << count_clients << " is completed" << std::endl;
		count_clients.store(count_clients.load() - 1);

		l.unlock();

		std::this_thread::sleep_for(2000ms);
	}
}

int main()
{
	std::thread t1(Clients);
	std::thread t2(Operator);

	t1.join();
	t2.join();

	return 0;
}
