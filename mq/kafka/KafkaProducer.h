#pragma once

#include "KafkaConfig.h"

class HashPartitionerCb : public RdKafka::PartitionerCb
{
public:
	unsigned int partitioner_cb(const RdKafka::Topic *topic, const string *key,
		unsigned int partition_cnt, void *msg_opaque);

private:
	static inline unsigned int djb_hash (const char *str, size_t len)
	{
		unsigned int hash = 5381;
		for (size_t i = 0 ; i < len ; i++)
			hash = ((hash << 5) + hash) + str[i];
		return hash;
	}
};

class KafkaProducer : MqProducer
{
public:
	KafkaProducer(const MqConfig & config = MqConfig());
	virtual ~KafkaProducer();

	unsigned int publishMessage(shared_ptr<MqTopic> t, const string & message, void *param = nullptr);

	virtual shared_ptr<MqTopic> createTopicEx(const string & topic);
private:
	shared_ptr<RdKafka::Producer> __producer;
};
