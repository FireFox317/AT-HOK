/*
 * Reliability.h
 *
 *  Created on: Apr 13, 2018
 *      Author: timon
 */

#ifndef RELIABILITY_H_
#define RELIABILITY_H_

#include "Message.h"
#include <chrono>

class Reliability {
public:
	Reliability();
	virtual ~Reliability();
	void setSendMessage(Message message);
	void checkTimestamp(std::string timestamp);
private:
	Message sendMessage;
	bool acknowledged;
};

extern Reliability rel;

#endif /* RELIABILITY_H_ */
