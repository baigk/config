#pragma once

#include <unordered_map>
#include "Mq.h"

typedef shared_ptr<MqConsumer> (* CreateConsumerFunc)(const MqConfig &);
typedef shared_ptr<MqProducer> (* CreateProducerFunc)(const MqConfig &);

typedef struct tagMqInfo {
	CreateConsumerFunc createConsumerFunc;
	CreateProducerFunc createProducerFunc;
}MqInfo;

class MqFactory
{
    MqFactory & getInstance();

public:
	MqFactory();
	virtual ~MqFactory();

	shared_ptr<MqConsumer> createConsumer(const string & type, const MqConfig & config = MqConfig());

	shared_ptr<MqProducer> createProducer(const string & type, const MqConfig & config = MqConfig());

	unsigned int registerMqInfo(const string & type, const MqInfo & mqInfo) ;

private:
	unordered_map<string, MqInfo> __mqInfo;
};
