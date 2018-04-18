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
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <vector>
#include <string>
#include <array>
#include <map>

#include "Message.h"

#ifndef SECURITY_H_
#define SECURITY_H_

class security
{
private:
	std::string myName;
	std::string myIP;
	std::string group;
	int port;
	std::map<std::string, std::string> keyTable;

//	std::string message, encriptedMessage, ivtemp;


//	void encodeMessage();
//	void decodeMessage();

public:
	security();
	~security();
//	void setInfo(std::string NAME, std::string ip, int port, std::string group);
//	void setEncriptedMessage(std::string message);
//	std::string getEncriptedMessage();
//	void generateKeyPair();
//	void senderHandshake();
//	void receiverHandshake(std::string data);



	void encriptMessage(Message& message);
	void decriptMessage(Message& message);
	std::string encriptData(std::string data, std::string key);
	std::string decriptData(std::string data, std::string key);

	void generateSessionKey();

//	bool flag = false;
};

extern security chatsecurity;

#endif /* SECURITY_H_ */
