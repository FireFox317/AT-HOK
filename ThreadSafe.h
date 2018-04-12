/*
 * SafePrint.h
 *
 *  Created on: Apr 12, 2018
 *      Author: timon
 */

#ifndef THREADSAFE_H_
#define THREADSAFE_H_

#include <mutex>

extern std::mutex coutMutex;

#define ThreadSafe(X) { \
	std::lock_guard<std::mutex> lock(coutMutex); \
	X \
	}

#endif /* THREADSAFE_H_ */
