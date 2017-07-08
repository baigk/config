#ifndef _KAFKA_PRODUCER_
#define _KAFKA_PRODUCER_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "rdkafkacpp.h"

namespace ustd
{

class KafkaProducer
{
public:
	KafkaProducer();
	~KafkaProducer();
	
public:
	bool init(const string &host, const int &port, const bool &async = true, const int &size = 0x7fffffff);
	int send(const char *data,
		const int &size,
		const string &topic,
		const int &partition = 0,
		const int &timeout = 0);
	int send_batch(const vector<pair<const char *, const int &>> &data,
		const string &topic,
		const int &partition = 0,
		const int &timeout = 0);
	bool create_topic(const string &topic);
	void close();

private:
	void read_config(const char *path);
	RdKafka::Topic* get_topic(const string &topic);

private:
	shared_ptr<RdKafka::Producer> producer_;
	shared_ptr<RdKafka::Conf> conf_;
	shared_ptr<RdKafka::Conf> tconf_;
	unordered_map<string, shared_ptr<RdKafka::Topic>> topics_;
};

}

#endif //_KAFKA_PRODUCER_