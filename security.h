/*
 * security.h
 *
 *  Created on: Apr 12, 2018
 *      Author: Niek & Ivo
 */

#include <vector>
#include <string>

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
	std::string message;

public:
	security();
	~security();
	void setInfo(std::string NAME, std::string IP, int PORT, std::string GROUP);
	void handshake();
};

#endif /* SECURITY_H_ */
