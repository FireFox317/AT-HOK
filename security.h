/*
 * security.h
 *
 *  Created on: Apr 12, 2018
 *      Author: Niek & Ivo
 */

#include <iostream>
#include <chrono>
#include <fstream>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/pssr.h>
#include <vector>
#include <string>
#include <array>
#include "sender.h"
#include "receiver.h"

#ifndef SECURITY_H_
#define SECURITY_H_

class security
{
private:
	std::string myName;
	std::string myIP;
	std::string group;
	int port;
	std::vector<std::vector<std::string>> keyTable;
	std::string message, encriptedMessage;

	void generateSessionKey();
	void encodeMessage();
	void decodeMessage();

public:
	security();
	~security();
	void setInfo(std::string NAME, std::string IP, int PORT, std::string GROUP);
	void setEncriptedMessage(std::string message);
	std::string getEncriptedMessage();
	void generateKeyPair();
	void senderHandshake();
	void receiverHandshake();
};

#endif /* SECURITY_H_ */
