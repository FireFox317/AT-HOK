#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <queue>
#include <thread>
#include <list>
#include <iostream>
#include <mutex>
#include <condition_variable>

//using namespace std;

template<class T>
class BlockingQueue
{

	private:
		std::queue<T> theQueue;
		std::mutex mutex_queue;
		std::condition_variable cond;

	public:
		BlockingQueue<T>() {}

		T pop() {
			std::unique_lock<std::mutex> lk(mutex_queue);
			if ( theQueue.empty() ) { //we need to wait if there is nothing in the queue!
				cond.wait(lk);
			}
			T ret = theQueue.front();
			theQueue.pop();
			lk.unlock();
			return ret;
		}

		void push(T object) {
			mutex_queue.lock();
			theQueue.push(object);
			mutex_queue.unlock();
			cond.notify_one();
		}

};

#endif