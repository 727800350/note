#include <stdio.h>
#include <string.h>
#include <string>

// ref: http://blog.csdn.net/sky04/article/details/6881649
// all these chars are < 127, the max value of type char is 127
static const std::string base64_chars = 
						 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						 "abcdefghijklmnopqrstuvwxyz"
						 "0123456789+/";

static bool is_base64(char c){
	return (isalnum(c) || (c == '+') || (c == '/'));
}

/**
 * convert a string s13 to base64 manually example:
 * 1. convert to ascii: 115 49 51
 * 2. convert to binary data: 01110011 00110001 00110011
 * 3. group in 6 bit(4 groups): 011100 110011 000100 110011
 * 4. convert 6 bits to 8 bits(add 0 in front), 就是 00011100 00110011 00000100 00110011
 * 5. convert to decimal: 28 51 4 51
 * 6. refer the base64 table, we get: c z E z
 **/
std::string base64_encode(const char * str){
	char char_array_3[3] = {'\0'};
	char char_array_4[4] = {'\0'};
	std::string res;

	int i = 0;
	int len = strlen(str);
	for(int index = 0; index < len; index ++){
		char_array_3[i++] = str[index];

		// change 3 Bytes to 4 Bytes
		if(i == 3){
			/**
			 * 0xfc: 1111 1100
			 * using & 0xfc, we get the first 6 bits of char_array_3[0]
			 * using >> 2, we expand the 6 bits to a Byte, that is char_array_4[0]
			 **/
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			/**
			 * 0x03: 0000 0011
			 * firstly we get the last 2 bits of char_array_3[0] and make them the first 4 bits of char_array_4[1]
			 * 0xf0: 1111 0000
			 * then we get the first 4 bits of char_array_3[1] and make them the last 4 bits of char_array_4[1]
			 **/
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			/**
			 * get the last 4 bits of char_array_3[1] and make them the first 6 bits of char_array_4[2]
			 * get the first 2 bits of char_array_3[2] and make them the last 2 bits of char_array_4[2]
			 **/
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			/**
			 * get the last 6 bits of char_array_3[2] and make them char_array_4[3]
			 **/
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; i < 4; i++){
				res += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	/**
	 * when len(str) is not a multiple of 3
	 * if len(str) % 3 = 1,
	 * becase the last one byte will be converted to two bytes in base64 format
	 * then add two '=' to the end of base64 result,
	 **/
	if(i){
		for(int j = i; j < 3; j++){
			char_array_3[j] = '\0';
		}
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for(int j = 0; j < i + 1; j++){
			res += base64_chars[char_array_4[j]];
		}
		while(i++ < 3){
			res += '=';
		}
	}

	return res;
}

std::string base64_decode(std::string const& encoded_string){
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	char char_array_4[4], char_array_3[3];
	std::string ret;

	while(in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])){
		char_array_4[i++] = encoded_string[in_];
		in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; i < 3; i++){
				ret += char_array_3[i];
			}
			i = 0;
		}
	}

	if(i){
		for(j = i; j < 4; j++){
			char_array_4[j] = 0;
		}

		for (j = 0; j < 4; j++){
			char_array_4[j] = base64_chars.find(char_array_4[j]);
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; j < i - 1; j++){
			ret += char_array_3[j];
		}
	}

	return ret;
}

int main(){
	const char *str = "s123";
	std::string str_encode = base64_encode(str);
	fprintf(stdout, "encode result of [%s] is [%s]\n", str, str_encode.c_str());

	std::string str_decode = base64_decode(str_encode);
	fprintf(stdout, "decode result of [%s] is [%s]\n", str_encode.c_str(), str_decode.c_str());
	return 0;
}

