/*
 * chat.h
 *
 *  Created on: Apr 11, 2018
 *      Authors: Ivo & Niek
 */

#include <iostream>
#include <string>

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
    void setReceivedMessage(std::string message);
    void printReceivedMessage();
};

#endif /* CHAT_H_ */
