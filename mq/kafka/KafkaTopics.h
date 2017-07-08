#pragma once

#include "KafkaConfig.h"

class KafkaTopics
{
public:

    static KafkaTopics & getInstance();
    unsigned int createTopic(shared_ptr<RdKafka::Producer> producer, string &topic, const MqConfig & config = MqConfig());
    unsigned int createTopic(shared_ptr<RdKafka::Producer> producer, vector<string> &topic, const MqConfig & config = MqConfig());
    unsigned int createTopic(shared_ptr<RdKafka::Producer> producer, const string & topic, const MqConfig & config = MqConfig());
    unsigned int updateTopicConf(shared_ptr<RdKafka::Producer> producer, const string & topic, const MqConfig & config = MqConfig());
    unsigned int deleteTopic(shared_ptr<RdKafka::Producer> producer, vector<string> &topic);
    unsigned int deleteTopic(shared_ptr<RdKafka::Producer> producer, const string & topic);
    shared_ptr<RdKafka::Topic> getTopic(const string &topic);

private:
	unordered_map<string, shared_ptr<RdKafka::Topic>> __topics;
};



