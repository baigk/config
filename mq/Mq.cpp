
#include "MqFactory.h"

MqConsumer::MqConsumer(){}
MqConsumer::MqConsumer(MqConfig & config): _config(config){
	MqConsumerManager::getInstance().addConsumer(getptr());
}

shared_ptr<MqConsumer> MqConsumer::getptr() {
    return shared_from_this();
}

shared_ptr<MqProducer> MqProducer::getptr() {
    return shared_from_this();
}

unsigned int MqConsumer::subscribe(const vector<string> &topics) {
	unsigned int ret = 0;
	for (auto & i : topics) {
		ret |= subscribe(i);
	}

	return ret;
}

unsigned int MqConsumer::subscribe(const string &topic) {
	auto ret = subscribeEx(topic);
	
	_topics.insert(make_pair(topic, ""));
	
	return ret;
}

MqProducer(){}

MqProducer(MqConfig & config): _config(config){
	MqProducerManager::getInstance().addProducer(getptr());
}

MqProducer & MqProducer::createTopic(const string & topic){
	auto t = MqTopicManager::getInstance().getTopic(topic);
	if (t != nullptr) {
		return *this;
	}

	t = createTopicEx(topic);
	if (t != nullptr) {
		cout << "create topic fail" << endl;
	}

	MqTopicManager::getInstance().addTopic(topic, t, getptr())

	return *this;
}

MqTopic::MqTopic(){}
MqTopic::MqTopic(const string & topic):_topic(topic){}


MqConsumerManager &MqConsumerManager::getInstance() {
	static MqConsumerManager instance;
	return instance
}

void MqConsumerManager::addConsumer(shared_ptr<MqConsumer> c) {
	__consumers.emplace_back(c);
}

MqProducerManager &MqProducerManager::getInstance() {
	static MqProducerManager instance;
	return instance
}

void MqProducerManager::addProducer(shared_ptr<MqProducer> p) {
	_producers.emplace_back(p);
}

MqTopicManager &MqTopicManager::getInstance() {
	static MqTopicManager instance;
	return instance
}

shared_ptr<MqTopic> MqTopicManager::getTopic(string & topic) {
	auto it = __topics.find(topic);
	if (it == __topics.end()){
		return nullptr;
	}

	return it->second;
}

void MqTopicManager::addTopic(const string & topic, shared_ptr<MqTopic> t, shared_ptr<MqProducer> p){
	__topics.insert(make_pair(topic, make_pair(t, p));
}

unsigned int MQ_Publish(const string & topic, const string & message, void *param) {
	auto t = MqTopicManager::getInstance().getTopic(topic);
	if (t == nullptr) {
		return 1;
	}

	return t->first.publishMessage(t->second, message, param);
}

