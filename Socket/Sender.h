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
#include <condition_variable>
#include <iostream>

#include "ip_config.h"

#include "SenderSocket.h"

namespace Sender{
	std::condition_variable wantToSend;
	bool finished = false;
	std::string message;
	std::mutex message_mutex;

	void sendMessage(std::string data){
		std::lock_guard<std::mutex> lk(message_mutex);
		message = data;
		wantToSend.notify_one();
	}

	void closeSocket(){
		finished = true;
		wantToSend.notify_one();
	}

	void loop(){
		SenderSocket senderSocket(IP, PORT, MULTIGROUP);

		while(!finished){
			std::unique_lock<std::mutex> lk(message_mutex);
			wantToSend.wait(lk, []{return !finished;});
			senderSocket.sendMessage(message);
			lk.unlock();
		}
	}
}


#endif /* SOCKET_SENDER_H_ */
