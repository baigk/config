#ifndef __KAFKA_CONSUMER_CONFIG__H
#define __KAFKA_CONSUMER_CONFIG__H

#include "MqConfig.h"

class KafkaConsumerConfig :public MqConfig
{
	public:
		static std::shared_ptr<KafakConsumerConfig> create(std::vertor<MqConfigItem> * config>) {
			std::shared_ptr<KafkaConsumerConfig> kcc(new KafkaConsumerConfig);

			kcc->addConfigItem(config);

			return kcc;
		}
};


#endif
