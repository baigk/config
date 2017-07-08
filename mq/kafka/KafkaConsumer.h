
#pragma once

#include "KafkaConfig.h"

class ExampleEventCb : public RdKafka::EventCb {
public:
	void event_cb (RdKafka::Event &event);
};


class ConsumeCb : public RdKafka::ConsumeCb {
public:
	void consume_cb (RdKafka::Message &msg, void *opaque);
};

class KafkaConsumer : public MqConsumer
{
	public:
		~KafkaConsumer();
		void conf_dump();
		KafkaConsumer(shared_ptr<RdKafka::Conf> global, shared_ptr<RdKafka::Conf> toipc);

		unsigned int runFlag() {return __run;}
		void setRunFlag(unsigned int flag) {__run = flag;}

		RdKafka::KafkaConsumer *getConsumer(){return __consumer;}
         unsigned int subscribe(const string & topic);
	private:
		shared_ptr<RdKafka::Conf> __globalConf;
		shared_ptr<RdKafka::Conf> __topicConf;
		RdKafka::KafkaConsumer *__consumer;
		ConsumeCb *__consumeCb;
		ExampleEventCb * __eventCb;
		unsigned int __run;
		pthread_t __id;
};

