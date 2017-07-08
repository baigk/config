#include <string>
#include <string.h>
#include <functional>
#include <thread>
#include <time.h>
#include <iostream>
#include <fstream>
//#include <sstream>

#include "producer.h"


#define		SIZE_LEN		4
#define		PROTO_SIZE		2048*10
#if 0
void func(uKafkaProducer p, const string& topic, const string& file)
{
	int partition = 0;
	int timeout = 0;
	
	fstream input(file, fstream::in | fstream::binary);

	printf("dasdasda");
	while (!input.eof())
	{
		int size = 0;
		char proto[PROTO_SIZE] = {0};
		char buf[PROTO_SIZE] = {0};

		input.read((char*)&size, SIZE_LEN);
		if (size == 0)
		{
			break;
		}
		input.read(proto, size);

		memcpy(buf, (char*)&size, 4);
		memcpy(buf + 4, proto, size);

		printf("dasdasda1111111");
		p.send(buf, size + 4, topic, partition, timeout);
	}

	input.close();
}
#endif

int main(int argc, char* args[])
{
	string msg1("message1");
	//string msg2("message2");
	string name1("q");
	//string name2("h");
	string host("localhost");
	int port = 9092;
	string file;//("./tc.28127.2015-12-16-12-00");

	if(argc > 1)
	{
		name1 = args[1];
		file = args[2];

		uKafkaProducer p;
		p.init(host, port);
		p.create_topic(name1);
		//p.create_topic(name2);

		p.send("test", 5, name1, 0, 0);
		
		//thread t1(func, p, name1, file);
		//thread t2(func, p, name2, msg2);

		//t1.join();
		//t2.join();

		p.close();
	}

	return 0;
}
