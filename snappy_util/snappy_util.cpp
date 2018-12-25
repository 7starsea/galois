///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include "snappy_util.h"
#include <cstdio>
#include <iostream>
#include <string.h>		///strchr

#include <time.h> 
#include <cstring>  /// std::memcpy

#include "snappy/snappy.h"

void encrypt(std::string & str, const std::string & key){
	const int n = (int)str.length(), m = (int)key.size();
	if (m <= 0)return;

	for (int i = 0; i < n; ++i)
		str[i] ^= key[i % m];
}

bool snappy_get_contents(const std::string& filename, std::string & data) {
	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == NULL) {
		perror(filename.c_str());
		return false;
	}

	data.clear();
	while (!feof(fp)) {
		char buf[4096];
		std::size_t ret = fread(buf, 1, 4096, fp);
		if (ret == 0 && ferror(fp)) {
			perror("fread");
			return false;
		}
		data.append(std::string(buf, ret));
	}
	fclose(fp);
	return true;
}
bool snappy_save_contents(const std::string& filename, const std::string& str) {
	FILE* fp = fopen(filename.c_str(), "wb");
	if (fp == NULL) {
		perror(filename.c_str());
		return false;
	}

	int ret = fwrite(str.data(), str.size(), 1, fp);
	if (ret != 1) {
		perror("fwrite");
		return false;
	}

	fclose(fp);
	return true;
}

bool snappy_compress_to_str(const std::string & infile, std::string & out, const std::string & key){
	std::string input;
	if (snappy_get_contents(infile, input)){
		encrypt(input, key);
		return snappy::Compress(input.data(), input.size(), &out) > 0;
	}
	return false;
}
bool snappy_uncompress_to_str(const std::string & infile, std::string & out, const std::string & key){
	std::string input;
	bool flag = false;
	if (snappy_get_contents(infile, input)){
		flag = snappy::Uncompress(input.data(), input.size(), &out);
		if (flag){
			encrypt(out, key);
		}
	}
	return flag;
}

bool snappy_compress_to_file(const std::string & infile, const std::string & outfile, const std::string & key){
	std::string out;
	if (snappy_compress_to_str(infile, out, key)){
		return snappy_save_contents(outfile, out);
	}
	return false;
}
bool snappy_uncompress_to_file(const std::string & infile, const std::string & outfile, const std::string & key){
	std::string out;
	if (snappy_uncompress_to_str(infile, out, key)){
		return snappy_save_contents(outfile, out);
	}
	return false;
}
