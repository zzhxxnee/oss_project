#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

#define GB2BYTE(num) (num*1024*1024*1024)

inline long get_timediff(struct timespec *start, struct timespec *end)
{
	return (end->tv_nsec - start->tv_nsec) 
			+ ((end->tv_sec - start->tv_sec) * 1E9);
}


int main(int argc, char **argv)
{
	pid_t pid;
	unsigned long i;
	unsigned long data_size_GB = 0;
	unsigned long data_size = 0;
	unsigned long *data;
	struct timespec start_time, end_time;

	if (argc < 3) {
		printf("./a.out <data_size_in_GB> <vfork>\n");
		return 0;
	}
	data_size_GB = (unsigned long)atoi(argv[1]);
	data_size = GB2BYTE(data_size_GB);
	printf("data_size = %lu GB, %lu Bytes\n", data_size_GB, data_size);

	data = (unsigned long*)malloc(data_size);
	if (!data) {
		printf("memory allocation fail\n");
	} else {
		printf("memory allocation success\n");
	}

#if 1
	for (i = 0; i < (data_size >> 3); i++) {
		data[i] = i;
	}
#endif

	clock_gettime(CLOCK_MONOTONIC, &start_time);

	if (atoi(argv[2]) == 1) { // vfork
		pid = vfork();
		if (pid < 0) {
			printf("failed to create a new process\n");
		} else if (pid == 0) {
			//printf("I'm child pid=%d\n", getpid());
			exit(0);
		} else if (pid > 0) { // after the child terminates
			clock_gettime(CLOCK_MONOTONIC, &end_time);
			printf("Elased Time for vfork() with %luGB data: %lu ns\n", data_size_GB, get_timediff(&start_time, &end_time));
			//printf("I'm parent pid=%d\n", getpid());
		}
	} else { // fork
		pid = fork();
		if (pid < 0) {
			printf("failed to create a new process\n");
		} else if (pid == 0) {
			//printf("I'm child pid=%d\n", getpid());
		} else if (pid > 0) {
			clock_gettime(CLOCK_MONOTONIC, &end_time);
			printf("Elased Time for fork() with %luGB data: %lu ns\n", data_size_GB, get_timediff(&start_time, &end_time));
			//printf("I'm parent pid=%d\n", getpid());
		}
	}

	


}
