/*
 * Receiver.h
 *
 *  Created on: Apr 12, 2018
 *      Author: timon
 */

#ifndef SOCKET_RECEIVER_H_
#define SOCKET_RECEIVER_H_

#include "ip_config.h"
#include "BlockingQueue.h"
#include "Sender.h"
#include "ReceiverSocket.h"


#include <thread>

namespace Receiver{

	ReceiverSocket* receiverSocket;

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

	void closeSocket(){
		receiverSocket->closeSocket();
		delete receiverSocket;
	}

	void mainReceiveLoop(BlockingQueue< std::string > *q)
	{
		while (1) {
			std::string message = q->pop();

			std::vector<std::string> vec;
			split(message,"/",vec);

			std::string sourceIP = vec[0];
			std::string destinationIP = vec[1];
			std::string timeStamp = vec[2];
			std::string data = vec[3];

			std::string timeStampTemp;

			if(destinationIP == IP){
				// Received a message that is for us
				std::cout << "Message: " << message << std::endl;
			} else if(sourceIP == IP){
				// Received own message
				std::cout << "Received own message" << std::endl;
			} else {
				if(timeStamp == timeStampTemp){
					std::cout << "Received a retransmitted message" << std::endl;
				} else {
					timeStampTemp = timeStamp;
					Sender::sendMessage(message);
					std::cout << "Retransmitted the message" << std::endl;
				}
			}
		}
	}

	void loop(){
		BlockingQueue< std::string > q;
		std::thread receiver_thread(mainReceiveLoop, &q);
		receiverSocket = new ReceiverSocket(IP, PORT, MULTIGROUP, &q);
		receiverSocket->receive();
		receiver_thread.detach();
	}

}



#endif /* SOCKET_RECEIVER_H_ */
