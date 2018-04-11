/*
 * Sender.h
 *
 *  Created on: Apr 11, 2018
 *      Author: timon
 */

#ifndef SOCKET_SENDER_H_
#define SOCKET_SENDER_H_

#include <atomic>
#include <mutex>

#include "ip_config.h"

#include "SenderSocket.h"

namespace Sender{
	std::atomic<bool> wantToSend(false);
	std::atomic<bool> finished(false);
	std::string message;
	std::mutex message_mutex;

	void sendMessage(std::string data){
		std::lock_guard<std::mutex> lk(message_mutex);
		message = data;
		wantToSend = true;
	}

	void closeSocket(){
		finished = true;
	}

	void loop(){
		SenderSocket senderSocket(IP, PORT, MULTIGROUP);
		while(!finished){
			if(wantToSend){
				std::lock_guard<std::mutex> lt(message_mutex);
				senderSocket.sendMessage(message);
				wantToSend = false;
			}
		}
	}
}



#endif /* SOCKET_SENDER_H_ */
