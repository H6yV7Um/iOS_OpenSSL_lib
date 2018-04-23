#include "common.h"
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <openssl/sha.h>
#include <fstream>
#include <iostream>

using namespace std;


int cal_file_sha1(const std::string& path, char *sha)
{
	FILE *fp = fopen(path.c_str(), "rb");
	if (fp == NULL)
	{
		printf("open file %s fail\n", path.c_str());
		return -1;
	}

	SHA_CTX c;
	SHA1_Init(&c);

	char buf[100 * 1000];
	for (;;)
	{
		ssize_t n = read(fileno(fp), buf, sizeof(buf));
		if (n < 0)
		{
			printf("read from file %s fail\n", path.c_str());
			return -1;
		}
		else if (n == 0)
		{
			break;
		}

		SHA1_Update(&c, buf, n);

		if ((size_t)n < sizeof(buf))
		{
			break;
		}
	}

	fclose(fp);
	SHA1_Final((unsigned char *)sha, &c);

	return 0;
}

int cal_file_sha1(const std::string& path, unsigned long long position, unsigned long long data_size, char* sha)
{
	ifstream in_file;
	in_file.open(path.c_str(), ios::in | ios::binary);
	if (!in_file)
	{
		cout << "open file " << path << " fail" << endl;
		return -1;
	}
	
	in_file.seekg(position);
	if (in_file.eof())
	{
		cout << "Too large position" << endl;
		return -2;
	}

	char tmp[2000];
	unsigned long long left_size = data_size;
	SHA_CTX c;
	SHA1_Init(&c);
	while (!in_file.eof() && left_size > 0)
	{
		in_file.read(tmp, left_size > 2000 ? 2000 : left_size);
		SHA1_Update(&c, tmp, in_file.gcount());
		left_size -= in_file.gcount();
	}
	in_file.close();
	SHA1_Final((unsigned char *)sha, &c);
	return 0;
}


int cal_str_sha1(const std::string& str, char* sha)
{
	SHA_CTX c;
	SHA1_Init(&c);
	SHA1_Update(&c, str.data(), str.size() );
	SHA1_Final((unsigned char *)sha, &c);

	return 0;
}

int http_complete(void *buf, unsigned len)
{
	string str;
	str.assign((char*)buf, len);
	size_t head_tail = str.find("\r\n\r\n");
	if (head_tail == string::npos)
	{
		if (len > 1000)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	head_tail += strlen("\r\n\r\n");

	size_t cotent_length = str.find("Content-Length: ");
	if (cotent_length == string::npos)
	{
		return head_tail;
	}

	if (cotent_length < head_tail)
	{
		char* p = (char*)buf;
		unsigned body_len = atoi(p + cotent_length + strlen("Content-Length: "));
		if (len >= head_tail + body_len)
		{
			return head_tail + body_len;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

int http_head_complete(void *buf, unsigned len)
{
	string str;
	str.assign((char*)buf, len);
	size_t head_tail = str.find("\r\n\r\n");
	if (head_tail == string::npos)
	{
		if (len > 1000)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	head_tail += strlen("\r\n\r\n");	
	return head_tail;	
}
