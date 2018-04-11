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


#include "Socket/Sender.h"
#include "Socket/receiver.h"
#include "Socket/BlockingQueue.h"

#include "Socket/ip_config.h"

#include "Message.h"

BlockingQueue< std::string > q;

void split(const std::string& s, const char* delim, std::vector<std::string>& v) {
    auto i = 0;
    auto pos = s.find(delim);
    while (pos != std::string::npos) {
      v.push_back(s.substr(i, pos-i));
      i = ++pos;
      pos = s.find(delim, pos);

      if (pos == std::string::npos)
         v.push_back(s.substr(i, s.length()));
    }
}

int main(void){
	std::thread send_thread(Sender::loop);
	std::thread receiver_thread(receivePacket, IP, PORT, MULTIGROUP, &q); //start network receiving thread

	Message mes("192.168.5.2","Hello!");

	Sender::sendMessage(mes.toString());

	while (1) {
		std::string message = q.pop();

		std::vector<std::string> vec;
		split(message,"/",vec);

		if(vec[1] == IP){
			// Received a message that is for us.

		} else if(vec[0] == IP){
			// Received own message

		} else {

			Sender::sendMessage(message);
		}
		std::cout << "Received message: " << message << std::endl;
	}

	Sender::closeSocket();
	send_thread.join();
	receiver_thread.join();

	return 0;
}



