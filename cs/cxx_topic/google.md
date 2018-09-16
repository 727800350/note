# glog
- `LOG(INFO), LOG(WARNING), LOG(ERROR), LOG(FATAL)`: info 是0, warning 是1, fatal 级别会出core
- `--minloglevel=0`: 打印级别大于0的日志, 也就是INFO 及以上都会打印
- `--v=0`: 默认不打印vlog, eg: `vlog(100) << "vlog"` 则需要 --v=100 才能打印出来, 还可以配合`--vmodule` 进行文件级别的控制

# gflags
- `DEFINE_bool`: boolean
- `DEFINE_int32`: 32-bit integer
- `DEFINE_int64`: 64-bit integer
- `DEFINE_uint64`: unsigned 64-bit integer
- `DEFINE_double`: double
- `DEFINE_string`: C++ string

# gtest

# gperftools
```C++
#include <gperftools/profiler.h>

void func1(){
	int i = 0;
	while(i < 100000){
		++i;
	}
}

void func2() {
	int i = 0;
	while(i < 200000){
		++i;
	}
}

int main(){
	ProfilerStart("my.prof");
	for(int i = 0; i < 1000; ++i){
		func1();
		func2();
	}
	ProfilerStop();
	return 0;
}

g++ -std=c++11 -g -Wall test.cpp -lprofiler
google-pprof --text ./a.out my.prof // 二进制的命令老版本为 pprof
google-pprof --pdf ./a.out my.prof >output.pdf
```

