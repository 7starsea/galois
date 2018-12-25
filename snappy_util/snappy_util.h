///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef SHANNON_SNAPPY_UTIL_H
#define SHANNON_SNAPPY_UTIL_H

#include <string>

void encrypt(std::string & str, const std::string & key);

bool snappy_get_contents(const std::string& filename, std::string & data);
bool snappy_save_contents(const std::string& filename, const std::string& str);

bool snappy_compress_to_str(const std::string & infile, std::string & out, const std::string & key);
bool snappy_uncompress_to_str(const std::string & infile, std::string & out, const std::string & key);

bool snappy_compress_to_file(const std::string & infile, const std::string & outfile, const std::string & key);
bool snappy_uncompress_to_file(const std::string & infile, const std::string & outfile, const std::string & key);

#endif
