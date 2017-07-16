
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

unsigned int MqFactory::registerMqInfo(const string & type, MqFactoryBase *mqInfo){
	__mqInfo = mqInfo;
    return 0;
}

#if 0
void  MqFactory::addConsumer(shared_ptr<MqConsumer> consumer) {
	__consumers.emplace_back(consumer);
}

void  MqFactory::addProducer(shared_ptr<MqProducer> producer) {
	__producers.emplace_back(producer);
}

void  MqFactory::addTopic(const string & topic, shared_ptr<MqTopic> topic) {
	__topics.insert(make_pair(topic, topic);
}


shared_ptr<MqTopic> MqFactory::getTopic(const string & topic) {
	auto it = __topics.find(topic);
	if (it ==  __topics.end()) return nullptr;

	return it->second;
}
#endif

