#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

const int max_kl = 1024;
const int max_vl = 10 * 1024 * 1024;

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);

	google::SetVersionString("1.0.0.0");
	google::SetUsageMessage(std::string("usage: cat - | ") + argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);

	int fds[] = {-1, -1};
	if(pipe(fds) != 0){
		LOG(ERROR) << "pipe error";
		return -1;
	}

	pid_t pid = fork();
	if(pid < 0){
		LOG(ERROR) << "fork error";
		return -1;
	}
	else if(pid == 0){
		// child
		LOG(INFO) << "child pid " << getpid() << " starts";

		dup2(fds[1], STDOUT_FILENO);
		close(fds[0]);

		/*
		 * 系统调用exec是用来执行一个可执行文件来代替当前进程的执行图像.
		 * 需要注意的是, 该调用并没有生成新的进程, 而是在原有进程的基础上, 替换原有进程的正文.
		 * 调用前后是同一个进程, 进程号PID不变. 但执行的程序变了(执行的指令序列改变了).
		 * execl 成功后就不会返回, 因为后面的代码就不会执行.
		 * execl 出错后会返回, 后面的代码才可能执行到.
		 * 一定要要在fork 出的子进程中调用! pthread_create创建的子线程与父线程共用同一代码段.
		 *
		 */
		execl("/bin/sh", "sh", "-c", "ls -l | head -n 5", NULL);
		exit(-1);
	}
	else{
		// parent
		LOG(INFO) << "parent pid " << getpid() << " starts";

		char buffer[1024] = {'\0'};
		read(fds[0], buffer, sizeof(buffer) - 1);
		fprintf(stdout, "%s", buffer);

		int status = -1;
		int option = 0;
		/* option:
		 * 	0: block
		 * 	WNOHANG: return 0 immediately if no child has exited
		 */
		int ret = waitpid(pid, &status, option);
		if(ret == -1){
			LOG(INFO) << "waitpid error";
		}
		else if(ret == 0){
			LOG(INFO) << "WNOHANG option used";
		}
		else{
			if(WIFEXITED(status)){
				int exitcode = WEXITSTATUS(status);
				LOG(INFO) << "child " << pid << " exited with code " << exitcode;
			}
			else if(WIFSIGNALED(status)){
				int bysignal = WTERMSIG(status);
				LOG(INFO) << "child " << pid << " exited by signal " << bysignal;
			}
			else{
				LOG(INFO) << "new case " << ret;
			}
		}

	}

	LOG(INFO) << "in process " << getpid();

	google::ShutdownGoogleLogging();
	return 0;
}

