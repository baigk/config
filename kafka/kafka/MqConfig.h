#ifndef __MQ_CONFIG__H
#define __MQ_CONFIG__H

class MqConfig
{
	public:
		std::map<std::string, std::string> & getConfig() {return __config;}
		void update(std::string key, std::string val){};
	private:
		std::map<std::string, std::string> __config;
};


#endif
