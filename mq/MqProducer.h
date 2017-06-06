#ifndef __MQ_PRODUCER__H
#define __MQ_PRODUCER_H

class MqProducer : public Mq
{
public:
	unsigned int CreateTopic(std::vector<std::string> &topics, std::vertor<MqConfigItem> * config> = nullptr) {return 0;}
	unsigned int CreateTopic(const std::string & topic, std::vertor<MqConfigItem> * config> = nullptr) {return 0;}
	unsigned int UpdateTopic(const std::string & topic, std::vertor<MqConfigItem> * config> = nullptr) {return 0;}
	unsigned int DeleteTopic(const std::string & topic);
	unsigned int publicMessage(const std::string & topic, const std::string & message, void *param = nullptr) {return 0;}
};
