#ifndef _KAFKA_CONSUMER_
#define _KAFKA_CONSUMER_

#include <vector>
#include <string>
#include <memory>
#include <stdint.h>
#include <unordered_map>
#include "rdkafkacpp.h"

#define KAFKA_MSG_F_FREE	0x1
#define KAFKA_MSG_F_COPY	0x2

namespace ustd
{

struct topic_info_t
{
	shared_ptr<RdKafka::Topic> topic_;
	int64_t offset_;

	topic_info_t() :
	topic_(nullptr), 
	offset_(RdKafka::Topic::OFFSET_BEGINNING)
	{

	}
};

class KafkaConsumer
{
public:
	KafkaConsumer();
	~KafkaConsumer();
	
public:
	bool init(const string &host, const int &port);
	bool create_topic(const string &topic);
	bool start_topic(const string &topic, const int &partition = 0, const int64_t &offset = RdKafka::Topic::OFFSET_BEGINNING);
	bool get_last_offset(const string &topic, int64_t &offset);
	int recv(vector<char> &data,
		const string &topic,
		const int &flag = KAFKA_MSG_F_COPY,
		const int &partition = 0,
		const int &timeout = 0);

	int recv_batch(vector<vector<char>> &data, const int &size,
		const string &topic,
		const int &flag = KAFKA_MSG_F_COPY,
		const int &partition = 0,
		const int &timeout = 0);
	void close();

private:
	void read_config(const char *path);
	topic_info_t *get_topic(const string &topic);

private:
	shared_ptr<RdKafka::Consumer> consumer_;
	shared_ptr<RdKafka::Conf> conf_;
	shared_ptr<RdKafka::Conf> tconf_;
	unordered_map<string, shared_ptr<topic_info_t>> topics_;
};

}

#endif //_KAFKA_CONSUMER_