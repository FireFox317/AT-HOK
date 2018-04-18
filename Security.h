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
#include "Message.h"

class Security {
public:
	Security();

	virtual ~Security();
	void encriptMessage(Message& message);
	void decriptMessage(Message& message);
	std::string encriptData(std::string data, std::string key);
	std::string decriptData(std::string data, std::string key);

	void generateSessionKey();
private:
	std::map<std::string, std::string> keyTable;
};

extern Security chatsecurity;

#endif /* SECURITY_H_ */


