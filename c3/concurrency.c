//Yunfan Li, Zhicheng Fu, Xiang Li
//CS544-001
//Concurrency #3
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

struct ele {
	struct ele *next;
	int num;
};

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

struct ele *head;

int flag = 1;

void *thread_search(void *arg);

void *thread_insert(void *arg);

void *thread_delete(void *arg);

struct ele *get_end(void);

struct ele *get_end(void)
{
	struct ele *cur = head;
	
	while(cur->next != NULL) {
		cur = cur->next;
	}

	return cur;
}

void *thread_search(void *arg)
{
	int find_flag = 0;
	int fndNum = -1;
	
	pthread_detach(pthread_self());

	fndNum = rand()%20;			
	
	while(1) {
		find_flag = 0;
		
		if(flag) {			
			struct ele *cur = head;		
			
			sleep(5);
			
			while(cur != NULL) {
				if(cur->num == fndNum) {
					find_flag = 1;
					printf("Found the number %d\n", cur->num);
					break;
				}
				cur = cur->next;
			}

			if(find_flag == 0)
				printf("Not found the number %d\n", fndNum);
		}
		
	}
}

void *thread_insert(void *arg)
{
	pthread_detach(pthread_self());

	while(1) {
		int nodeNum = -1;
		struct ele *node;
		struct ele *end;
		
		sleep(2);

		nodeNum = rand()%20;

		pthread_mutex_lock(&mtx);

		node = (struct ele*)malloc(sizeof(struct ele));
		node->next = NULL;
		node->num = nodeNum;		
		end = get_end();		
		end->next = node;
		printf("Inserted %d\n", nodeNum);
		
		pthread_mutex_unlock(&mtx);
	}
}

void *thread_delete(void *arg)
{
	pthread_detach(pthread_self());

	while(1) {
		int delNum = -1;
		struct ele *cur;
		
		sleep(7);

		delNum = rand()%20;
		
		pthread_mutex_lock(&mtx);

		flag = 0;
		
		cur = head;

		while(cur->next != NULL) {
			if(cur->next->num == delNum) {
				struct ele *tmp = cur->next;
				printf("Deleted %d\n", delNum);
				cur->next = cur->next->next;
				free(tmp);
				break;
			}
			cur = cur->next;
		}
		
		pthread_mutex_unlock(&mtx);

		flag = 1;
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	head = (struct ele*)malloc(sizeof(struct ele));
	head->next = NULL;
	head->num = -999999;

	while(1) {
		int ca = -1;
		pthread_t ptid;
		
		printf("Please input (0, 1, 2 indicate Searcher, Inserter, Deleter): ");
		scanf("%d", &ca);

		switch(ca) {
		case 0:
			pthread_create(&ptid, NULL, thread_search, NULL);
			break;
		case 1:
			pthread_create(&ptid, NULL, thread_insert, NULL);
			break;
		case 2:
			pthread_create(&ptid, NULL, thread_delete, NULL);
			break;
		default:
			printf("Please input correct number!\n\n");
			break;
		}
	}
}
