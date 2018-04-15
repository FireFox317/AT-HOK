/*
 * Message.h
 *
 *  Created on: Apr 11, 2018
 *      Author: timon
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <vector>

class Message {
public:
	Message() = default;
	Message(std::string _destinationIP, std::string _data);
	Message(std::string _destinationIP, std::string timestamp, std::string _data);
	Message(std::string _sourceIP, std::string _destinationIP, std::string timestamp, std::string _data);

	bool valid();
	std::string toString();
	std::string getData();
	std::string getTimestamp();
	std::string getComputerNumber();

	virtual ~Message();
private:
	std::string sourceIP;
	std::string destinationIP;
	std::string timeStamp;
	std::string data;
	void split(const std::string& s, const char* delim, std::vector<std::string>& v);
};

#endif /* MESSAGE_H_ */
