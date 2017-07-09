#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

using namespace std;

using MqConfigItem = pair<string, string>;
using MqConfig     = vector<MqConfigItem>;

class Mq 
{
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

class MqConsumer : public Mq, public enable_shared_from_this<MqConsumer> {
public:
	MqConsumer();
	MqConsumer(MqConfig & config);
    unsigned int subscribe(const vector<string> &topics);
    unsigned int subscribe(const string &topic) ;

	virtual unsigned int subscribeEx(const string &topic) {return 0;}

	shared_ptr<MqConsumer> getptr();
protected:
	unordered_map<string, string> _topics;
	MqConfig _config;
};

class MqProducer : public Mq, public enable_shared_from_this<MqProducer>
{
public:
	MqProducer();
	MqProducer(const MqConfig & config);
	
    shared_ptr<MqProducer> getptr();
	
    virtual unsigned int publishMessage(shared_ptr<MqTopic> t, const string & message, void *param = nullptr) {return 0;}

	MqProducer & createTopic(const string & topic);

	virtual shared_ptr<MqTopic> createTopicEx(const string & topic);
protected:
	MqConfig _config;

};


class MqConsumerManager {
public: 
	static MqConsumerManager &getInstance();
	void addConsumer(shared_ptr<MqConsumer> c);

private:
	vector<shared_ptr<MqConsumer>> __consumers;
};

class MqProducerManager {
public: 
    static MqProducerManager &getInstance();
	void addProducer(shared_ptr<MqProducer> p);

private:
	vector<shared_ptr<MqProducer>> _producers;
};

class MqTopicManager {
public:	
	static MqTopicManager &getInstance();
	shared_ptr<MqTopic> getTopic(const string & topic);
	shared_ptr<MqProducer> getProducer(const string & topic);

	void addTopic(const string & topic, shared_ptr<MqTopic> t, shared_ptr<MqProducer> p);
private:
	//unordered_map<string, pair<shared_ptr<MqTopic>, shared_ptr<MqProducer> > __topics;
	unordered_map<string, shared_ptr<MqTopic>> __topics;
	unordered_map<string, shared_ptr<MqProducer>> __producers;
};


extern "C" unsigned int MQ_Publish(const string & topic, const string & message, void *param = nullptr);

