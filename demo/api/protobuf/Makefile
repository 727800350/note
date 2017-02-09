all: addressbook.cpp addressbook.proto
	protoc -I=./ --cpp_out=./ addressbook.proto
	clang++ -g -Wall addressbook.cpp addressbook.pb.cc -lpthread -lprotobuf
