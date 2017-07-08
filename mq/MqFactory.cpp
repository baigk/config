
#include <memory>
#include "MqFactory.h"

MqFactory::MqFactory(){}
MqFactory::~MqFactory(){
	__mqInfo.clear();
}

MqFactory & MqFactory::getInstance() {
    static MqFactory instance;
    return instance;
}

unsigned int MqFactory::registerMqInfo(const string & type, const MqInfo  & mqInfo){
	__mqInfo[type] = mqInfo;
    return 0;
}

shared_ptr<MqConsumer>  MqFactory::createConsumer(const string & type = "kafka", const MqConfig & config) {
	auto it = __mqInfo.find(type);
	if (it == __mqInfo.end()) return nullptr;

	return it->second.createConsumerFunc(config);
}

shared_ptr<MqProducer>  MqFactory::createProducer(const string & type = "kafka", const MqConfig & config) {
	auto it = __mqInfo.find(type);
	if (it == __mqInfo.end()) return nullptr;

	return it->second.createProducerFunc(config);
}

