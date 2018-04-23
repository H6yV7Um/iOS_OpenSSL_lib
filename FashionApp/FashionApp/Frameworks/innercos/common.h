#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>

unsigned Hexstr2Bin(char c);


int http_complete(void *buf, unsigned len);

int cal_file_sha1(const std::string& path, char *sha);

int cal_file_sha1(const std::string& path, unsigned long long position, unsigned long long data_size, char* sha);

int cal_str_sha1(const std::string& str, char* sha);

int http_head_complete(void *buf, unsigned len);

#endif
