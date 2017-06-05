#include <pthread.h>
#include <queue>
#include <stdio.h>

using namespace std;

class objectsPool
{
	public:
		objectsPool(unsigned int num, void *(*pfn)(void *), void *args);
		~objectsPool();
		void * allocObject();
		void freeObject(void *obj);
	
	private:
		pthread_mutex_t __mutex;
		queue<void *> __FreeList;
};

class mutexObject
{
	public:
		mutexObject();
		~mutexObject();
		static void *createInstance(void *p){ return new mutexObject;}
		void wait();
		void wait(unsigned int msec);
		void notify();
	private:
		pthread_mutex_t __mutex;
		pthread_cond_t __cond;
};
