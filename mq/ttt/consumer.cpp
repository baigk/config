
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
            cerr << "Read msg at offset " << message->offset() << endl;
            cout << "Key: " << *message->key() << endl;
            printf("%.*s\n",
               static_cast<int>(message->len()),
               static_cast<const char *>(message->payload()));
            break;

        case RdKafka::ERR__PARTITION_EOF:
            cerr << "EOF reached for all partition(s)" << endl;
            break;

        case RdKafka::ERR__UNKNOWN_TOPIC:
        case RdKafka::ERR__UNKNOWN_PARTITION:
            cerr << "Consume failed: " << message->errstr() << endl;
            break;

        default:
            cerr << "Consume failed: " << message->errstr() << endl;
    }
}

class ExampleEventCb : public RdKafka::EventCb {
public:
	void event_cb (RdKafka::Event &event) {
		switch (event.type())
		{
			case RdKafka::Event::EVENT_ERROR:
				cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<  event.str() << endl;
				break;

			case RdKafka::Event::EVENT_STATS:
				cerr << "\"STATS\": " << event.str() << endl;
				break;

			case RdKafka::Event::EVENT_LOG:
				fprintf(stderr, "LOG-%i-%s: %s\n",  event.severity(), event.fac().c_str(), event.str().c_str());
				break;

			case RdKafka::Event::EVENT_THROTTLE:
				cerr << "THROTTLED: " << event.throttle_time() << "ms by " <<
					event.broker_name() << " id " << (int)event.broker_id() << endl;
				break;

			default:
				cerr << "EVENT " << event.type() <<
					" (" << RdKafka::err2str(event.err()) << "): " <<
					event.str() << endl;
				break;
		}
	}
};

#if 0
class ExampleRebalanceCb : public RdKafka::RebalanceCb {
private:
	static void part_list_print (const vector<RdKafka::TopicPartition*>&partitions){
		for (unsigned int i = 0 ; i < partitions.size() ; i++)
			cerr << partitions[i]->topic() << "[" << partitions[i]->partition() << "], ";

		cerr << "\n";
  }

public:
	void rebalance_cb (RdKafka::KafkaConsumer *consumer,
		     RdKafka::ErrorCode err, vector<RdKafka::TopicPartition*> &partitions) {
		cerr << "RebalanceCb: " << RdKafka::err2str(err) << ": ";

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
		KafkaConsumer(string broker ="localhost", MqConfig * globalConfig = nullptr, MqConfig * topicConfig = nullptr);

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

KafkaConsumer::KafkaConsumer(string broker, MqConfig * global, MqConfig * topic) {
	string errstr;
	

	__globalConf  = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	__topicConf  = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	
    //ExampleRebalanceCb ex_rebalance_cb;
	//conf->set("rebalance_cb", &ex_rebalance_cb, errstr);

	auto configItem = [&](RdKafka::Conf *kafkaConfig, string key, string val)->unsigned int{
		auto ret = kafkaConfig->set(key, val, errstr);
		if (ret != RdKafka::Conf::CONF_OK) {
			cout << "config fail " << ret << " " << key << " " << val << " " << errstr << endl;
			return 1;
		}

		return 0;
	};


    auto config = [&](MqConfig * mqConfig, RdKafka::Conf *kafkaConfig)->unsigned int {
		unsigned int ret = 0;
		for (auto & cfgItem : mqConfig->getConfig()) {
			ret |= configItem(kafkaConfig, cfgItem.first, cfgItem.second);
		}
		return ret;
	};

	#if 0
	global->update("metadata.broker.list", broker);

	config(global, __globalConf);
	config(topic, __topicConf);
	#endif
	#if 0
	conf->set("group.id",  optarg, errstr)
	conf->set("compression.codec", optarg, errstr)  
	conf->set("statistics.interval.ms", optarg, errstr) 
	#endif
  
	__consumeCb = new ConsumeCb;

    auto ret = __globalConf->set("consume_cb", __consumeCb, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << endl;
	}

	__eventCb = new ExampleEventCb;

	ret = __globalConf->set("event_cb", __eventCb, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << endl;
	}


	ret = __globalConf->set("default_topic_conf", __topicConf, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << endl;
	}

	__consumer = RdKafka::KafkaConsumer::create(__globalConf, errstr);
	if (__consumer == nullptr) {
		cerr << "Failed to create consumer: " << errstr << endl;
		return;
	}

	vector<string> topics = {"test", "test1"};
	RdKafka::ErrorCode err  = __consumer->subscribe(topics);
	if (err) {
		cout << "Failed to subscribe to " << topics.size() << " topics: "
              << RdKafka::err2str(err) << endl;
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
		cout << "Create pthread error!n" << endl;
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
	auto dump = [](list<string> *conf) {
		for (list<string>::iterator it = conf->begin(); it != conf->end(); ) {
			cout << "    " <<  *it << " = ";
			it++;
			cout << *it << endl;
			it++; 
		}
		cout << endl;

    };

	if (__globalConf != nullptr) {
		cout << "global_config:" << endl;
		dump(__globalConf->dump());
	}

	if (__topicConf != nullptr) {
		cout << "topic_config:" << endl;
		dump(__topicConf->dump());
	}
}

int main1 (int argc, char **argv) {
	KafkaConsumer consumer;

	while(1) {
		sleep(100000);
	}
}
