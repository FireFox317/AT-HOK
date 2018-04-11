#include <string>
#include <cstring>
#include <stdio.h>
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <windows.h>
#elif __linux__
#include <sys/socket.h>    	//socket
#include <arpa/inet.h> 		//inet_addr
#include <unistd.h>  // needed to close socket file descriptor
#endif

void sendPacket(std::string ip, int port, std::string group, std::string message) {
	/**
	 * Create a new datagram socket 
	 */
	#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// make socket
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "error opening socket!" << std::endl;
		WSACleanup();
		exit(20);
	}
	#elif __linux__
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		std::cout << "Error opening socket!" << std::endl;
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
	  perror("Local interface failed");
	  exit(2);
	}

	/**
	 * Fill in the sockaddr_in to specify the destination of datagrams
	 * In this case this is simply a port and multicast group address
	 */
	struct sockaddr_in multicastSender = {0};
	multicastSender.sin_family = AF_INET;
	//multicastSender.sin_addr.s_addr = inet_addr(group.c_str());
	inet_pton(AF_INET, group.c_str(), &multicastSender.sin_addr.s_addr);
	multicastSender.sin_port = htons(port);

	int len;
	len = message.size();
	char * data = new char[len]; //allocate some memory for our data
	memcpy(data, message.c_str(), len); // write string into packet data
	if (sendto(sock, data, len, 0, (struct sockaddr*)&multicastSender,sizeof(struct sockaddr_in)) < 0) //sent a UDP packet containing our example data
		    perror("Sendto failed");
	free(data);

	return;
}
