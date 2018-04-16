/*
 * Reliability.cpp
 *
 *  Created on: Apr 13, 2018
 *      Author: timon
 */

#include "Reliability.h"

#include <iostream>

#include "Socket/Sender.h"

#include <chrono>
#include <iostream>

Reliability rel;

Reliability::Reliability() {
	// TODO Auto-generated constructor stub
	retransmission = false;
	acknowledged = false;
}

Reliability::~Reliability() {
	// TODO Auto-generated destructor stub
}


void Reliability::setSendMessage(Message message){
	sendMessage = message;
	acknowledged = false;
	timer.start(std::chrono::milliseconds(1000), [](){
			if(rel.getAck()){
				rel.timer.stop();
				rel.retransmission = false;
			} else {
				std::cout << "Send message again, because no ACK yet" << std::endl;
				rel.retransmission = true;
				Sender::sendMessage(rel.sendMessage);
			}
	});
}

void Reliability::checkTimestamp(std::string timestamp){
	if(sendMessage.getTimestamp() == timestamp){
		acknowledged = true;
		//std::cout << "Correct ack" << std::endl;
	}
}

bool Reliability::getAck(){
	return acknowledged;
}

//bool Reliability::isRetransmitting(){
//	return retransmission;
//}

