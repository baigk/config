#include "KafkaTopic.h"
#include "KafkaConfig.h"

KafkaTopic::KafkaTopic(){}

KafkaTopic::KafkaTopic(shared_ptr<RdKafka::Producer> producer, string &topic, MqConfig & config){
	string errstr;

	conf = make_shared<KafkaTopicConfig>(config);

	tpk = shared_ptr<RdKafka::Topic>(RdKafka::Topic::create(producer.get(), topic, conf->getKafkaConig().get(), errstr));
	if (!tpk){
		cerr << "Failed to create topic: " << errstr << endl;
		return 1;
	}
	
	return 0;
}


