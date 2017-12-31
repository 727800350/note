#include <stdio.h>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <gtest/gtest.h>

// -ljsoncpp -lgtest -lglog -lgflags -lpthread

void print(Json::Value &root){
	if(not root.empty()){
		LOG(INFO) << "size:" << root.size() << " type(string|array|object):" << root.isString() << root.isArray() << root.isObject();
		if(root.isString()){
			LOG(INFO) << root.asString();
		}
		else if(root.isArray() or root.isObject()){
			for(Json::Value::iterator it = root.begin(); it != root.end(); it++){
				print(*it);
			}
		}
	}
}

Json::Value root;

TEST(JsoncppTest, Test_append){
	// trivial
	root["key1"] = "value1";
	root["key2"] = "value2";
	
	// object
	Json::Value value3;
	value3["key31"] = "value31";
	value3["key32"] = "value32";
	root["key3"] = value3;

	// array
	Json::Value value4;
	value4[0] = "value41";
	value4[1] = "value42";
	value4[2] = "value43";
	root["key4"] = value4;

	// object array
	Json::Value value51;
	value51["key511"] = "value511";
	value51["key512"] = "value512";
	Json::Value value52;
	value52["key521"] = "value521";
	value52["key522"] = "value522";
	Json::Value value5;
	value5.append(value51);
	value5.append(value52);
	root["key5"] = value5;
	
	std::string doc = root.toStyledString();
	LOG(INFO) << "json size:" << root.size();
	LOG(INFO) << "json value:";
	fprintf(stdout, "%s\n", doc.c_str());
	print(root);
}

TEST(JsoncppTest, Test_read){
	Json::Reader reader;
	/*
	 * Jsoncpp 的 Json::Writer 类是一个纯虚类,并不能直接使用.
	 * Json::Writer 的子类:Json::FastWriter, Json::StyledWriter, Json::StyledStreamWriter.
	 */
	Json::FastWriter writer;
	Json::Value json;
	std::string doc = root.toStyledString();
	bool suc = reader.parse(doc, json);
	ASSERT_TRUE(suc == true);

	LOG(INFO) << json["key1"].asString();
	std::string array = writer.write(json["key4"]);
	LOG(INFO) << array;
	std::string object = writer.write(json["key3"]);
	LOG(INFO) << object;
	object = writer.write(json["key5"]);
	LOG(INFO) << object;
}

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	google::SetVersionString("1.0.0.0");
	google::SetUsageMessage(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	testing::InitGoogleTest(&argc, argv);

	int ret = RUN_ALL_TESTS();

	google::ShutdownGoogleLogging();
	return ret;
}

