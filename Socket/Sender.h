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
	extern std::atomic<bool> wantToSend;
	extern std::atomic<bool> finished;
	extern std::string message;
	extern std::mutex message_mutex;


	void sendMessage(std::string data);

	void closeSocket();

	void loop();
}



#endif /* SOCKET_SENDER_H_ */
