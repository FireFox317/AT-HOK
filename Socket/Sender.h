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
#include "../Message.h"

#include "SenderSocket.h"

namespace Sender{

	extern std::atomic<bool> wantToSend;
	extern std::atomic<bool> finished;
	extern std::string message;
	extern std::mutex message_mutex;


	void sendMessage(Message message);

	void closeSocket();

	void loop();

}


#endif /* SOCKET_SENDER_H_ */
