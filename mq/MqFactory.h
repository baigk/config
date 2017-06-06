#include "Mq.h"
#include "Sington.h"

using CreateConsumerFunc = (MqConsumer *)(*)(MqConfig *);
using CreateProducerFunc = (MqProducer *)(*)(MqConfig *);

typedef struct tagMqInfo {
	CreateConsumerFunc createConsumerFunc;
	CreateProducerFunc createProducerFunc;
}MqInfo;

class MqFactory
{
	DECLARE_SINGTON(MqFactory);
public:
	MqFactory();
	virtual ~MqFactory();

	std::shared_ptr<MqConsumer> createConsumer(std::string & type, std::vertor<MqConfigItem> * config> = nullptr);

	std::shared_ptr<MqProducer> createProducer(std::string & type, std::vertor<MqConfigItem> * config> = nullptr);

	unsigned int registerMqInfo(const std::string & type, const MqInfo & mqInfo) ;

private:
	std::map<std::string, MqInfo> __mqInfo;
}
