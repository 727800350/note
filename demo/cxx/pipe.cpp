#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int fds[2];
	char buf[80];
	pid_t pid;
	
	pipe(fds);
	pid = fork();
	if(pid > 0){
		fprintf(stderr, "This is in the father process, here write a string to the pipe.\n");
		char s[] = "Hello world, this is write by pipe.\n";
		write(fds[1], s, sizeof(s));
		close(fds[0]);
		close(fds[1]);
	}
	else if(pid == 0){
		printf("This is in the child process, here read a string from the pipe.\n");
		read(fds[0], buf, sizeof(buf));
		fprintf(stderr, "%s\n", buf);
		close(fds[0]);
		close(fds[1]);
	}
	
	waitpid(pid, NULL, 0);
	
	return 0;
}

