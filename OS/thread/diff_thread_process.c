#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

int process_value = 0;
int	thread_value = 0;
void	*runner(void *param);

int	main(int argc, char *argv[])
{
	pid_t pid;
	pthread_t tid;
	pthread_attr_t attr;

	pid = fork();
	if (pid == 0) // 자식 프로세스
	{
		process_value = 1;
		printf("child process value: %d\n", process_value);
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, runner, NULL);
		// 자식 프로세스의 메인 스레드 이외에 하나 더 쓰레드를 만들었다.
		pthread_join(tid, NULL); // 메인 쓰레드에서 새로 만든 쓰레드가 끝날 때까지 기다리는 함수
		printf("child thread value: %d\n", thread_value);
		// 아때, 같은 프로세스의 다른 쓰레드는 자원을 공유해서 thread_value가 5로 바뀐다.
	}
	else if (pid > 0) // 부모 프로세스
	{
		wait(NULL);
		printf("parent process value: %d\n", process_value);
		// 부모 프로세스와 자식 프로세스는 자원을 공유하지 않는다.
	}
}

void	*runner(void *param)
{
	thread_value = 5;
	pthread_exit(0);
}