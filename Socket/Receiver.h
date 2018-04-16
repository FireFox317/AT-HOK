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
#include "ReceiverSocket.h"

#include "../ThreadSafe.h"

#include "../Message.h"
#include "../Routing.h"


#include <thread>

namespace Receiver{

	ReceiverSocket* receiverSocket;
	std::thread* receiverThread;

	void closeSocket(){
		receiverSocket->closeSocket();
		receiverThread->detach();
		delete receiverThread;
		delete receiverSocket;
	}

	void mainReceiveLoop(BlockingQueue< std::string > *q)
	{
		Routing routing;
		while (1) {
			std::string data = q->pop();

			Message receivedMessage = routing.process(data);
			if(receivedMessage.valid()){
				std::cout << receivedMessage.getComputerNumber() << ">" << receivedMessage.getData() << std::endl;
			}
		}
	}

	void loop(){
		BlockingQueue< std::string > q;
		receiverThread = new std::thread(mainReceiveLoop, &q);
		receiverSocket = new ReceiverSocket(IP, PORT, MULTIGROUP, &q);
		receiverSocket->receive();
	}

}



#endif /* SOCKET_RECEIVER_H_ */