#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

	if(argc < 2) return 0;

	pid_t pid;
	int fd;
	const char *name = "OS";
	struct timeval *startTime;
	int SIZE = sizeof(startTime);

	fd = shm_open(name,O_CREAT | O_RDWR,0666);
	ftruncate(fd,SIZE);
	startTime = (struct timeval *)
	mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	pid = fork();

	if(pid < 0){
		fprintf(stderr, "Fork Failed\n");
		return 1;
	} else if (pid == 0){
		gettimeofday( startTime, 0);

		execvp(argv[1],argv+1);
	} else {
		wait(NULL);

		struct timeval end_time;
		struct timeval elapsed_time;

		gettimeofday( &end_time, 0);

		timersub( &end_time, startTime, &elapsed_time);
	
		printf("\nElapsed time: %ld.%06ld seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
	}
	shm_unlink(name);
	return 0;
}



