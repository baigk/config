#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <unordered_map>
#include <memory>
#include <vector>

using namespace std;

#include "rdkafkacpp.h"
#include "Mq.h"

class KafkaConfig {
public:
	KafkaConfig(){}

	unsigned int set(const MqConfig & config = MqConfig()){
		string errstr;

        for (auto & i : config) {
            _conf->set(i.first, i.second, errstr);
        }
	}
	virtual ~KafkaConfig(){}

	shared_ptr<RdKafka::Conf> getKafkaConig() {return _conf;}

protected:
	shared_ptr<RdKafka::Conf> _conf;
};

class KafkaGlobalConfig : public KafkaConfig {
public:
	KafkaGlobalConfig(const MqConfig & config = MqConfig()) {
		_conf = shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
		set(config);
	}
};	
	
class KafkaTopicConfig : public KafkaConfig {
public:
	KafkaTopicConfig(const MqConfig & config = MqConfig()) {
		_conf = shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));
		set(config);
	}
};

