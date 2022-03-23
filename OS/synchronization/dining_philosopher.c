#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define TRUE 1
#define NUM_PHILS 5

enum {THINKING, SLEEPING, EATING} state[NUM_PHILS];

pthread_mutex_t mutex_lock;
pthread_cond_t cond_vars[NUM_PHILS];

int leftOf(int i)
{
	return (i + NUM_PHILS - 1) % NUM_PHILS;
}

int rightOf(int i)
{
	return (i + 1) % NUM_PHILS;
}

void	init()
{
	int i;
	
	for (i = 0; i < NUM_PHILS; i++)
	{
		state[i] = THINKING;
		pthread_cond_init(&cond_vars[i], NULL);
	}
	pthread_mutex_init(&mutex_lock, NULL);
	srand(time(0));
}

void	test(int i)
{
	if (state[i] == SLEEPING && state[leftOf(i)] != EATING \
	&& state[rightOf(i)] != EATING)
	{
		state[i] = EATING;
		pthread_cond_signal(&cond_vars[i]); // 먹을 때까지 기다려
	}
}

void	pickup(int i)
{
	pthread_mutex_lock(&mutex_lock);

	state[i] = SLEEPING;
	test(i);
	while (state[i] != EATING)
		pthread_cond_wait(&cond_vars[i], &mutex_lock);
	pthread_mutex_unlock(&mutex_lock);
}

void	putdown(int i)
{
	pthread_mutex_lock(&mutex_lock);

	state[i] = THINKING;
	test(leftOf(i));
	test(rightOf(i));

	pthread_mutex_unlock(&mutex_lock);
}

void	think(int id)
{
	printf("%d: Now, I'm thinking...\n", id);
	usleep((1 + rand() % 50) * 10000);
}

void	eat(int id)
{
	printf("%d: Now, I'm eating...\n", id);
	usleep((1 + rand() % 50) * 10000);
}

void	*philosopher(void *param) 
{
	int id;

	id = *((int *)param);
	while (TRUE)
	{
		think(id);
		pickup(id);
		eat(id);
		putdown(id);
	}
}

int main()
{
	int i;
	pthread_t tid;

	init();
	for (i = 0; i < NUM_PHILS; i++)
		pthread_create(&tid, NULL, philosopher, (void *)&i);
	for (i = 0; i < NUM_PHILS; i++)
		pthread_join(tid, NULL);
	return (0);
}