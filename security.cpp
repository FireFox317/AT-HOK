/*
 * security.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: Niek & Ivo
 */

#include "security.h"

security::security()
{

}

security::~security()
{

}

void security::setInfo(std::string NAME, std::string IP, int PORT, std::string GROUP)
{
	myName = NAME;
	myIP = IP;
	port = PORT;
	group = GROUP;
}

void security::handshake()
{

}
