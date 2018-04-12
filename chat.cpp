/*
 * chat.cpp
 *
 *  Created on: Apr 11, 2018
 *      Authors: Ivo & Niek
 */

#include "chat.h"

chat::chat()
{

}

chat::~chat()
{

}

void chat::setSentMessage(std::string message)
{
    sentMessage = message;
}


void chat::sendSentMessage(std::string ip, int port, std::string group)
{
    std::thread senderThread (sendPacket, ip, port, group, sentMessage);
    senderThread.join();
}

void chat::setReceivedMessage(std::string message)
{
    receivedMessage = message;
}

void chat::receiver(std::string ip, int port, std::string group)
{
	BlockingQueue< std::string > q;

    std::thread receiver(receivePacket, ip, port, group, &q);           //start network receiving thread

	while (1) {
		std::string message = q.pop();
		std::cout << std::left << "          ";
		std::cout.width(40); std::cout << std::right << message << std::endl;
	}
}

void chat::printReceivedMessage()
{
    std::cout << receivedMessage << std::endl;
}

void chat::printSentMessage()
{
    std::cout << sentMessage << std::endl;
}
