#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

int sum = 0;

int turn; // 어떤 쓰레드를 실행시킬껀지 가르키는 변수
// turn을 굳이 하나 더 쓰는 이유는 flag가 둘 다 true여서 C.S 직전까지 둘 다 오게되어도 turn 값을 최종적으로 어떤 프로세스에서 바꾸냐에 따라 진입될 쓰레드가 달라진다.
int flag[2]; // i번째 쓰레드가 임계영역을 사용하겠다고 알리는 변수 -> 상호배제를 위한 변수

void	*producer(void *param)
{
	for (int k = 0; k < 10; k++)
	{
		// Entry Section
		flag[0] = TRUE;
		turn = 1; // 다음 차례는 consumer 쓰레드임을 명시
		while (flag[1] && turn == 1)
			;
		
		// Critical Section
		sum++;
		printf("pro sum = %d\n", sum);
		// Exit Section
		flag[0] = FALSE;

		// Remainder Section
	}
	pthread_exit(0);
}

void	*consumer(void *param)
{
	for (int k = 0; k < 10; k++)
	{
		// Entry Section
		flag[1] = TRUE;
		turn = 0; // 다음 차례는 producer 쓰레드임을 명시
		while (flag[0] && turn == 0)
			;
		
		// Critical Section
		sum--;
		printf("con sum = %d\n", sum);

		// Exit Section
		flag[1] = FALSE;

		// Remainder Section
	}
	pthread_exit(0);
}

int main()
{
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, producer, NULL);
	pthread_create(&tid2, NULL, consumer, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("sum = %d\n", sum);
}