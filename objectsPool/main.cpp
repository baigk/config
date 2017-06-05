#include "objects.h"
#include <pthread.h>
#include <unistd.h>

mutexObject *mutex;
objectsPool *p;

void * test1(void *) {
	unsigned int i = 0;

	mutex = (mutexObject *)p->allocObject();
	printf("\r\n before waitiiiii %#x", (unsigned int)(long)mutex);
	while(1) {
		printf("\r\n before wait");
		mutex->wait(10000);
		printf("\r\n end wait");
	}

}

void * test2(void *) {
	sleep(500);
	printf("\r\n before waitiiiii11111 %#x", (unsigned int)(long)mutex);
	while(1) {
		printf("\r\n before notify");
		mutex->notify();
		printf("\r\n end notify");
		sleep(5);
	}
}

int main() {
	p = new objectsPool(10, mutexObject::createInstance, nullptr);

	pthread_t tid1, tid2;

	pthread_create(&tid1, nullptr, test1, nullptr);
	pthread_create(&tid2, nullptr, test2, nullptr);

	printf("\r\n tid: %#x, %#x", (unsigned int)tid1, (unsigned int)tid2);
	
	pthread_join(tid1, nullptr);
	pthread_join(tid2, nullptr);

	delete p;

	while(1) {
		sleep(1);
	}
}
