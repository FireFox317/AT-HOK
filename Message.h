/*
 * Message.h
 *
 *  Created on: Apr 11, 2018
 *      Author: timon
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>

class Message {
public:
	Message(std::string _destinationIP, std::string _data);
	std::string toString();
	virtual ~Message();
private:
	std::string sourceIP;
	std::string destinationIP;
	std::string timeStamp;
	std::string data;
};

#endif /* MESSAGE_H_ */
