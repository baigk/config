#pragma once

#include "Mq.h"

class MqFactory {
public:
	virtual shared_ptr<MqConsumer> createConsumer(const MqConfig & config = MqConfig());
	
	virtual shared_ptr<MqProducer> createProducer(const MqConfig & config = MqConfig());
	
	unsigned int registerMqInfo(const string & type, MqFactory * mqInfo);
	
private:
	
	MqFactory * __mqInfo;
};

#if 0
class MqFactory
{
    static MqFactoryBase * getFactory(const string & type = "kafka");

	static MqFactory & getInstance();

public:
	MqFactory();
	virtual ~MqFactory();

    unsigned int registerMqInfo(const string & type, MqFactoryBase * mqInfo);

private:
    unordered_map<string, MqFactoryBase *> __mqInfo;
};
#endif
