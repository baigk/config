#pragma once

#include <string>
#include <vector>

using namespace std;

using MqConfigItem = pair<string, string>;
using MqConfig = vector<MqConfigItem>;

class Mq 
{
};

class MqConsumer : public Mq
{
public:
    unsigned int subscribe(const vector<string> &topics) {return 0;}
    unsigned int subscribe(const string &topic) {return 0;}
};

class MqProducer : public Mq
{
public:
    unsigned int createTopic(vector<string> &topics, const MqConfig & config = MqConfig()) {return 0;}
    unsigned int createTopic(const string & topic, const MqConfig & config = MqConfig()) {return 0;}
    unsigned int updateTopic(const string & topic, const MqConfig & config = MqConfig()) {return 0;}
    unsigned int deleteTopic(const string & topic);
    unsigned int publicMessage(const string & topic, const string & message, void *param = nullptr) {return 0;}
};
