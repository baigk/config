#include <sys/time.h>
#include <pthread.h>
#include "objects.h"

objectsPool::objectsPool(unsigned int num, void *(*pfn)(void *), void *args) {
	for (unsigned int i = 0; i < num; i++) {
		void *p = (* pfn)(args);
		__FreeList.push(p);

		printf("\r\n %#x", (unsigned int)(long)p);
	}

	pthread_mutex_init(&__mutex, nullptr);

	printf("\r\n create objectsPool %#x", (unsigned int)(long)this);
}

objectsPool::~objectsPool(){
	pthread_mutex_destroy(&__mutex);

	while(!__FreeList.empty()) {
		auto p = __FreeList.front();
		//delete p;
		__FreeList.pop();
	}
	printf("\r\n delete objectsPool %#x", (unsigned int)(long)this);
}

void * objectsPool::allocObject() {
	pthread_mutex_lock(&__mutex);
	if (__FreeList.empty()) {
		pthread_mutex_unlock(&__mutex);
		return nullptr;
	}

	void *p = __FreeList.front();
	__FreeList.pop();

	printf("\r\n ++++++%#x", (unsigned int)(long)p);

	pthread_mutex_unlock(&__mutex);

	return p;
}

void objectsPool::freeObject(void *obj) {

	pthread_mutex_lock(&__mutex);
	__FreeList.push(obj);
	pthread_mutex_unlock(&__mutex);

	return;
};

mutexObject::mutexObject() {
	pthread_mutex_init(&__mutex, nullptr);
	pthread_cond_init(&__cond, nullptr);
	printf("\r\n create mutextObject %#x", (unsigned int)(long)this);
}

mutexObject::~mutexObject() {
	pthread_mutex_destroy(&__mutex);
	pthread_cond_destroy(&__cond);
	printf("\r\n destry mutextObject %#x", (unsigned int)(long)this);
}

void mutexObject::wait(){
 	pthread_mutex_lock(&__mutex);
 	pthread_cond_wait(&__cond, &__mutex);
	pthread_mutex_unlock(&__mutex);
}

void mutexObject::wait(unsigned int msec){
	struct timespec abstime;
	
	auto  get_abstime_wait = [&](int microseconds) {
  		int absmsec;
  		struct timeval tv;
  		gettimeofday(&tv, NULL);
  		absmsec = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		printf("\r\n222 %u", abstime.tv_sec);
  		absmsec += microseconds;

  		abstime.tv_sec = absmsec / 1000;
		printf("\r\n333 %u", abstime.tv_sec);
  		abstime.tv_nsec = absmsec % 1000 * 1000000;
	};

 	pthread_mutex_lock(&__mutex);
	get_abstime_wait(msec);
	printf("\r\n111 %u", abstime.tv_sec);
 	pthread_cond_timedwait(&__cond, &__mutex, &abstime);
	pthread_mutex_unlock(&__mutex);
}

void mutexObject::notify(){
 	pthread_mutex_lock(&__mutex);
	pthread_cond_signal(&__cond);
	pthread_mutex_unlock(&__mutex);
}
