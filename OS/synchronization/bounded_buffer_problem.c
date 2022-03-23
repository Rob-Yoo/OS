#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define TRUE 1
#define BUFFER_SIZE 5
// BUFFER_SIZE를 공유
int buffer[BUFFER_SIZE];

// Binary Semaphore
pthread_mutex_t mutex;

// Counting Semaphore
sem_t empty;
sem_t full;

int in = 0;
int out = 0;

void	insert_item(int item)
{
	sem_wait(&empty); // buffer가 빌 때까지 기다린다.
	// 다 비어지면 동작
	pthread_mutex_lock(&mutex); // Producer가 여러명 일때의 상호배제 

	// Crictical Section
	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	printf("Producer: inserted $%d\n", item);

	pthread_mutex_unlock(&mutex);
	sem_post(&full);
}

void	remove_item(int *item)
{
	sem_wait(&full); // 버퍼가 찰 떄까지 기다린다.
	// 다 채워지면 동작
	pthread_mutex_lock(&mutex);

	// Crictical Section
	*item = buffer[out];
	out = (out + 1) % BUFFER_SIZE;
	printf("Consumer: removed $%d\n", *item);

	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
}

void	*producer(void *param)
{
	int	item;

	while (TRUE)
	{
		usleep((1 + rand() % 5) * 100000); // 0.1초 ~ 1.5초 슬립
		item = 1000 + rand() % 1000; // 1000 ~ 2000 랜덤
		insert_item(item); // Critical Section
	}
	pthread_exit(0);
}

void	*consumer(void *param)
{
	int	item;

	while (TRUE)
	{
		usleep((1 + rand() % 5) * 100000);
		remove_item(&item); // Critical Section
	}
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	int numOfProducers = 1;
	int numOfConsumers = 1;
	// int numOfProducers = BUFFER_SIZE;
	// int numOfConsumers = BUFFER_SIZE;
	pthread_t tid;
	int i;

	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	srand(time(0));

	for (i = 0; i < numOfProducers; i++)
		pthread_create(&tid, NULL, producer, NULL);
	for (i = 0; i < numOfConsumers; i++)
		pthread_create(&tid, NULL, consumer, NULL);

	sleep(5);
	return (0);
}