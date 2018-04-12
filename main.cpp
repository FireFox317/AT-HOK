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
#include "Message.h"
#include "Socket/ReceiverThread.h"





int main(void){
	std::thread send_thread(Sender::loop);
	std::thread receive_thread(Receiver::loop);


	Message mes("192.168.5.2","Hello!");

	bool loop = true;
	while (loop) {
		std::string text;
		std::getline(std::cin, text);
		if(text == "."){
			Sender::sendMessage(mes.toString());
		}
		if(text == "quit"){
			loop = false;
		}
	}


	Sender::closeSocket();
	closeReceiver();

	send_thread.join();
	receive_thread.detach();

	return 0;
}



