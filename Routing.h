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

class Routing {
public:
	Routing();
	virtual ~Routing();
	std::string process(std::string data);
private:
	void split(const std::string& s, const char* delim, std::vector<std::string>& v);
};

#endif /* ROUTING_H_ */
