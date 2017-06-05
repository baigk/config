#ifndef __MQ_CONFIG__H
#define __MQ_CONFIG__H

class MqConfig
{
	public:
		std::map<std::string, std::string> & getConfig() {return __config;}
	private:
		std::map<std::string, std::string> __config;
};


#endif
