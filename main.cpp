/*
 * main.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: timon
 */


#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <mutex>

#include <chrono>

#include "Socket/Sender.h"
#include "Message.h"
#include "Socket/Receiver.h"

#include "ThreadSafe.h"

int main(void){

	std::thread send_thread(Sender::loop);
	std::thread receive_thread(Receiver::loop);

	std::string number;
	std::cout << "Destination (255 is groupchat): ";
	std::getline(std::cin, number);

	std::string ip;
	if(number == "255"){
		ip = MULTIGROUP;
	} else {
		ip = "192.168.5.";
		ip.append(number);
	}

	std::cout << "Type 'quit' to close the application." << std::endl;
	bool loop = true;
	while (loop) {
		std::cout << "Message: ";
		std::string text;
		std::getline(std::cin,text);

		if(text == "quit"){
			loop = false;
			break;
		}
		Message mes(ip,text);
		Sender::sendMessage(mes);
	}

	Sender::closeSocket();
	Receiver::closeSocket();

	send_thread.join();
	receive_thread.detach();

	return 0;
}



