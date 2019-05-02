#include <thread>
#include <mutex>
#include <vector>
#include <iostream>

int main()
{
	std::mutex myMutex;
	std::vector<std::thread> threads;

	int actionCount = 0;
	int threadCount = 10;
	int actionLength = 50000 / threadCount;

	for (int i = 0; i < threadCount; ++i)
	{
		threads.push_back(std::thread(
			[&](int low, int high) 
		{
			myMutex.lock();
			std::cout << "Thread Initiated\n";

			for (int j = low; j < high; ++j)
				actionCount++;

			std::cout << "Thread Ended\n";
			myMutex.unlock();
		}, i * actionLength, (i + 1) * actionLength
		));
	}

	for (auto& t : threads)
	{
		t.join();
	}

	std::cout << actionCount;

	return 0;
}
