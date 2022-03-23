#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

int sum = 0;

sem_t sem;

void	*counter(void *param)
{
	for (int k = 0; k < 10000; k++)
	{
		// Entry Section
		sem_wait(&sem);

		//Critical Section
		sum++;

		//Exit Section
		sem_post(&sem);

		//Remainder Section
	}
	pthread_exit(0);
}

int main()
{
	pthread_t tid1, tid2;

	sem_init(&sem, 0, 1); // S = 1 -> Binary Semaphore == Mutex Lock
	pthread_create(&tid1, NULL, counter, NULL);
	pthread_create(&tid2, NULL, counter, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("sum : %d\n", sum);
}