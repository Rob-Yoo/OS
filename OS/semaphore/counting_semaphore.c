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
	pthread_t tid[5];

	sem_init(&sem, 0, 5); // S = 5 -> Race Condition 발생!! 왜냐하면, 5개의 쓰레드 모두 각각의 semaphore들을 하나씩 들고 Critical Section에 진입할 수 있기 때문에
	// sem_init(&sem, 0, 1); 따라서 Binary semaphore을 씀으로써 1개의 쓰레드만 1개의 semaphore를 들고 진입하고 나머지 쓰레드들은 waiting que에서 대기하고 있으면 해결된다.
	for (int i = 0; i < 5; i++)
		pthread_create(&tid[i], NULL, counter, NULL);
	for (int i = 0; i < 5; i++)
		pthread_join(tid[i], NULL);
	printf("sum : %d\n", sum);
}