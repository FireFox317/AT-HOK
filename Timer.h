/*
 * Timer.h
 *
 *  Created on: Apr 16, 2018
 *      Author: timon
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <thread>
#include <functional>

class Timer {
	std::thread* th;
	bool running = false;
public:
	typedef std::chrono::milliseconds Interval;
	typedef std::function<void(void)> Timeout;

	void start(const Interval &interval, const Timeout &timeout);
	void stop();
};

#endif /* TIMER_H_ */
