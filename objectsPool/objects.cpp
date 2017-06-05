#include <pthread.h>
#include "objects.h"

objectsPool::objectsPool(unsigned int num, void *(*pfn)(void *), void *args) {
	for (unsigned int i = 0; i < num; i++) {
		void *p = (* pfn)(args);
		__FreeList.push_back(p);
	}
	__mutex = PTHREAD_MUTEX_INITIALIZER;
}

void * objectsPool::allocObject() {
	pthread_mutex_lock(__mutex);
	if (__FreeList.empty()) {
		pthread_mutex_unlock(__mutex);
		return nullptr;
	}

	*p = __FreeList.frond();
	__FreeList.pop();

	pthread_mutex_unlock(__mutex);

	return p;
}

void objectsPool::freeObject(void *obj) {
	pthread_mutex_lock(__mutex);
	__FreeList(obj);
	pthread_mutex_unlock(__mutex);

	return;
};

mutextObject::mutexObject() {
	__mutex = PTHREAD_MUTEX_INITIALIZER;
	__cond = PTHREAD_CONF_INITIALIZER;
}
void wait(){
	struct timespec abstime;
	
	auto  get_abstime_wait = [&](int microseconds) {
  		int absmsec;
  		struct timeval tv;
  		gettimeofday(&tv, NULL);
  		absmsec = tv.tv_sec * 1000 + tv.tv_usec / 1000;
  		absmsec += microseconds;

  		abstime->tv_sec = absmsec / 1000;
  		abstime->tv_nsec = absmsec % 1000 * 1000000;
	}

 	pthread_mutex_lock(&__mutex);
	get_abstime_wait(5000);
 	pthread_cond_timedwait(&__cond, &__mutex, &abstime);
	pthread_mutex_unlock(&__mutex);
}

void notify(){
 	pthread_mutex_lock(&__mutex);
	pthread_conf_signal(&__conf);
	pthread_mutex_unlock(&__mutex);
}
