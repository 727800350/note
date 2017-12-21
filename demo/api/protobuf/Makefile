all: addressbook.cpp addressbook.proto
	protoc -I=./ --cpp_out=./ addressbook.proto
	clang++ -g -Wall --std=c++11 addressbook.cpp addressbook.pb.cc -lpthread -lprotobuf -lglog -lgflags -lgtest
