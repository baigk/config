
class KafkaConfig 
{
public:
	KafkaConfig(){}

	unsigned int set(std::shared_ptr<MqConfig> config){
		std::string errstr;

		if (config != nullptr){
			for (auto & i : *config) {
				_conf->set(i.first, i.second);
			}
		}
	}
	virtual ~KafkaConfig(){}

	std::shared_ptr<RdKafka::Conf> getKafkaConig() {return _conf;}

protected:
	std::shared_ptr<RdKafka::Conf> _conf;
};

class KafkaGlobalConfig : public kafkaConfig
{
public:
	KafkaGlobalConfig(std::shared_ptr<MqConfig> config) {
		_conf = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));

		set(config);
	}
};	
	
class KafkaProducerConfig : public KafkaGlobalConfig {
public:
	KafkaProducerConfig(std::shared_ptr<MqConfig> config) : KafkaGlobalConfig(config) {}
};

class KafkaConsumerConfig : public KafkaGlobalConfig {
public:
	KafkaConsumerConfig(std::shared_ptr<MqConfig> config) : KafkaGlobalConfig(config) {}
};

class KafkaTopicConfig : public KafkaConfig {
public:
	KafkaConsumerConfig(std::shared_ptr<MqConfig> config) {
		tconf_ = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));
		set(config);
	}
};

