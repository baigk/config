#include "MqFactory.h"
#include "KafkaConsumer.h"
#include "KafkaProducer.h"

class KafkaFactory : public MqFactory {
public:
	static KafkaFactory * getInstance();
	
	virtual shared_ptr<MqConsumer> createConsumer(const MqConfig & config = MqConfig());
	
	virtual shared_ptr<MqProducer> createProducer(const MqConfig & config = MqConfig());
	
	//virtual shared_ptr<MqTopic> createTopic(const string & topic, const MqConfig & config = MqConfig());
};

KafkaFactory * KafkaFactory::getInstance() {
	static KafkaFactory instance;

	return &instance;
}

shared_ptr<MqConsumer> KafkaFactory::createConsumer(const MqConfig & config) {
	shared_ptr<MqConsumer> consumer(new KafkaConsumer(config));

	return consumer;
}

shared_ptr<MqProducer> KafkaFactory::createProducer(const MqConfig & config) {
	shared_ptr<MqProducer> producer(new KafkaProducer(config));

	return producer;
}

#if 0
shared_ptr<MqTopic>  KafkaFactory::createTopic(const string & topic, const MqConfig & config) {

	shared_ptr<KafkaTopic> topic = make_shared<KafkaTopic>(topic, config);

	addTopic(topic);

	return topic;
}	
#endif

void __attribute__((constructor)) kafkaActive() {
	(void)MqFactory::getInstance().registerMqInfo("kafka", KafkaFactory::getInstance());
}

