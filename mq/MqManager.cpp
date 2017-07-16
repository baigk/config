#include "MqFactory.h"

class MqManager {
public:
	
private:
	vector<shared_ptr<MqProducer>> __producers;
	vector<shared_ptr<MqConsumer>> __consumer;
	vector<shared_ptr<MqTopic>> __topic;
}

unsigned int KafkaProducer::createTopic(vector<string> &topic, const MqConfig & config = MqConfig()) {
         return KafakTopics::getInstance().createTopics(__producer, topic, config);
          }

 unsigned int KafkaProducer::createTopic(const string & topic, const MqConfig & config = MqConfig()) {
          return KafakTopics::getInstance().createTopics(__producer, topic, config);
           }

 unsigned int KafkaProducer::updateTopicConf(const string & topic, const MqConfig & config = MqConfig()) {
          return KafakTopics::getInstance().updateTopicConf(__producer, topic, config);
           }

 unsigned int KafkaProducer::deleteTopic(const string & topic) {
          return KafakTopics::getInstance().deleteTopic(__producer, topic, config);
           }

 unsigned int KafkaProducer::deleteTopic(const string & topic) {
          return KafakTopics::getInstance().deleteTopic(__producer, topic, config);
           }
void __attribute__((constructor)) kafkaActive() {
//	kafka->createConsumeConfigFunc = KakfaConsumerConfig::create;
//  kafka->createProducerConfigFunc = KafkaProducerConfig::create;
    kafka->createConsumerFunc = KakfaConsumer::create;
    kafka->createProducerFunc = KafkaProducer::create;

	(void)MqFactory::getInstance().registerMqInfo("kafka", kafka);
}

