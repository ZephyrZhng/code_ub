#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
	pid_t pc1, pc2, pr1, pr2;
	int fd[2];
	char buf1[50], buf2[50], s[50];
	pipe(fd);
	while((pc1 = fork()) == -1);
	if(pc1 < 0){
		printf("create child process1 error: %s\n", strerror(errno));
		exit(1);
	}else if(pc1 == 0){
		lockf(fd[1], 1, 0);
		sprintf(buf1, "child process1 %d is sending a msg", getpid());
		write(fd[1], buf1, 50);
		printf("in process1 %d, sending a msg\n", getpid());
		lockf(fd[1], 0, 0);
		exit(0);
	}
	while((pc2 = fork()) == -1);
	if(pc2 < 0){
		printf("create child process2 error: %s\n", strerror(errno));
		exit(1);
	}else if(pc2 == 0){
		lockf(fd[1], 1, 0);
		sprintf(buf2, "child process2 %d is sending a msg", getpid());
		write(fd[1], buf2, 50);
		printf("in process2 %d, sending a msg\n", getpid());
		lockf(fd[1], 0, 0);
		exit(0);
	}
	printf("in parent process %d\n", getpid());
	pr1 = wait(0);
	if(pr1 > 0){
		read(fd[0], s, 50);
		printf("parent process %d received a msg: %s\n", getpid(), s);;
	}else{
		printf("error: %s\n", strerror(errno));
	}
	pr2 = wait(0);
	if(pr2 > 0){
		read(fd[0], s, 50);
		printf("parent process %d received a msg: %s\n", getpid(), s);
	}else{
		printf("error: %s\n", strerror(errno));
	}
	return 0;
}