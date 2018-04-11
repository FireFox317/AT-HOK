/*
 * main.cpp
 *
 *  Created on: Apr 10, 2018
 *      Authors: Ivo & Niek
 */

#include <iostream>
#include "chat.h"

int main(void)
{
	std::cout << "hoi" << std::endl;
	chat groupChat;
	groupChat.setReceivedMessage("Dit bevat veelsteveel hoi's");
	groupChat.printReceivedMessage();
	std::cout << "hey" << std::endl;
	return 0;
}


