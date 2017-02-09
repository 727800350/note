#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "addressbook.pb.h"

using namespace std;
using namespace addressbook;

int add_person(AddressBook &address_book, const char *name, const char *email, const char *phone_number){
	int id = address_book.person_size();
	Person *person = address_book.add_person();
	person->set_id(id);

// 	getline(cin, *person->mutable_name());
	person->set_name(std::string(name));

	person->set_email(email);

	Person::PhoneNumber* phone = person->add_phone();
	phone->set_number(phone_number);
	switch(id % 3){
		case 0:
			phone->set_type(Person::MOBILE);
			break;
		case 1:
			phone->set_type(Person::HOME);
			break;
		case 2:
			phone->set_type(Person::WORK);
			break;
	}

	return 0;
}

int print(AddressBook &address_book){
	for(int i = 0; i < address_book.person_size(); i++){
		const Person& person = address_book.person(i);

		cout << "Person ID: " << person.id() << endl;
		cout << "	Name: " << person.name() << endl;
		if(person.has_email()){
			cout << "	E-mail address: " << person.email() << endl;
		}

		for(int j = 0; j < person.phone_size(); j++){
			const Person::PhoneNumber& phone_number = person.phone(j);

			switch (phone_number.type()){
				case Person::MOBILE:
					cout << "	Mobile phone #: ";
					break;
				case Person::HOME:
					cout << "	Home   phone #: ";
					break;
				case Person::WORK:
					cout << "	Work   phone #: ";
					break;
			}
			cout << phone_number.number() << endl;
		}
	}

	return 0;
}

int mem2string(AddressBook &address_book, std::string &str){
	if(address_book.SerializeToString(&str) == false){
		fprintf(stderr, "SerializeToString error\n");
		return -1;
	}

	return 0;
}

int string2mem(AddressBook &address_book, const std::string &str){
	if(address_book.ParseFromString(str) == false){
		fprintf(stderr, "ParseFromString error\n");
		return -1;
	}

	fprintf(stdout, "\nGet from std::string\n");
	print(address_book);

	return 0;
}

int mem2char(AddressBook &address_book, char *str, int max_size){
	if(address_book.SerializeToArray(str, max_size) == false){
		fprintf(stderr, "SerializeToArray error\n");
		return -1;
	}

	return address_book.ByteSize();
}

/**
 * func: read from C char array
 * para: size: the acutal size of message in Bytes, obtained from pb.ByteSize()
 **/
int char2mem(AddressBook &address_book, const char *str, int size){
	if(address_book.ParseFromArray(str, size) == false){
		fprintf(stderr, "ParseFromgArray error\n");
		return -1;
	}

	fprintf(stdout, "\nGet from C char array\n");
	print(address_book);

	return 0;
}
// write to file using C++ stream
int mem2stream(AddressBook &address_book, const char *path){
	fstream output(path, ios::out | ios::trunc | ios::binary);
	if (!address_book.SerializeToOstream(&output)){
		cerr << "Failed to write address book." << endl;
		return -1;
	}

	return 0;
}

// read from file using C++ stream
int stream2mem(AddressBook &address_book, const char* path){
	fstream input(path, ios::in | ios::binary);
	if(!input){
		cout << path << ": File not found.	Creating a new file." << endl;
	}

	if(!address_book.ParseFromIstream(&input)){
		cerr << "Failed to parse address book." << endl;
		return -1;
	}

	fprintf(stdout, "\nGet from file using C++ stream\n");
	print(address_book);

	return 0;
}

int mem2file(AddressBook &address_book, const char *path){
	int fd = open(path, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if(fd <= 0){
		fprintf(stderr, "open %s error\n", path);
		return -1;
	}
	if(address_book.SerializeToFileDescriptor(fd) == false){
		fprintf(stderr, "SerializeToFileDescriptor error\n");
		close(fd);
		return -1;
	}
	close(fd);

	return 0;
}

int file2mem(AddressBook &address_book, const char *path){
	int fd = open(path, O_RDONLY);
	if(fd <= 0){
		fprintf(stderr, "open %s error\n", path);
		return -1;
	}
	if(address_book.ParseFromFileDescriptor(fd) == false){
		fprintf(stderr, "ParseFromFileDescriptor error\n");
		close(fd);
		return -1;
	}
	close(fd);

	fprintf(stdout, "\nGet from file\n");
	print(address_book);

	return 0;
}

int main(){
	// Verify that the version of the library that we linked against is compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	AddressBook address_book;

	int ret = 0;

	ret = add_person(address_book, "xxx", "xxx@xxx.com", "123456");
	assert(ret == 0);

	fprintf(stdout, "Original data\n");
	print(address_book);

	// std::string container
	std::string data;
	ret = mem2string(address_book, data);
	assert(ret == 0);
	ret = string2mem(address_book, data);
	assert(ret == 0);

	// char container
	int size = 1000;
	char *str = new (std::nothrow)char[size];
	ret = mem2char(address_book, str, size);
	assert(ret > 0);
	ret = char2mem(address_book, str, ret);
	assert(ret == 0);

	if(str != NULL){
		delete []str;
	}

	// stream file container
	char path[100] = {'\0'};
	snprintf(path, 100, "%s", "./addressbook.data.stream");
	ret = mem2stream(address_book, path);
	assert(ret == 0);
	ret = stream2mem(address_book, path);
	assert(ret == 0);

	// file container
	snprintf(path, 100, "%s", "./addressbook.data.file");
	ret = mem2file(address_book, path);
	assert(ret == 0);
	ret = file2mem(address_book, path);
	assert(ret == 0);

	// Optional: Delete all global objects allocated by libprotobuf
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

