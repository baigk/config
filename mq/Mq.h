#pragma once

#include <string>
#include <vector>

using namespace std;

using MqConfigItem = pair<string, string>;
using MqConfig     = vector<MqConfigItem>;

class Mq 
{
};

class MqConsumer : public Mq, public enable_shared_from_this
{
public:
	MqConsumer();
	MqConsumer(MqConfig & config);
    unsigned int subscribe(const vector<string> &topics) {return 0;}
    unsigned int subscribe(const string &topic) {return 0;}

	virtual unsigned int subscribeEx(const string &topic) {return 0;}

	shared_ptr<MqConsumer> getptr();
protected:
	unorderd_map<string topic, string t> _topics;
	MqConfig _config;
};

class MqProducer : public Mq
{
public:
	MqProducer();
	MqProducer(MqConfig & config);
	
    shared_ptr<MqProducer> getptr();
	
    unsigned int publishMessage(shared_ptr<MqTopic> t, const string & message, void *param = nullptr) {return 0;}

	MqProducer & createTopic(const string & topic);

	virtual shared_ptr<MqTopic> createTopicEx(const string & topic);
protected:
	MqConfig _config;

};

class MqTopic : public Mq {
public:
	MqTopic();
	MqTopic(const string & topic);
	
protected:
	string _topic;
	//vector<shared_ptr<MqConsumer>> __consumers;
	//shared_ptr<MqProducer> __producer;
};

class MqConsumerManager {
public: 
	MqConsumerManager &getInstance();
	void addConsumer(shared_ptr<MqConsumer> c);

private:
	vector<shared_ptr<MqConsumer>> __consumers;
};

class MqProducerManager {
public: 
	MqConsumerManager &getInstance();
	void addProducer(shared_ptr<MqProducer> p);

private:
	vector<shared_ptr<MqProducer>> _producers;
};

class MqTopicManager {
public:	
	MqConsumerManager &getInstance();
	pair<shared_ptr<MqTopic>, shared_ptr<MqProducer> getTopic(string & topic);

	void addTopic(const string & topic, shared_ptr<MqTopic> t, shared_ptr<MqProducer> p);
private:
	unorderd_map<string topic, pair<shared_ptr<MqTopic>, shared_ptr<MqProducer>> __topics;
};


extern "C" unsigned int MQ_Publish(const string & topic, const string & message, void *param = nullptr);

