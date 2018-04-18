/*
 * Routing.h
 *
 *  Created on: Apr 13, 2018
 *      Author: timon
 */

#ifndef ROUTING_H_
#define ROUTING_H_

#include <string>
#include <vector>

#include "Message.h"

class Routing {
public:
	Routing();
	virtual ~Routing();
	Message process(std::string data);
	void split(const std::string& s, const char* delim, std::vector<std::string>& v);
private:

	std::string timeStampTemp;
};

#endif /* ROUTING_H_ */
