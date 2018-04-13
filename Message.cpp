/*
 * Message.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: timon
 */

#include "Message.h"

#include "Socket/ip_config.h"
#include <chrono>
#include <iostream>

Message::Message(std::string _destinationIP, std::string _data) {
	// TODO Auto-generated constructor stub
	destinationIP = _destinationIP;
	data = _data;
	sourceIP = IP;
	int64_t timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch().count();
	std::string timeStampString = std::to_string(timeSinceEpoch);
	timeStamp = timeStampString;
}

Message::Message(std::string _destinationIP, std::string timestamp, std::string _data){
	destinationIP = _destinationIP;
	data = _data;
	sourceIP = IP;
	timeStamp = timestamp;
}

std::string Message::toString(){
	std::string str;
	str.append(sourceIP);
	str.append("/");
	str.append(destinationIP);
	str.append("/");
	str.append(timeStamp);
	str.append("/");
	str.append(data);
	return str;
}

std::string Message::getTimestamp(){
	return timeStamp;
}

Message::~Message() {
	// TODO Auto-generated destructor stub
}

