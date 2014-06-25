#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

void *foo(void *)
{
	for(;;)
	{
		sleep(10);
	}
	
	return(nullptr);
}

int main()
{
		int i = 0;
		pthread_t thread;

		while (1) {
			if (pthread_create(&thread, NULL, foo, NULL) != 0)
				return(i);
			i ++;
			printf("i = %d\n", i);
			}			
}
