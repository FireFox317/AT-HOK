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
#include "IP"
 
int main(void)
{
	chat groupChat;
	std::thread receiverThread (groupChat.receiver, IP, PORT, GROUP);
	bool quit = false;
	while(!quit)
	{
		std::string message;
		std::getline(std::cin, message);
		if (message == ".")
		{
			quit = true;
			break;
		}
		groupChat.setSentMessage(message);
		groupChat.sendSentMessage(IP, PORT, GROUP);
	}
	std::cout << "Goodbye! :)" << std::endl;
	receiverThread.detach();
	return 0;
}
