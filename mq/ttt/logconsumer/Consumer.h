#ifndef _CONSUMER_
#define _CONSUMER_

#include <vector>
#include <string>
//#include <memory>
#include <stdint.h>
#include <unordered_map>
#include <thread>
//#include "ustd/thread.h"
#include "singleton.h"
#include "KafkaConsumer.h"

namespace ustd
{

class Consumer
{
USTD_DECLARE_SINGLETON(Consumer)
public:
	bool start();
	void wait();
	void save(bool immediate);

private:
	void read_config(const char* path = "/etc/maxmob/logconsumer/consumer.conf");
	void process_msg(const string& topic, const vector<char>& data);
	void worker(uKafkaConsumer c, const string& topic);
	void write_offset(const string& topic);
	int64_t read_offset(const string& topic);
	string get_full_name(const string& path, const string& file_name);
private:
	string host_;
	int port_;
	int partition_;
	int interval_;
	int count_;
	int64_t offset_;
	int timeout_;
	string offset_path_;
	string log_path_;
	uKafkaConsumer consumer_;
	unordered_map<string, int64_t> topics_;
	vector<thread> threads_;
	time_t last_time_;
};

}

#endif //_CONSUMER_