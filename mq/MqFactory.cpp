
#include "MqFactory.h"

MqFactory::MqFacory(){}
MqFactory::~MqFactory(){
	__mqInfo.clear();
}

unsigned int MqFactory::registerMqInfo(const std::string & type, const std::shared_ptr<MqInfo>  mqInfo){
	__mqInfo[type] = mqInfo

std::shared_ptr<Mq>  MqFactory::createConsumer(std::string & type = "kafka", std::vertor<MqConfigItem> * config> = nullptr) {
	auto it = __mqInfo.find(type);
	if (it == __mqInfo.end()) return nullptr;

	return it->second(config);
}

std::shared_ptr<Mq>  MqFactory::createProducer(std::string & type = "kafka", std::vertor<MqConfigItem> * config> = nullptr) {
	auto it = __mqInfo.find(type);
	if (it == __mqInfo.end()) return nullptr;

	return it->second(config);
}

