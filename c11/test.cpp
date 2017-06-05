#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

class test
{
	public:
		void run() {
			while(1) {
				static int i = 0;
				printf("\r\n ++ %u", i++);
				sleep(1);
			}
		}
		void start() {
			auto route = [](void *para)->void *{
				test * t = (test *)para;
				t->i = 1;
				t->j = 2;

				t->run();
			};

			pthread_t tid;
			pthread_create(&tid, NULL, route, this);
		}

	public:
		int i;
		int j;
};
int main() {
	test t;

	t.start();

	sleep(5);
	printf("\r\n %u %u", t.i, t.j);

	while(1) {
		sleep(100);
	}
}
