
#include "KafkaConsumer.h"
#include "KafkaTopic.h"
#include "unistd.h"

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

void ExampleEventCb::event_cb (RdKafka::Event &event) {
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


void ConsumeCb::consume_cb (RdKafka::Message &msg, void *opaque) {
	msg_consume(&msg, opaque);
}

KafkaConsumer::KafkaConsumer(shared_ptr<RdKafka::Conf> global, shared_ptr<RdKafka::Conf> toipc) {
	string errstr;

    #if 0
	global->update("metadata.broker.list", broker);
    config(global, __globalConf);
	config(topic, __topicConf);
	conf->set("group.id",  optarg, errstr)
	conf->set("compression.codec", optarg, errstr)  
	conf->set("statistics.interval.ms", optarg, errstr) 
	#endif

	__globalConf = make_shared<KafkaGlobalConfig>(config);
	__topicConf = make_shared<KafkaTopicConfig>(config);
  
	__consumeCb  = new ConsumeCb;
	__eventCb = new ExampleEventCb;
	__consumer = RdKafka::KafkaConsumer::create(global, errstr);
	if (__consumer == nullptr) {
		cerr << "Failed to create consumer: " << errstr << endl;
		return;
	}
    
    auto ret = global->set("consume_cb", __consumeCb, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << endl;
	}

	ret = global->set("event_cb", __eventCb, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << endl;
	}

	#if 0

	ret = global->set("default_topic_conf", topic, errstr);
	if (ret != RdKafka::Conf::CONF_OK) {
		cout << "config fail " << ret << " " << "consume_cb"  << " " << errstr << endl;
	}


	vector<string> topics = {"test", "test1"};
	RdKafka::ErrorCode err  = __consumer->subscribe(topics);
	if (err) {
		cout << "Failed to subscribe to " << topics.size() << " topics: "
              << RdKafka::err2str(err) << endl;
		return;
	}
    #endif

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

unsigned int KafkaConsumer::subscribe(const string & topic) {
     RdKafka::ErrorCode err  = __consumer->subscribe(topic);
     if (err) {
          cout << "Failed to subscribe to " << topics.size() << " topics: "
               << RdKafka::err2str(err) << endl;
          return 1;
     }
                                     }
     return 0;
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
