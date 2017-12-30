/*************************************************************************
	> File Name: bvar_ex.cpp
	> Author: Eric Wang
	> Email: wangchaogo1990 at gmail.com
	> Created Time: 2017-12-30 05:28:13
 ************************************************************************/

#include <stdio.h>
#include <gtest/gtest.h>
#include <gflags/gflags.h>
#include <butil/logging.h>
#include <butil/time.h>

// clang++ -lpthread -lbrpc -lgflags -lgtest

TEST(butilTest, Test_time){
	butil::Timer time;
	time.start();
	sleep(1);
	time.stop();
	LOG(INFO) << time.s_elapsed() << "s " << time.m_elapsed() << "ms " << time.u_elapsed() << "us " << time.n_elapsed() << "ns";
}

int main(int argc, char* argv[]){
	google::SetVersionString("1.0.0.0");
	google::SetUsageMessage(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

