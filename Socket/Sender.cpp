/*
 * Sender.cpp
 *
 *  Created on: Apr 13, 2018
 *      Author: timon
 */


#include "Sender.h"

#include "../Reliability.h"

std::atomic<bool> Sender::wantToSend(false);
std::atomic<bool> Sender::finished(false);
std::string Sender::message;
std::mutex Sender::message_mutex;

void Sender::sendMessage(Message message){
		std::lock_guard<std::mutex> lk(Sender::message_mutex);
		Sender::message = message.toString();
		Sender::wantToSend = true;
		if(!message.checkMultigroup() && !rel.retransmission && !(message.getData() == "ACK")){
			rel.setSendMessage(message);
		}
	}

void Sender::closeSocket(){
	Sender::finished = true;
	}

void Sender::loop(){
	SenderSocket senderSocket(IP, PORT, MULTIGROUP);
	while(!Sender::finished){
		if(Sender::wantToSend){
			std::lock_guard<std::mutex> lt(Sender::message_mutex);
			senderSocket.sendMessage(Sender::message);
			Sender::wantToSend = false;
		}
	}
}

