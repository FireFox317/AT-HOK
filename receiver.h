#include "BlockingQueue.h"

int receivePacket(std::string ip, int port, std::string group, BlockingQueue<std::string>* q);

void closeSocket();
