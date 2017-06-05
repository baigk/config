#include <pthread.h>
using namespace std;

class objectsPool
{
	public:
		objectsPool(unsigned int num, void *(*pfn)(void *), void *args);
		void * allocObject();
		void freeObject();
	
	private:
		pthread_mutex_t __mutex;
		queue<void *> __FreeList;
};

class mutexObject
{
	public:
		mutexObject();
		void wait();
		void notify();
	private:
		pthread_mutex_t __mutex;
		pthread_cont_t __cond;
};
