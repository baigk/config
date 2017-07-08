#include "KafkaProducer.h"
#include "KafkaTopics.h"

unsigned int partitioner_cb(const RdKafka::Topic *topic, const string *key,
		unsigned int partition_cnt, void *msg_opaque)
{
	return 0;
}

KafkaProducer::KafkaProducer(vector<MqConfigItem> * config) {
	__conf = make_shared<KafkaProducerConfig>(config) 

	string errstr;
    __producer = shared_ptr<RdKafka::Producer>(RdKafka::Producer::create(__conf->getKafkaConfig().get(), errstr));
   	if (!__producer){
       	cerr << "Failed to create producer: " << errstr << endl;
	}
}

KafkaProducer::~KafkaProducer() {
	while (__producer->outq_len() > 0)
	{
		cerr << "Waiting for " << __producer->outq_len() << endl;
		__producer->poll(1000);
	}

	RdKafka::wait_destroyed(1000);
}

unsigned int KafkaProducer::produce(const string &topic, const string & message, void * param){
	shard_ptr<RdKafka::Topic> tpk = KafakTopics::getInstance().getTopic(topic);
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
		cerr << "% Produce failed: " <<
		RdKafka::err2str(resp) << endl;
		return -1;
	}

	//__producer->poll(timeout);

	return 0;
}

