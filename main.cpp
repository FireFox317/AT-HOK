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
#include <IP>

#define PORT 12345 						//The port you want to use
//#define IP "192.168.5.4" 				//The IP address of this computer "192.168.5.1"
#define GROUP "228.1.2.3" 				//The multicast group you want to use

int main(void)
{
	std::thread receiver(receivedMessage, IP, PORT, GROUP)
	std::cout << "hoi" << std::endl;
	chat groupChat;
	std::string message;
	std::cout << "What message would you like to send?" << std::endl;
	message << std::cin;
	chat.setSentMessage(message);
	groupChat.sendSentMessage(IP, PORT, GROUP);
	groupChat.setReceivedMessage("Dit bevat veelsteveel hoi's");
	groupChat.printReceivedMessage();
	std::cout << "hey" << std::endl;
	return 0;
}


