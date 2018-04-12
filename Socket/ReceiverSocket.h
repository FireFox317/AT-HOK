/*
 * ReceiverSocket.h
 *
 *  Created on: Apr 12, 2018
 *      Author: timon
 */

#ifndef SOCKET_RECEIVERSOCKET_H_
#define SOCKET_RECEIVERSOCKET_H_

#include <string>

#include "BlockingQueue.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
 //#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#elif __linux__
#include <sys/socket.h>    	//socket
#include <arpa/inet.h> 		//inet_addr
#include <unistd.h>  // needed to close socket file descriptor
#endif

class ReceiverSocket {
public:
	ReceiverSocket(std::string ip, int port, std::string group, BlockingQueue<std::string>* q);
	virtual ~ReceiverSocket();
	void receive();
	void closeSocket();
private:

#ifdef _WIN32
	SOCKET get_receive_socket(std::string ip, uint16_t port, std::string group);
#elif __linux__
	int get_receive_socket(std::string ip, uint16_t port, std::string group );
#endif

#ifdef _WIN32
	SOCKET rsock;
#elif __linux__
	int rsock;
#endif
	BlockingQueue<std::string>* blockQ;
};

#endif /* SOCKET_RECEIVERSOCKET_H_ */
