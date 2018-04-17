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
#include "../Storage.h"

#include "../main.h"

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
				if(receivedMessage.checkMultigroup()){
					storage.addGroupChatMessage(receivedMessage.getComputerNumber() + " > " + receivedMessage.getData());
					{
					    wxCommandEvent* event = new wxCommandEvent(MY_EVENT, wxID_ANY);
					    event->SetString(receivedMessage.getData().c_str());
					    wxQueueEvent(wxGetApp().mainFrame->GetEventHandler(),event);
					}

				} else {
					storage.addOneToOneMessage(receivedMessage.getSourceIP(), receivedMessage.getComputerNumber() + " > " + receivedMessage.getData());
				}
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
