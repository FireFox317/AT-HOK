#include <thread>
#include <string>
#include <iostream>
#include "BlockingQueue.h"
#include "receiver.h"
#include "sender.h"

#define PORT 12345 //The port you want to use
#define IP "192.168.5.1" //The IP address of this computer "192.168.5.1"
#define GROUP "228.1.2.3" //The multicast group you want to use

BlockingQueue< std::string > q;

int main() {
	std::thread sender(sendPacket, IP, PORT, GROUP, "Hallo allemaal, wat fijn dat je er bent."); //start networking sending thread
	std::thread receiver(receivePacket, IP, PORT, GROUP, &q); //start network receiving thread
	
	while (1) {
		std::string message = q.pop();
		std::cout << "Received message: " << message << std::endl;
	}

	exit(0);
}
