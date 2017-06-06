#ifndef __MQ_CONSUMER__H
#define __MQ_CONSUMER_H

#include "Mq.h"

class MqConsumer
{
public:
	unsigned int subscribe(const std::vector<std::string> &topics) {return 0;}
	unsigned int subscribe(const std::string &topic) {return 0;}
};
