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
#include "BlockingQueue.h"
#include "receiver.h"
#include "sender.h"

#ifndef CHAT_H_
#define CHAT_H_



    
public:
    chat();
    virtual ~chat();

    bool newMessage = false;
    
    void setSentMessage(std::string message);
    void sendsentMessage(std::string ip, int port, std::string group);
    void setReceivedMessage(std::string message);
    void printReceivedMessage();
};

#endif /* CHAT_H_ */
