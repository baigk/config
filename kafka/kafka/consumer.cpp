
#include <iostream>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <map>
#include <vector>

#include "rdkafkacpp.h"
#include "MqConfig.h"

void msg_consume(RdKafka::Message* message, void* opaque) {
    switch (message->err()) {
        case RdKafka::ERR__TIMED_OUT:
            break;

        case RdKafka::ERR_NO_ERROR:
            std::cerr << "Read msg at offset " << message->offset() << std::endl;
            std::cout << "Key: " << *message->key() << std::endl;
            printf("%.*s\n",
               static_cast<int>(message->len()),
               static_cast<const char *>(message->payload()));
            break;

        case RdKafka::ERR__PARTITION_EOF:
            std::cerr << "EOF reached for all partition(s)" << std::endl;
            break;

        case RdKafka::ERR__UNKNOWN_TOPIC:
        case RdKafka::ERR__UNKNOWN_PARTITION:
            std::cerr << "Consume failed: " << message->errstr() << std::endl;
            break;

        default:
            std::cerr << "Consume failed: " << message->errstr() << std::endl;
    }
}

class ExampleEventCb : public RdKafka::EventCb {
public:
	void event_cb (RdKafka::Event &event) {
		switch (event.type())
		{
			case RdKafka::Event::EVENT_ERROR:
				std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<  event.str() << std::endl;
				break;

			case RdKafka::Event::EVENT_STATS:
				std::cerr << "\"STATS\": " << event.str() << std::endl;
				break;

			case RdKafka::Event::EVENT_LOG:
				fprintf(stderr, "LOG-%i-%s: %s\n",  event.severity(), event.fac().c_str(), event.str().c_str());
				break;

			case RdKafka::Event::EVENT_THROTTLE:
				std::cerr << "THROTTLED: " << event.throttle_time() << "ms by " <<
					event.broker_name() << " id " << (int)event.broker_id() << std::endl;
				break;

			default:
				std::cerr << "EVENT " << event.type() <<
					" (" << RdKafka::err2str(event.err()) << "): " <<
					event.str() << std::endl;
				break;
		}
	}
};

#if 0
class ExampleRebalanceCb : public RdKafka::RebalanceCb {
private:
	static void part_list_print (const std::vector<RdKafka::TopicPartition*>&partitions){
		for (unsigned int i = 0 ; i < partitions.size() ; i++)
			std::cerr << partitions[i]->topic() << "[" << partitions[i]->partition() << "], ";

		std::cerr << "\n";
  }

public:
	void rebalance_cb (RdKafka::KafkaConsumer *consumer,
		     RdKafka::ErrorCode err, std::vector<RdKafka::TopicPartition*> &partitions) {
		std::cerr << "RebalanceCb: " << RdKafka::err2str(err) << ": ";

		part_list_print(partitions);

		if (err == RdKafka::ERR__ASSIGN_PARTITIONS) {
			consumer->assign(partitions);
			partition_cnt = (int)partitions.size();
		} else {
			consumer->unassign();
		}
	}
};
#endif

class ConsumeCb : public RdKafka::ConsumeCb {
public:
	void consume_cb (RdKafka::Message &msg, void *opaque) {
		msg_consume(&msg, opaque);
	}
};

class KafkaConsumer
{
	public:
		~KafkaConsumer();
		void conf_dump();
		KafkaConsumer(std::string broker ="localhost", MqConfig * globalConfig = nullptr, MqConfig * topicConfig = nullptr);

		unsigned int runFlag() {return __run;}
		void setRunFlag(unsigned int flag) {__run = flag;}

		RdKafka::KafkaConsumer *getConsumer(){return __consumer;}
	private:
		RdKafka::Conf *__globalConf;
		RdKafka::Conf *__topicConf;
		RdKafka::KafkaConsumer *__consumer;
		ConsumeCb *__consumeCb;
		ExampleEventCb * __eventCb;
		unsigned int __run;
		pthread_t __id;
};

KafkaConsumer::KafkaConsumer(std::string broker, MqConfig * global, MqConfig * topic) {
	std::string errstr;
	

	__globalConf  = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	__topicConf  = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	
    //ExampleRebalanceCb ex_rebalance_cb;
	//conf->set("rebalance_cb", &ex_rebalance_cb, errstr);
	
	auto configItem = [&](RdKafka::Conf *kafkaConfig, std::string & key, std::string & val) ->unsigned int{
		auto ret = kafkaConfig->set(key, val, errstr);
		if (ret != RdKafka::Conf::CONF_OK) {
			std::cout << "config fail " << ret << " " << key << " " << val << " " << errstr << std::endl;
			return 1;
		}

		return 0;
	};


    auto config = [=](MqConfig * mqConfig, RdKafka::Conf *kafkaConfig)->unsigned int {
		unsigned int ret = 0;
		for (auto & cfgItem : mqConfig->getConfig()) {
			ret |= configItem(kafkaConfig, cfgItem.first, cfgItem.second);
		}
		return ret;
	};

	global->update("metadata.broker.list", broker);

	config(global, __globalConf);
	config(topic, __topicConf);

	#if 0
	conf->set("group.id",  optarg, errstr)
	conf->set("compression.codec", optarg, errstr)  
	conf->set("statistics.interval.ms", optarg, errstr) 
	#endif
  
	__consumeCb = new ConsumeCb;

    auto ret = __globalConf->set("consume_cb", __consumeCb, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		std::cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << std::endl;
	}

	__eventCb = new ExampleEventCb;

	ret = __globalConf->set("event_cb", __eventCb, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		std::cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << std::endl;
	}


	ret = __globalConf->set("default_topic_conf", __topicConf, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		std::cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << std::endl;
	}

	__consumer = RdKafka::KafkaConsumer::create(__globalConf, errstr);
	if (__consumer == nullptr) {
		std::cerr << "Failed to create consumer: " << errstr << std::endl;
		return;
	}

	std::vector<std::string> topics = {"test", "test1"};
	RdKafka::ErrorCode err  = __consumer->subscribe(topics);
	if (err) {
		std::cout << "Failed to subscribe to " << topics.size() << " topics: "
              << RdKafka::err2str(err) << std::endl;
		return;
	}

	auto thread = [](void *para)->void *{
		KafkaConsumer * c = (KafkaConsumer *)para;

		while (c->runFlag()) {
			RdKafka::Message *msg = c->getConsumer()->consume(1000);
			if (msg != nullptr) {
				msg_consume(msg, NULL);
				delete msg;
			}
		}
	};

	auto ret1 = pthread_create(&__id, NULL, thread, this);
	if (ret1 != 0) {
		std::cout << "Create pthread error!n" << std::endl;
	}
}

KafkaConsumer::~KafkaConsumer() {
	__consumer->close();

	setRunFlag(0);

	pthread_join(__id, NULL);

	delete __consumer;
	delete __consumeCb;
	delete __eventCb;

	RdKafka::wait_destroyed(5000);
}


void KafkaConsumer::conf_dump() {
	auto dump = [](std::list<std::string> *conf) {
		for (std::list<std::string>::iterator it = conf->begin(); it != conf->end(); ) {
			std::cout << "    " <<  *it << " = ";
			it++;
			std::cout << *it << std::endl;
			it++; 
		}
		std::cout << std::endl;

    };

	if (__globalConf != nullptr) {
		std::cout << "global_config:" << std::endl;
		dump(__globalConf->dump());
	}

	if (__topicConf != nullptr) {
		std::cout << "topic_config:" << std::endl;
		dump(__topicConf->dump());
	}
}

int main (int argc, char **argv) {
	KafkaConsumer consumer;

	while(1) {
		sleep(100000);
	}
}
