//Yunfan Li, Zhicheng Fu, Xiang Li
//CS544-001
//Concurrency #4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <libgen.h>
#include <math.h>
#include <time.h>

#define N 100

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg);

void hello(void);

void world(void);

void hello(void) {
	printf("Hello, ");
}

void world(void) {
	printf("World!\n");
}

void *thread_func(void *arg)
{
	int in = *(int *)arg;
	
	pthread_detach(pthread_self());

	while(1) {
		sleep(2);
		
		pthread_mutex_lock(&mtx);

		printf("thread %d: ", in);
		hello();
		world();
				
		pthread_mutex_unlock(&mtx);
	}
}

int main(int argc, char **argv)
{
	int i = 0;
	pthread_t ptid;
	
	while(1) {
		if(i < N)
			pthread_create(&ptid, NULL, thread_func, &i);
		sleep(1);
		i++;
	}
}
