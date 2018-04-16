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

#include "Timer.h"

class Reliability {
public:
	Reliability();
	virtual ~Reliability();
	void setSendMessage(Message message);
	void checkTimestamp(std::string timestamp);
	bool getAck();

	bool retransmission;
	Timer timer;
	Message sendMessage;
	int timeout;
//	bool isRetransmitting();
private:

	bool acknowledged;


};

extern Reliability rel;

#endif /* RELIABILITY_H_ */
