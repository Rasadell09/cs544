//Yunfan Li, Zhicheng Fu, Xiang Li
//CS544-001
//Concurrency #5
//Problem #2
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

int waiting = 0;
int chairs = 0;

sem_t customers;
sem_t barbers;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *barber_func(void *arg);

void *customer_func(void *arg);

void cut_hair(void);

void *barber_func(void *arg)
{
	pthread_detach(pthread_self());
	while(1) {
		sem_wait(&customers);

		pthread_mutex_lock(&mtx);

		waiting -= 1;
		sem_post(&barbers);

		pthread_mutex_unlock(&mtx);

		cut_hair();
	}
}

void *customer_func(void *arg)
{
	int id = *(int *)arg;

	pthread_detach(pthread_self());
	
	printf("%d's customer is in the shop.\n", id);
	
	pthread_mutex_lock(&mtx);
	
	if(waiting < chairs) {
		waiting += 1;
		
		sem_post(&customers);

		pthread_mutex_unlock(&mtx);
		
		sem_wait(&barbers);
	}
	else {
		printf("Too many customers, %d's customer left the shop.\n", id);

		pthread_mutex_unlock(&mtx);
	}

	pthread_exit(0);
}

void cut_hair(void)
{
	printf("Barber is cutting hair\n");
	sleep(5);
	printf("Barber done\n");
}

int main(int argc, char **argv)
{
	int b = 1;
	int i = 0;
	pthread_t cid;
	pthread_t bid;

	printf("How many chairs do you want: ");
	scanf("%d", &chairs);
	
	sem_init(&customers, 0, 0);
	sem_init(&barbers, 0, b);

	for(i = 0; i < b; i++) {
		pthread_create(&bid, NULL, barber_func, &i);

		sleep(1);
	}
	
	i = 0;
	while(1) {
		pthread_create(&cid, NULL, customer_func, &i);

		sleep(1);

		i++;
	}
}
