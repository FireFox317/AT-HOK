/*
 * Sender.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: timon
 */

#include <iostream>
#include <cstring>
#include "SenderSocket.h"
#include "../ThreadSafe.h"

SenderSocket::SenderSocket(std::string ip, int port, std::string group) {
	// TODO Auto-generated constructor stub
	/**
		 * Create a new datagram socket
		 */
		#ifdef _WIN32
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		// make socket
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == INVALID_SOCKET) {
			ThreadSafe(std::cout << "error opening socket!" << std::endl;)
			WSACleanup();
			exit(20);
		}
		#elif __linux__
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock < 0) {
			ThreadSafe(std::cout << "Error opening socket!" << std::endl;)
			exit(20);
		}
		int reuse = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
		#endif

		/**
		 * Enable/Disable loopback. (loop = 0 is disabled, loop = 1 is enabled)
		 * This allows you to receive multicast packets you yourself have send.
		 * Obviously enabling this is not really useful for anything but testing
		 */
		char loop = 0;
		if(setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loop, sizeof(loop)) < 0){
			perror("Failed to disable loopback");
			#ifdef _WIN32
			WSACleanup();
			#elif __linux__
			close(sock);
			#endif
			exit(7);
		}

		/**
		 * Set the interface to send multicast packets on.
		 * This should be our own IP address
		 */
		struct in_addr local = {0};
		//local.s_addr = inet_addr(ip.c_str());
		inet_pton(AF_INET, ip.c_str(), &local.s_addr);
		if(setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&local, sizeof(local)) < 0)
		{
			#ifdef _WIN32
			WSACleanup();
			#elif __linux__
			close(sock);
			#endif
		  perror("Local interface failed");
		  exit(2);
		}

		/**
		 * Fill in the sockaddr_in to specify the destination of datagrams
		 * In this case this is simply a port and multicast group address
		 */
		multicastSender = {0};
		multicastSender.sin_family = AF_INET;
		//multicastSender.sin_addr.s_addr = inet_addr(group.c_str());
		inet_pton(AF_INET, group.c_str(), &multicastSender.sin_addr.s_addr);
		multicastSender.sin_port = htons(port);

}

bool SenderSocket::sendMessage(std::string data){
	int len = data.size();
	char * c_data = new char[len]; //allocate some memory for our data
	memcpy(c_data, data.c_str(), len); // write string into packet data
	if (sendto(sock, c_data, len, 0, (struct sockaddr*)&multicastSender,sizeof(struct sockaddr_in)) < 0){ //sent a UDP packet containing our example data
		perror("Sendto failed");
		free(c_data);
		return false;
	}
	free(c_data);
//	ThreadSafe(std::cout << "Packet send! Size: " << len << " Data: " << data << std::endl;)
	return true;
}


SenderSocket::~SenderSocket() {
	// TODO Auto-generated destructor stub
	#ifdef _WIN32
	WSACleanup();
	#elif __linux__
	close(sock);
	#endif
}

