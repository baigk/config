#include "MqFactory.h"
#include "KafakProducer.h"
#include "KafkaConsumer.h"

void __attribute__((constructor)) kafkaActive() {
//	kafka->createConsumeConfigFunc = KakfaConsumerConfig::create;
//  kafka->createProducerConfigFunc = KafkaProducerConfig::create;
    kafka->createConsumerFunc = KakfaConsumer::create;
    kafka->createProducerFunc = KafkaProducer::create;

	(void)MqFactory::getInstance().registerMqInfo("kafka", kafka);
}
