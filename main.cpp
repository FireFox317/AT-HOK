/*
 * main.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: timon
 */


#include <iostream>
#include <thread>
#include <map>


#include "Socket/Sender.h"
#include "Socket/receiver.h"
#include "Socket/BlockingQueue.h"

#include "Socket/ip_config.h"

#include "Message.h"

BlockingQueue< std::string > q;

int main(void){
	std::thread send_thread(Sender::loop);
	std::thread receiver_thread(receivePacket, IP, PORT, MULTIGROUP, &q); //start network receiving thread

	Message mes("192.168.5.2","Hello!");

	Sender::sendMessage(mes.toString());

	while (1) {
		std::string message = q.pop();
		if(message == "Type: Routing\nRequest"){
			Sender::sendMessage("Type: Routing\nResponse");
		}
		std::cout << "Received message: " << message << std::endl;
	}

	Sender::closeSocket();
	send_thread.join();
	receiver_thread.join();

	return 0;
}



