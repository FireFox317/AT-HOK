/*
 * Reliability.cpp
 *
 *  Created on: Apr 13, 2018
 *      Author: timon
 */

#include "Reliability.h"

#include <iostream>

#include <chrono>

Reliability rel;

Reliability::Reliability() {
	// TODO Auto-generated constructor stub

}

Reliability::~Reliability() {
	// TODO Auto-generated destructor stub
}


void Reliability::setSendMessage(Message message){
	sendMessage = message;
	acknowledged = false;

}

void Reliability::checkTimestamp(std::string timestamp){
	if(sendMessage.getTimestamp() == timestamp){
		acknowledged = true;
		std::cout << "Correct ack" << std::endl;
	}
}

