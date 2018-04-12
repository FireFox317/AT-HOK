/*
 * Receiver.h
 *
 *  Created on: Apr 12, 2018
 *      Author: timon
 */

#ifndef SOCKET_RECEIVERTHREAD_H_
#define SOCKET_RECEIVERTHREAD_H_

#include "ip_config.h"
#include "BlockingQueue.h"
#include "Sender.h"

#include <thread>
#include "receiverTest.h"

namespace Receiver{


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

	BlockingQueue< std::string > q;

	void loop(){
		std::thread receiver_thread(receivePacket, IP, PORT, MULTIGROUP, &q); //start network receiving thread

		while (1) {
			std::string message = q.pop();

			std::vector<std::string> vec;
			split(message,"/",vec);

			std::string sourceIP = vec[0];
			std::string destinationIP = vec[1];
			std::string timeStamp = vec[2];
			std::string data = vec[3];

			std::string timeStampTemp;

			if(destinationIP == IP){
				// Received a message that is for us.
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
}



#endif /* SOCKET_RECEIVERTHREAD_H_ */
