#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
	int fds[4] = {-1, -1, -1, -1};
	if(pipe(fds) != 0 or pipe(fds + 2) != 0){
		fprintf(stderr, "pipe error\n");
		return -1;
	}
	int fd_pwc = fds[1]; // parent write to child
	int fd_crp = fds[0]; // child read from parent
	int fd_cwp = fds[3]; // child write to parent
	int fd_prc = fds[2]; // parent read from child

	pid_t pid = fork();
	if(pid < 0){
		fprintf(stderr, "fork error\n");
		return -1;
	}
	else if(pid == 0){
		// child
		fprintf(stderr, "child process %d\n", getpid());

		char msg[] = "msg from child to parent";
		write(fd_cwp, msg, sizeof(msg));

		char buf[1024];
		read(fd_crp, buf, sizeof(buf));
		fprintf(stdout, "%s\n", buf);

		close(fd_crp);
		close(fd_pwc);
		close(fd_prc);
		close(fd_cwp);
		sleep(3);
		exit(0);
	}
	else{
		// parent
		fprintf(stderr, "parent process %d\n", getpid());

		char msg[] = "msg from parent to child";
		write(fd_pwc, msg, sizeof(msg));

		char buf[1024];
		read(fd_prc, buf, sizeof(buf));
		fprintf(stdout, "%s\n", buf);

		close(fd_crp);
		close(fd_pwc);
		close(fd_prc);
		close(fd_cwp);

		int status = -1;
		int option = 0;
		/* option:
		 * 	0: block
		 * 	WNOHANG: return 0 immediately if no child has exited
		 */
		int ret = waitpid(pid, &status, option);
		if(ret == -1){
			fprintf(stderr, "waitpid error\n");
		}
		else if(ret == 0){
			fprintf(stderr, "WNOHANG option used\n");
		}
		else{
			if(WIFEXITED(status)){
				int exitcode = WEXITSTATUS(status);
				fprintf(stderr, "process %d exited with code %d\n", pid, exitcode);
			}
			else if(WIFSIGNALED(status)){
				int bysignal = WTERMSIG(status);
				fprintf(stderr, "process %d exited by signal %d\n", pid, bysignal);
			}
			else{
				fprintf(stderr, "new case %d\n", ret);
			}
		}
	}

	fprintf(stderr, "in process %d\n", getpid());

	return 0;
}

