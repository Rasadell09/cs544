//Yunfan Li, Zhicheng Fu, Xiang Li
//CS544-001
//Concurrency #5
//Problem #1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <libgen.h>
#include <math.h>
#include <time.h>

int v = 1;
sem_t resource;

void *thread_func(void *arg);

void *thread_func(void *arg)
{
	int id = *(int *)arg;
	printf("%d has been created\n", id);
	pthread_detach(pthread_self());
	while(1) {
		int cn = 0;

		sem_getvalue(&resource, &cn);
		if((cn > 0) && (v == 1)) {
			if(sem_trywait(&resource) == -1)
				v = 0;
			else {
				sem_getvalue(&resource, &cn);
				printf("%d is using the resource\n", id);
				usleep(((rand()%1000)+2000)*1000);
				
				sem_post(&resource);
				
				sem_getvalue(&resource, &cn);
				if(cn == 3)
					v = 1;
			}
		}
	}
}

int main(int argc, char **argv)
{
	int i = 0;
	pthread_t ptid;

	srand(time(NULL));

	sem_init(&resource, 0, 3);
	
	while(1) {
		pthread_create(&ptid, NULL, thread_func, &i);

		sleep(2);
		
		i++;
	}
}
