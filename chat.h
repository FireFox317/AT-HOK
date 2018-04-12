/*
 * chat.h
 *
 *  Created on: Apr 11, 2018
 *      Author: ubuntu
 */

#include <iostream>
#include <string>
#include <thread>
#include "BlockingQueue.h"
#include "receiver.h"
#include "sender.h"

#ifndef CHAT_H_
#define CHAT_H_



class chat
{
protected:
    	std::string sentMessage;
    	std::string receivedMessage;

public:
    chat();
    virtual ~chat();

    bool newMessage = false;

    void setSentMessage(std::string message);
    void sendSentMessage(std::string ip, int port, std::string group);
    void setReceivedMessage(std::string message);
    static void receiver(std::string ip, int port, std::string group);
    void printReceivedMessage();
    void printSentMessage();
};

#endif /* CHAT_H_ */