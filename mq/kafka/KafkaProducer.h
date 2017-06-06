#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

class HashPartitionerCb : public RdKafka::PartitionerCb
{
public:
	unsigned int partitioner_cb(const RdKafka::Topic *topic, const std::string *key,
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

class KafkaProducer 
{
public:
	KafkaProducer(std::shared_ptr<MqConfig> config);
	virtual ~KafkaProducer();
 
	static std::shared_ptr<MqProducer> create(std::shared_ptr<MqConfig> config = nullptr);

	unsigned int publishMessage(const std::string &topic, const std::string & message, void *param = nullptr);

	unsigned int createTopic(std::vector<std::string> &topic, std::vertor<MqConfigItem> * config> = nullptr);
	unsigned int createTopic(const std::string & topic, std::vertor<MqConfigItem> * config> = nullptr);
	unsigned int updateTopicConf(const std::string & topic, std::vertor<MqConfigItem> * config> = nullptr);
	unsigned int deleteTopic(std::vector<std::string> &topic);
	unsigned int deleteTopic(const std::string & topic);

private:
	std::shared_ptr<RdKafka::Producer> __producer;
	std::shared_ptr<KafkaProducerConfig> __conf;
};
