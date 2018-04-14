/*
 * Sender.h
 *
 *  Created on: Apr 11, 2018
 *      Author: timon
 */

#ifndef SENDERSOCKET_H_
#define SENDERSOCKET_H_




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

#include <string>

class SenderSocket {
public:
	SenderSocket(std::string ip, int port, std::string group);
	virtual ~SenderSocket();
	bool sendMessage(std::string message);

private:
#ifdef _WIN32
	SOCKET sock;
#elif __linux__
	int sock;
#endif

	struct sockaddr_in multicastSender;



};

#endif /* SENDERSOCKET_H_ */
