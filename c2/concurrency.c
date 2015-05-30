//Yunfan Li, Zhicheng Fu, Xiang Li
//CS544-001
//Concurrency #2
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
#include <semaphore.h>

#define PHI_NUM 5

int wait_num = 0;
int wait_q[PHI_NUM] = {0};
sem_t forks[PHI_NUM];
sem_t max_phi;
int s[PHI_NUM] = {0};

void *thread_func(void *arg);

void *thread_func(void *arg)
{
	int in = *(int *)arg;
	int slp = 0;

	printf("Created %d philosopher\n", in);
	
	pthread_detach(pthread_self());

	while(1) {
		//think
		slp = rand()%20+1;
		printf("Philosopher %d is thinking for %d seconds...\n", in, slp);
		s[in] = 0;
		sleep(slp);

		//get_forks
		sem_wait(&max_phi);
	
		sem_wait(&forks[in]);
		sem_wait(&forks[(in-1+5)%5]);
		
		printf("Philosopher %d has gotten forks\n", in);

		//eat
		slp = rand()%8 + 2;
	
		printf("Philosopher %d is eating for %d seconds...\n", in, slp);
		s[in] = 1;
		sleep(slp);

		//put_forks
		sem_post(&forks[(in-1+5)%5]);
		sem_post(&forks[in]);
	
		sem_post(&max_phi);

		printf("Philosopher %d has put forks\n", in);
	}
}

int main(int argc, char **argv)
{
	int i = 0;
	int forkState[PHI_NUM] = {0};
	pthread_t ptid;

	srand(time(NULL));

	for (i = 0; i < PHI_NUM; i++)
		sem_init(&forks[i], 0, 1);

	sem_init(&max_phi, 0, 4);
	
	for (i = 0; i < PHI_NUM; i++) {
		pthread_create(&ptid, NULL, thread_func, &i);
		sleep(1);
	}

	while(1) {
		sleep(1);
		memset(&forkState, 0, PHI_NUM*sizeof(int));
		printf("The status of PHILOSOPERS:\n");
		for (i = 0; i < PHI_NUM; i++) { 
			if (s[i] == 1) {
				printf("Eating  \t");
				forkState[i] = 1;
				forkState[(i-1+5)%5] = 1;
			}
			else
				printf("Thinking\t");
		}
		printf("\nThe status of FORKS:\n\t");
		for (i = 0; i < PHI_NUM; i++) {
			if (forkState[i] == 1)
				printf("Using   \t");
			else
				printf("lying   \t");
		}
		printf("\n");
	}
}
