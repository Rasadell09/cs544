//Yunfan Li, Zhicheng Fu
//CS544-001
//Homework #1
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
#include "mt19937ar.h"

struct item {
	int prt;
	int slp;
};

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int buf_num = 0;
struct item buf[32];

int asm_random(int begin, int end, int flag);

void *thread_func(void *arg);

int asm_random(int begin, int end, int flag)
{
	int result = 0;
	unsigned char ok;
	uint32_t rd;
	
	if (flag) {
		asm volatile ("rdrand %0; setc %1"
		              : "=r"(rd), "=qm"(ok)		              
		              );

		if (1 == ok) {
			result = abs((int)rd);
		}
		else {
			perror("Rdrand failed!\n");
			exit(1);
		}
	}
	else {
		result = abs((int)genrand_int32());
	}
	
	if (begin != end) {
		result = begin + ( result%(end-begin+1) );
	}
	
	return result;
}

void *thread_func(void *arg)
{
	int c_num1 = 0;
	int c_num2 = 0;
	int i = 0;
	
	pthread_detach(pthread_self());

	while(1) {

		while(0 == buf_num) {
			continue;
		}

		pthread_mutex_lock(&mtx);
		
		c_num1 = buf[0].prt;
		c_num2 = buf[0].slp;

		for(i = 0; i < (buf_num-1); i++) {
			buf[i].prt = buf[i+1].prt;
			buf[i].slp = buf[i+1].slp;
		}

		buf_num-=1;

		pthread_mutex_unlock(&mtx);
		
		sleep(c_num2);

		printf("%d\n", c_num1);
	}
}

int main(int argc, char **argv)
{
	int p_num1 = 0;
	int p_num2 = 0;
	int p_sleep_num = 0;

	pthread_t ptid;

	int flag = 0;
	unsigned int eax = 1;
	unsigned int ebx = 0;
	unsigned int ecx = 0;
	unsigned int edx = 0;
	
	asm volatile("cpuid"
	             : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	             : "a"(eax)	             
	             );

	if (ecx & 0x40000000) {
		flag = 1;
		printf("This machine supports RDRAND instruction.\n");
	}
	else {
		flag = 0;
		printf("This machine doesn't support RDRAND instruction.\n");
	}

	init_genrand(time(0));
	
	pthread_create(&ptid, NULL, thread_func, NULL);

	while(1) {
		p_num1 = asm_random(0, 0, flag);
		p_num2 = asm_random(2, 9, flag);

		while(32 == buf_num) {
			continue;
		}

		pthread_mutex_lock(&mtx);

		buf[buf_num].prt = p_num1;
		buf[buf_num].slp = p_num2;

		buf_num+=1;
		
		pthread_mutex_unlock(&mtx);

		p_sleep_num = asm_random(3, 7, flag);

		sleep(p_sleep_num);
	}
}
