#include "KafkaTopics.h"
#include "KafkaConfig.h"

unsigned int KafkaTopics::createTopic(std::shared_ptr<RdKafka::Producer> producer, std::string &topic, std::shared_ptr<KafkaTopicConfig> conf){
	std::string errstr;

	if (getTopic(topic))
	  return 0;

	td::shared_ptr<RdKafka::Topic> tpk = std::shared_ptr<RdKafka::Topic>(RdKafka::Topic::create(producer_.get(), topic, conf_.get(), errstr));
	if (!tpk){
		std::cerr << "Failed to create topic: " << errstr << std::endl;
		return 1;
	}

	__topics.insert(make_pair(topic, tpk));
	
	return 0;
}

unsigned int KafkaTopics::createTopic(std::shared_ptr<RdKafka::Producer> producer, std::vector<std::string> &topic, std::vertor<MqConfigItem> * config){
	std::shared_ptr<KafkaTopicConfig> conf = std::make_shared<KafkaTopicConfig>(config);
	unsigned int ret = 0;
	for (auto & i : topic) {
		ret |= createTopic(producer, i, conf);
	}

	return ret;
}

unsigned int KafkaTopics::createTopic(std::shared_ptr<RdKafka::Producer> producer, const std::string & topic, std::vertor<MqConfigItem> * config){
	std::shared_ptr<KafkaTopicConfig> conf = std::make_shared<KafkaTopicConfig>(config);
	
	return createTopic(producer, i, conf);
}
	    
unsigned int updateTopicConf(std::shared_ptr<RdKafka::Producer> producer, const std::string & topic, std::vertor<MqConfigItem> * config){
	return 0;
}
		    
unsigned int deleteTopic(std::shared_ptr<RdKafka::Producer> producer, std::vector<std::string> &topic) {
	return 0;
}
			   
unsigned int deleteTopic(std::shared_ptr<RdKafka::Producer> producer, const std::string & topic) {
	return 0;
}

std::shared_ptr<RdKafka::Topic> KafkaTopic::getTopic(const std::string &topic)
{
	auto it = topics_.find(topic);
	if(it != topics_.end())
	{
		return it->second.get();
	}

	return nullptr;
}

