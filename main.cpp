/*
 * main.cpp
 *
 *  Created on: Apr 10, 2018
 *      Authors: Ivo & Niek
 */

#include <iostream>
#include "chat.h"
#include <thread>
#include "BlockingQueue.h"
#include "receiver.h"
#include "sender.h"
#include "security.h"
#include "IP"
 
int main(void)
{
	chat chat1;
	security chat1security;
	chat1security.setInfo("Niek", IP, PORT, GROUP);
	std::thread receiverThread (chat1.receiver, IP, PORT, GROUP);
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
	std::cout << "Goodbye! :)" << std::endl;
	receiverThread.detach();
	return 0;
}
