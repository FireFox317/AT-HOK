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
	std::cout << "Computer number: ";
	std::getline(std::cin, number);

	std::string ip = "192.168.5.";
	ip.append(number);

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



