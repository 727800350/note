#include <stdio.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional> // std::hash

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <wcg_api.h>

/* https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
 * To be able to use std::unordered_map (or one of the other unordered associative containers) with a user-defined key-type, you need to define two things:
 * A hash function: this must be a class that overrides operator() and calculates the hash value given an object of the key-type.
 * One particularly straight-forward way of doing this is to specialize the std::hash template for your key-type.
 * A comparison function for equality: this is required because the hash cannot rely on the fact that the hash function will always provide a unique hash value for every distinct key (i.e., it needs to be able to deal with collisions), so it needs a way to compare two given keys for an exact match.
 * You can implement this either as a class that overrides operator(), or as a specialization of std::equal, or – easiest of all – by overloading operator==() for your key type (as you did already).
 * The difficulty with the hash function is that if your key type consists of several members, you will usually have the hash function calculate hash values for the individual members, and then somehow combine them into one hash value for the entire object.
 * For good performance (i.e., few collisions) you should think carefully about how to combine the individual hash values to ensure you avoid getting the same output for different objects too often.
 * A fairly good starting point for a hash function is one that uses bit shifting and bitwise XOR to combine the individual hash values, just like this demo.
 * or combine all members into a string first, and then use std::hash<std::string>()
 */

class Key{
public:
	Key(const std::string &x, const std::string &y, int z) : x_(x), y_(y), z_(z) {}

	std::string to_string() const {
		return "x: " + x_ + " y: " + y_ + " z: " + std::to_string(z_);
	}

	bool operator==(const Key &other) const {
		return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
	}

	const std::string& x() const {
		return x_;
	}

	const std::string& y() const {
		return y_;
	}

	int z() const {
		return z_;
	}

private:
	std::string x_;
	std::string y_;
	int z_;
};

namespace std {
	template <>
	struct hash<Key>{
		std::size_t operator()(const Key &key) const{
			return (std::hash<std::string>()(key.x()) ^ (std::hash<std::string>()(key.y()) << 1) >> 1) ^ (std::hash<int>()(key.z()) << 1);
		}
	};
}

class Value{
public:
	Value() {} // 作为value 需要
	Value(const std::string &x, int y) : x_(x), y_(y) {}

	std::string to_string() const {
		return "x: " + x_ + " y: " + std::to_string(y_);
	}

private:
	std::string x_;
	int y_;
};

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;

	Key k1("k1x", "k1y", 1);
	std::unique_ptr<Key> k2(new Key("k2x", "k2y", 2));
	Key k22("k2x", "k2y", 2);

	Value v1("v1x", 1);
	Value v2("v2x", 2);

	std::unordered_map<Key, Value> map;

	map[k1] = v1;
	map.insert(std::make_pair(*k2, v2));

	auto pos = map.find(k22);
	if(pos == map.end()){
		LOG(INFO) << "not found";
	}
	else{
		LOG(INFO) << "key: " << pos->first.to_string() << " => value: " << pos->second.to_string();
	}

	google::ShutdownGoogleLogging();
	return 0;
}

