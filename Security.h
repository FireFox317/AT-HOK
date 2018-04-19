/*
 * Security.h
 *
 *  Created on: Apr 18, 2018
 *      Author: timon
 */

#ifndef SECURITY_H_
#define SECURITY_H_

#include <map>
#include <string>

#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>


#include "Message.h"

class Security {
public:
	Security();
	virtual ~Security();

	void loadKeys();
	void setupConnection(std::string ip);
	void acceptConnection(Message message);

	void generateKeyPair();

	void encriptMessage(Message& message);
	void decriptMessage(Message& message);
	std::string encriptData(std::string data, std::string key);
	std::string decriptData(std::string data, std::string key);

private:
	std::map<std::string, CryptoPP::RSA::PublicKey> publicKeyList;
	CryptoPP::RSA::PrivateKey privateKey;

	std::map<std::string, std::string> sessionKeyList;








//
//	void generateSessionKey();
//private:
//	std::map<std::string, std::string> publicKeyList;
//	std::map<std::string, std::string> sessionKeyList;
};

extern Security chatsecurity;

#endif /* SECURITY_H_ */


