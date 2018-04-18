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

Message::Message(std::string _sourceIP, std::string _destinationIP, std::string timestamp, std::string _data){
	destinationIP = _destinationIP;
	data = _data;
	sourceIP = IP;
	timeStamp = timestamp;
	sourceIP = _sourceIP;
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

std::string Message::getDestinationIP(){
	return destinationIP;
}

void Message::setData(std::string _data){
	data = _data;
}

std::string Message::getSourceIP(){
	return sourceIP;
}

std::string Message::getTimestamp(){
	return timeStamp;
}

bool Message::checkMultigroup(){
	return destinationIP == MULTIGROUP;
}

std::string Message::getComputerNumber(){
	std::vector<std::string> ip;
	split(sourceIP,".",ip);
	return ip[3];
}

std::string Message::getData(){
	return data;
}

bool Message::valid(){
	return !(destinationIP == "empty" && data == "empty" && timeStamp == "empty");
}

Message::~Message() {
	// TODO Auto-generated destructor stub
}

void Message::split(const std::string& s, const char* delim, std::vector<std::string>& v) {
	auto i = 0;
	auto pos = s.find(delim);
	while (pos != std::string::npos) {
	  v.push_back(s.substr(i, pos-i));
	  i = ++pos;
	  pos = s.find(delim, pos);

	  if (pos == std::string::npos)
		 v.push_back(s.substr(i, s.length()));
	}
}

