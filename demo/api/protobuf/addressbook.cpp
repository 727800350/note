#include <stdio.h>
#include <string>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include "addressbook.pb.h"

const int max_vl = 10 * 1024 * 1024;

wcg::pb::ex::AddressBook address_book;

int add_person(wcg::pb::ex::AddressBook &address_book, const char *name, const char *email, const char *phone_number);

TEST(pbTest, Test_add_person){
	ASSERT_TRUE(add_person(address_book, "xxx", "xxx@xxx.com", "123456") == 0);
	ASSERT_TRUE(add_person(address_book, "yyy", "yyy@yyy.com", "654321") == 0);
	ASSERT_TRUE(address_book.person_size() == 2);
	fprintf(stdout, "%s", address_book.Utf8DebugString().c_str());
}

TEST(pbTest, Test_string){
	std::string v;
	ASSERT_TRUE(address_book.SerializeToString(&v) == true);

	wcg::pb::ex::AddressBook ex;
	ASSERT_TRUE(ex.ParseFromString(v) == true);
	fprintf(stdout, "%s", ex.Utf8DebugString().c_str());
}

TEST(pbTest, Test_char){
	char *v = new char[max_vl];
	int vl = address_book.ByteSize();
	ASSERT_TRUE(address_book.SerializeToArray(v, max_vl) == true);

	wcg::pb::ex::AddressBook ex;
	ASSERT_TRUE(ex.ParseFromArray(v, vl) == true);
	fprintf(stdout, "%s", ex.Utf8DebugString().c_str());
}

TEST(pbTest, Test_file){
	const char *path = "./data.pb";
	FILE *fp = fopen(path, "wb");
	int fd = fileno(fp);
	ASSERT_TRUE(fp != NULL);
	ASSERT_TRUE(fd > 0);

	ASSERT_TRUE(address_book.SerializeToFileDescriptor(fd) == true);
	fclose(fp);
	fp = NULL;
	fd = -1;

	fp = fopen(path, "rb");
	fd = fileno(fp);
	ASSERT_TRUE(fp != NULL);
	ASSERT_TRUE(fd > 0);

	wcg::pb::ex::AddressBook ex;
	ASSERT_TRUE(ex.ParseFromFileDescriptor(fd) == true);
	fclose(fp);
	fp = NULL;
	fd = -1;

	fprintf(stdout, "%s", ex.Utf8DebugString().c_str());
}

// http://www.p-chao.com/2017-09-04/protobuf%E5%8F%8D%E5%B0%84%E8%AF%A6%E8%A7%A3
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
TEST(pbTest, Test_Prototype){
	const google::protobuf::Descriptor *descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("wcg.pb.ex.AddressBook");
	ASSERT_TRUE(descriptor != NULL);

	const google::protobuf::Message *prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	ASSERT_TRUE(prototype != NULL);

	wcg::pb::ex::AddressBook *ab = (wcg::pb::ex::AddressBook *)prototype->New();
	ASSERT_TRUE(ab != NULL);

	wcg::pb::ex::Person *person = ab->add_person();
	person->set_name("wcg");
	fprintf(stdout, "%s", ab->Utf8DebugString().c_str());
	delete ab;
}

TEST(pbTest, Test_Reflector){
	wcg::pb::ex::AddressBook ab;
	const google::protobuf::Descriptor* descriptor = ab.GetDescriptor();
	ASSERT_TRUE(descriptor != NULL);

	const google::protobuf::Reflection* reflecter = ab.GetReflection();
	ASSERT_TRUE(reflecter != NULL);

	const google::protobuf::FieldDescriptor* owner = descriptor->FindFieldByName("owner");
	ASSERT_TRUE(owner != NULL);
	reflecter->SetString(&ab, owner, "Eric Wang");

	const google::protobuf::FieldDescriptor* desc = descriptor->FindFieldByName("desc");
	reflecter->SetString(&ab, desc, "an address book example");

	fprintf(stdout, "%s", ab.Utf8DebugString().c_str());
}

int main(int argc, char* argv[]){
	// Verify that the version of the library that we linked against is compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	address_book.set_owner("Eric Wang");
	address_book.set_desc("an address book example");

	testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();

	// Optional: Delete all global objects allocated by libprotobuf
	google::protobuf::ShutdownProtobufLibrary();

	return ret;
}

int add_person(wcg::pb::ex::AddressBook &address_book, const char *name, const char *email, const char *phone_number){
	int id = address_book.person_size();
	wcg::pb::ex::Person *person = address_book.add_person();
	person->set_name(name);
	person->set_email(email);

	wcg::pb::ex::PhoneNumber* phone = person->add_phone();
	phone->set_number(phone_number);
	switch(id % 3){
		case 0:
			phone->set_type(wcg::pb::ex::MOBILE);
			break;
		case 1:
			phone->set_type(wcg::pb::ex::HOME);
			break;
		case 2:
			phone->set_type(wcg::pb::ex::WORK);
			break;
	}

	return 0;
}

