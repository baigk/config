#include "Singon.h"

class KafkaTopics
{
	DECLARE_SINGON(KafkaTopics);
public:
    unsigned int KafkaTopics::createTopic(std::shared_ptr<RdKafka::Producer> producer, std::string &topic, std::shared_ptr<KafkaTopicConfig> conf = nullptr);
    unsigned int KafkaTopics::createTopic(std::shared_ptr<RdKafka::Producer> producer, std::vector<std::string> &topic, std::vertor<MqConfigItem> * config = nullptr);
    unsigned int KafkaTopics::createTopic(std::shared_ptr<RdKafka::Producer> producer, const std::string & topic, std::vertor<MqConfigItem> * config = nullptr);
    unsigned int updateTopicConf(std::shared_ptr<RdKafka::Producer> producer, const std::string & topic, std::vertor<MqConfigItem> * config = nullptr);
    unsigned int deleteTopic(std::shared_ptr<RdKafka::Producer> producer, std::vector<std::string> &topic);
    unsigned int deleteTopic(std::shared_ptr<RdKafka::Producer> producer, const std::string & topic);
    std::shared_ptr<RdKafka::Topic> KafkaTopic::getTopic(const std::string &topic)

private:
	std::map<std::string, std::shared_ptr<RdKafka::Topic> __topics;
};



