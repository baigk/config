#include "KafkaProducer.h"
#include "KafkaTopics.h"

unsigned int partitioner_cb(const RdKafka::Topic *topic, const std::string *key,
		unsigned int partition_cnt, void *msg_opaque)
{
}

KafkaProducer::KafkaProducer(std::shared_ptr<MqConfig> config) {
	__conf = std::make_shared<KafkaProducerConfig>(config) 

	std::string errstr;
    __producer = std::shared_ptr<RdKafka::Producer>(RdKafka::Producer::create(__conf->getKafkaConfig().get(), errstr));
   	if (!__producer){
       	std::cerr << "Failed to create producer: " << errstr << std::endl;
	}
}

KafkaProducer::~KafkaProducer() {
	while (__producer->outq_len() > 0)
	{
		std::cerr << "Waiting for " << __producer->outq_len() << std::endl;
		__producer->poll(1000);
	}

	RdKafka::wait_destroyed(1000);
}
 
std::shared_ptr<KakfaProducer> KafaProducer::create(std::vertor<MqConfigItem> * config> = nullptr) {
	std::shared_ptr<KafkaProducer> p(new KafakProducer(config));
	return p;
}

unsigned int KafkaProducer::createTopic(std::vector<std::string> &topic, std::vertor<MqConfigItem> * config> = nullptr) {
    return KafakTopics::getInstance().createTopics(__producer, topic, config);
}

unsigned int KafkaProducer::createTopic(const std::string & topic, std::vertor<MqConfigItem> * config> = nullptr) {
    return KafakTopics::getInstance().createTopics(__producer, topic, config);
}

unsigned int KafkaProducer::updateTopicConf(const std::string & topic, std::vertor<MqConfigItem> * config> = nullptr) {
    return KafakTopics::getInstance().updateTopicConf(__producer, topic, config);
}

unsigned int KafkaProducer::deleteTopic(const std::string & topic) {
    return KafakTopics::getInstance().deleteTopic(__producer, topic, config);
}

unsigned int KafkaProducer::deleteTopic(const std::string & topic) {
    return KafakTopics::getInstance().deleteTopic(__producer, topic, config);
}

unsigned int KafkaProducer::produce(const std::string &topic, const std::string & message, void * param){
	std::shard_ptr<RdKafka::Topic> tpk = KafakTopics::getInstance().getTopic(topic);
	if (tpk == nullptr){
		return 1;
	}

	RdKafka::ErrorCode resp =
		__producer->produce(tpk.get(), partition,
		RdKafka::Producer::RK_MSG_COPY,
		const_cast<char*>(message.c_str()), message.size(),
		NULL, NULL);

	if (resp != RdKafka::ERR_NO_ERROR)
	{
		std::cerr << "% Produce failed: " <<
		RdKafka::err2str(resp) << std::endl;
		return -1;
	}

	//__producer->poll(timeout);

	return 0;
}

