#pragma once


class KafkaTopic
{
public:
	KafkaTopic();
	KafkaTopic(shared_ptr<RdKafka::Producer> producer, const string &topic, MqConfig & config);
    shared_ptr<RdKafka::Topic> getTopic(const string &topic);

public:
	shared_ptr<RdKafka::Topic> tpk;
	shared_ptr<KafkaTopicConfig> conf;
};



