/*
 * Timer.cpp
 *
 *  Created on: Apr 16, 2018
 *      Author: timon
 */

#include "Timer.h"

void Timer::start(const Interval &interval, const Timeout &timeout){
	running = true;

	        th = new std::thread([=]()
	        {
	            while (running == true) {
	                std::this_thread::sleep_for(interval);
	                timeout();
	            }
	        });
}

void Timer::stop(){
	running = false;
	th->detach();
	delete th;
}
