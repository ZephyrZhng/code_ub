/* mshell.c */
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

void do_cd(char *argv[]);
void execute_new(char *argv[]);
void do_exit(char *argv[]);
void do_iredirect(char *argv[], int j);
void do_oredirect(char *argv[], int j);
void do_pipe(char *argv[], int j);

main()
{
	char *cmd=(void *)malloc(256*sizeof(char));
	char *cmd_arg[10];
	int cmdlen,i,j,tag;
	// j is length of cmd_arg

	do{
		/* 初始化cmd */
		for(i=0;i<255;i++) cmd[i]='\0';

		printf("-=Mini Shell=-*| ");
		fgets(cmd,256,stdin);

		cmdlen=strlen(cmd);
		cmdlen--;
		cmd[cmdlen]='\0';

		/* 把命令行分解为指针数组cmd_arg */
		for(i=0;i<10;i++) cmd_arg[i]=NULL;
		i=0; j=0; tag=0;
		while(i<cmdlen && j<10){
			if(cmd[i]==' '){
				cmd[i]='\0';
				tag=0;
			}else{
				if(tag==0)
					cmd_arg[j++]=cmd+i;
				tag=1;
			}
			i++;
		}

			/* 如果参数超过10个，就打印错误，并忽略当前输入 */
		if(j>=10 && i<cmdlen){
			printf("TOO MANY ARGUMENTS\n");
			continue;
		}

			/* 命令quit：退出Mini Shell */
		if(strcmp(cmd_arg[0],"quit")==0)
			break;

			/* 命令cd */
		if(strcmp(cmd_arg[0],"cd")==0){
			do_cd(cmd_arg);
			continue;
		}

		if(strcmp(cmd_arg[0], "exit")==0){
			do_exit(cmd_arg);
			continue;
		}

		// cmd_arg[j-2] = ">";
		// cmd_arg[j-1] = filename;
		if(strcmp(cmd_arg[j-2], ">")==0){
			do_oredirect(cmd_arg, j);
			continue;
		}

		if(strcmp(cmd_arg[j-2], "<")==0){
			do_iredirect(cmd_arg, j);
			continue;
		}

		int is_pipe = 0;
		for(i = 0; i < j; ++i){
			if(strcmp(cmd_arg[i], "|") == 0){
				is_pipe = 1;
				break;
			}
		}
		if(is_pipe){
			// printf("is pipe\n");
			// printf("%d\n", j);
			do_pipe(cmd_arg, j);
			continue;
		}

		/* 外部命令或应用程序 */
		execute_new(cmd_arg);
	}while(1);
}

/* 实现cd的功能 */
void do_cd(char *argv[])
{
	if(argv[1]!=NULL){
		if(chdir(argv[1])<0)
			switch(errno){
				case ENOENT:
				printf("DIRECTORY NOT FOUND\n");
				break;
				case ENOTDIR:
				printf("NOT A DIRECTORY NAME\n");
				break;
				case EACCES:
				printf("YOU DO NOT HAVE RIGHT TO ACCESS\n");
				break;
				default:
				printf("SOME ERROR HAPPENED IN CHDIR\n");
			}
	}
}
	
/* 执行外部命令或应用程序 */
void execute_new(char *argv[])
{
	pid_t pid;

	pid=fork();
	if(pid<0){
		printf("SOME ERROR HAPPENED IN FORK\n");
		exit(2);
	}else if(pid==0){
		if(execvp(argv[0],argv)<0)
			switch(errno){
				case ENOENT:
				printf("COMMAND OR FILENAME NOT FOUND\n");
				break;
				case EACCES:
				printf("YOU DO NOT HAVE RIGHT TO ACCESS\n");
				break;
				default:
				printf("SOME ERROR HAPPENED IN EXEC\n");
			}
		exit(3);
	}else
		wait(NULL);
}

void do_exit(char * argv[])
{
	exit(0);
}

void do_oredirect(char * argv[], int j)
{
	char *new_argv[10];
	int i, k;
	for(i=0;i<10;i++) new_argv[i]=NULL;
	for(i=0;i<j-2;++i) new_argv[i]=argv[i];

	int fid = open(argv[j-1], O_WRONLY|O_CREAT);

	int backup = dup(STDOUT_FILENO);
	int replaced = dup2(fid, STDOUT_FILENO);

	execute_new(new_argv);

	dup2(backup, STDOUT_FILENO);

	close(fid);
}

void do_iredirect(char * argv[], int j)
{	
	char *new_argv[10];
	int i, k;
	for(i=0;i<10;i++) new_argv[i]=NULL;
	for(i=0;i<j-2;++i) new_argv[i]=argv[i];

	int fid = open(argv[j-1], O_RDONLY);

	int backup = dup(STDIN_FILENO);
	int replaced = dup2(fid, STDIN_FILENO);

	execute_new(new_argv);

	dup2(backup, STDIN_FILENO);

	close(fid);
}

void do_pipe(char * argv[], int j)
{
	char *largv[10];
	char *rargv[10];
	int i, k, llen, rlen;

	llen = rlen = 0;

	for(i = 0; i < 10; ++i) largv[i] =NULL, rargv[i] = NULL;

	for(i = 0; i < j; ++i){
		// printf("%s\n", argv[i]);
		if(strcmp(argv[i], "|") != 0){
			largv[i] = argv[i];
			++llen;
		}else{
			break;
		}
	}

	++i;

	for(k = 0; i < j; ++i, ++k){
		// printf("%s\n", argv[i]);
		rargv[k] = argv[i];
		++rlen;
	}

	// printf("l: %d\tr: %d\n", llen, rlen);
	// for(i = 0; i < llen; ++i){
	// 	printf("%s ", largv[i]);
	// }
	// printf("\n");

	// for(i = 0; i < rlen; ++i){
	// 	printf("%s ", rargv[i]);
	// }
	// printf("\n");

	// parent pipe
	// child dup redirect

	int fd[2]; /*fd[0]是管道读取,fd[1]是管道的输入*/
	pid_t childpid;
	char buf[512];  
	int stat;
	int backup_in = dup(STDIN_FILENO);
	int backup_out = dup(STDOUT_FILENO);

	pipe(fd);

	childpid = fork();

	if(childpid == 0) { /*子进程*/
		// printf("lchild process executed!!!!!!!!!!!!!!!!!\n");
		close(fd[0]);

		int lchild_replaced = dup2(fd[1], STDOUT_FILENO);

		execute_new(largv);
		dup2(backup_out, STDOUT_FILENO);

		close(fd[1]); /* Child process closes up input side of pipe */
		exit(0);

	} else {
		wait(&stat);
		// printf("rchild process executed!!!!!!!!!!!!!!!!!\n");
		close(fd[1]);

		int rchild_replaced = dup2(fd[0], STDIN_FILENO);

		execute_new(rargv);
		dup2(backup_in, STDIN_FILENO);

		close(fd[0]); /* Child process closes up input side of pipe */
	}
}