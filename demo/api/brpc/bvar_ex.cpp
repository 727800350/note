/*************************************************************************
	> File Name: bvar_ex.cpp
	> Author: Eric Wang
	> Email: wangchaogo1990 at gmail.com
	> Created Time: 2017-12-30 05:28:13
 ************************************************************************/

#include <stdio.h>
#include <gtest/gtest.h>
#include <bvar/bvar.h>

// clang++ -lpthread -lbrpc -lgflags -lgtest

TEST(bvarTest, Test_adder){
	bvar::Adder<int> num;
	num << 1;
	ASSERT_TRUE(num.get_value() == 1);
	num << 2 << 3;
	ASSERT_TRUE(num.get_value() == 6);

	bvar::Window<bvar::Adder<int> > num_minute("ex", "num", &num, 60);
}

TEST(bvarTest, Test_max){
	bvar::Maxer<int> max;
	max << 1 << 2 << 3 << -4;
	ASSERT_TRUE(max.get_value() == 3);
}

TEST(bvarTest, Test_min){
	bvar::Miner<int> min;
	min << 1 << 2 << 3 << -4;
	ASSERT_TRUE(min.get_value() == -4);
}

TEST(bvarTest, Test_mean){
	bvar::IntRecorder mean;
	mean << 1 << 3 << 5;
	ASSERT_TRUE(mean.average() == 3);
}

TEST(bvarTest, Test_latency){
	bvar::LatencyRecorder latency("write latency");
	sleep(1);
	latency << 3;
	sleep(1);
	latency << 5;
	sleep(1);
	latency << 6;
	LOG(INFO) << "count:" << latency.count() << " qps:" << latency.qps();
}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

