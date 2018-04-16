/*
 * main.cpp
 *
 *  Created on: Apr 10, 2018
 *      Authors: Ivo & Niek
 */

#include <iostream>
#include <thread>
#include "BlockingQueue.h"
#include "receiver.h"
#include "sender.h"
#include "chat.h"
#include "security.h"
#include "IP"
 
int main(void)
{
	chat chat1;
	security chat1security;
	chat1security.setInfo("Niek", IP, PORT, GROUP);
	chat1security.generateKeyPair();
	chat1security.senderHandshake();
	std::cout << "System: Welcome to chatroom" << std::endl;
	std::thread receiverThread (chat1.receiver, IP, PORT, GROUP, &chat1security);
	bool quit = false;
	while(!quit)
	{
		std::string message;
		std::getline(std::cin, message);
		if (message == "quit")
		{
			quit = true;
			break;
		}
		chat1.setSentMessage(message);
		chat1.sendSentMessage(IP, PORT, GROUP);
	}
	std::cout << "System: Left chatroom" << std::endl;
	receiverThread.detach();
	return 0;
}
