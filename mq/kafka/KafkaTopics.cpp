#include "KafkaTopics.h"
#include "KafkaConfig.h"

KafkaTopics & KafkaTopics::getInstance() {
    static KafkaTopics instance;
    return instance;
}
                                            }
unsigned int KafkaTopics::createTopic(shared_ptr<RdKafka::Producer> producer, string &topic, const MqConfig & config){
	string errstr;

	if (getTopic(topic))
	  return 0;

	td::shared_ptr<RdKafka::Topic> tpk = shared_ptr<RdKafka::Topic>(RdKafka::Topic::create(producer_.get(), topic, conf_.get(), errstr));
	if (!tpk){
		cerr << "Failed to create topic: " << errstr << endl;
		return 1;
	}

	__topics.insert(make_pair(topic, tpk));
	
	return 0;
}

unsigned int KafkaTopics::createTopic(shared_ptr<RdKafka::Producer> producer, vector<string> &topic, const MqConfig & config){
	shared_ptr<KafkaTopicConfig> conf = make_shared<KafkaTopicConfig>(config);
	unsigned int ret = 0;
	for (auto & i : topic) {
		ret |= createTopic(producer, i, conf);
	}

	return ret;
}

unsigned int KafkaTopics::createTopic(shared_ptr<RdKafka::Producer> producer, const string & topic, const MqConfig & config){
	shared_ptr<KafkaTopicConfig> conf = make_shared<KafkaTopicConfig>(config);
	
	return createTopic(producer, i, conf);
}
	    
unsigned int KafkaTopic::updateTopicConf(shared_ptr<RdKafka::Producer> producer, const string & topic, const MqConfig & config){
	return 0;
}
		    
unsigned int KafkaTopic::deleteTopic(shared_ptr<RdKafka::Producer> producer, vector<string> &topic) {
	return 0;
}
			   
unsigned int KafkaTopic::deleteTopic(shared_ptr<RdKafka::Producer> producer, const string & topic) {
	return 0;
}

shared_ptr<RdKafka::Topic> KafkaTopic::getTopic(const string &topic)
{
	auto it = topics_.find(topic);
	if(it != topics_.end())
	{
		return it->second.get();
	}

	return nullptr;
}

