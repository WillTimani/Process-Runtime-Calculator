#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc < 2) return 0;

	int fd[2];
	pid_t pid;

	pipe(fd);

	pid = fork();

	if(pid < 0){
		fprintf(stderr, "Fork Failed\n");
		return 1;
	} else if(pid == 0){
		struct timeval startTimeTemp;
		gettimeofday( &startTimeTemp, 0);
	
		close(fd[0]);
	
		write(fd[1], &startTimeTemp, sizeof(startTimeTemp));

		close(fd[1]);

		execvp(argv[1],argv + 1);

	} else {
		wait(NULL);

		struct timeval startTime;
		struct timeval end_time;
		struct timeval elapsed_time;

		close(fd[1]);

		read(fd[0], &startTime, sizeof(startTime));

		close(fd[0]);
		
		gettimeofday( &end_time, 0);

		timersub( &end_time, &startTime, &elapsed_time);

		printf("\nElapsed time: %ld.%06ld seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
	}
	return 0;
}

