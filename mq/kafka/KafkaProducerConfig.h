#ifndef __KAFKA_PRODUCER_CONFIG__H
#define __KAFKA_PRODUDER_CONFIG__H

#include "MqConfig.h"

class KafkaProducerConfig :public KafkaConfig
{
	public:
		static std::shared_ptr<KafakProducerConfig> create(std::vertor<MqConfigItem> * config>) {
			std::shared_ptr<KafkaProducerConfig> kpc(new KafkaProducerConfig);

			kpc->addConfigItem(config);

			return kpc;
		}
};


#endif
