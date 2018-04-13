/*
 * Reliability.h
 *
 *  Created on: Apr 13, 2018
 *      Author: timon
 */

#ifndef RELIABILITY_H_
#define RELIABILITY_H_

#include "Message.h"

class Reliability {
public:
	Reliability();
	virtual ~Reliability();
	void setSendMessage(Message message);
	void checkTimestamp(std::string timestamp);
private:
	Message sendMessage;
	bool acknowledged;
	int timeout;
};

extern Reliability rel;

#endif /* RELIABILITY_H_ */
