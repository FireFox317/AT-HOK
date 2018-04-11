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


void chat::sendsentMessage(std::string ip, int port, std::string group)
{
    std::thread sender(sentMessage, ip, port, group);
}

void chat::setReceivedMessage(std::string message)
{
    receivedMessage = message;
}

void chat::printReceivedMessage()
{
    std::cout << receivedMessage << std::endl;
}
