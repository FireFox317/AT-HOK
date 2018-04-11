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
	std::cout << "hoi" << std::endl;
	chat groupChat;
	std::thread receiverThread (groupChat.receiver, IP, PORT, GROUP);
	std::string message;
	std::cout << "What message would you like to send?" << std::endl;
	std::cin >> message;
	groupChat.setSentMessage(message);
	groupChat.sendSentMessage(IP, PORT, GROUP);
	groupChat.printSentMessage();
	groupChat.setReceivedMessage("Dit bevat veelsteveel hoi's");
	groupChat.printReceivedMessage();
	std::cout << "hey" << std::endl;
	return 0;
}
