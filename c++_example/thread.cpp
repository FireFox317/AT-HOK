#include <thread>
#include <string>
#include <iostream>

#include "ip_config.h"

#include "BlockingQueue.h"
#include "receiver.h"
#include "sender.h"

#define PORT 12345 //The port you want to use
#define GROUP "228.1.2.3" //The multicast group you want to use

BlockingQueue< std::string > q;

int main() {
	std::thread sender(sendPacket, IP, PORT, GROUP, "Hi, this is timon"); //start networking sending thread
	std::thread receiver(receivePacket, IP, PORT, GROUP, &q); //start network receiving thread
	
	while (1) {
		std::string message = q.pop();
		std::cout << "Received message: " << message << std::endl;
	}

	exit(0);
}
