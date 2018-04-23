
#include <fstream>
#include <sstream>
#include <iostream>
#include "socket_api.h"
#include "common.h"
#include <cstdio>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <cctype>
#include <iomanip>
#include <sys/time.h>
#include <algorithm>
#include <map>
#include "innercosHead.h"


using namespace std;

string g_server_ip;   
unsigned g_server_port; 
string g_domain_name;
string g_appid;
string g_ak;
string g_sk;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

const string g_ak_cloud_function = "XBqp5526jS9s38YN58DhBrLx";
const string g_sk_cloud_function = "O4BSHxjIYhbJAgap6BRRpJpGKWntot4axG";

static inline bool is_base64(unsigned char c) 
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

template <typename T>
std::string NumberToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

std::string ShaFile(const std::string& path);
void PrintHelpmsg(char** argv);

void GetBucketNotification(const string& bucketid);
void PutBucketNotification(const string& bucketid, const string& xml_file);

void GetService();
void GetServiceCloudFunction(const string& appid);

void GetBucket(const string& bucketid, const string& prefix);
void PutBucket(const string& bucketid, const string& auth);
void DeleteBucket(const string& bucketid);
void HeadBucket(const string& bucketid);

int GetObject(const string& bucketid, const string& cos_path, const string& save_name);
int GetObject(const string& bucketid, const string& cos_path, const string& save_name, unsigned start, unsigned end);
int GetObject(const string& bucketid, const string& cos_path, const string& save_name, unsigned start);
int GetObject(const string& bucketid, const string& cos_path, const string& save_name, unsigned start, unsigned end, const std::string& match);
//void PutObject(const string& bucketid, const string& cos_path, const string& file_name);
void AppendObject(const string& bucketid, const string& cos_path, const string& file_name, unsigned long long position, unsigned long long append_size);
void DeleteObject(const string& bucketid, const string& cos_path);
void ForbidObject(const string& bucketid, const string& cos_path, const string& val);
void DeleteMultiObject(const string& bucketid, const string& xml_str);
void HeadObject(const string& bucketid, const string& cos_path);

void CompleteMultiObject(const string& bucketid, const string& cos_path, const string& uploadid, const string& xml_str);

void GetBucketCors(const string& bucketid);
void OptionsObject(const string& bucketid, const string& cos_path, const string& origin, const string& reqmethod);

void SetBucketWebSite(const string& bucketid, const string& xml_file_name);
void GetBucketWebSite(const string& bucketid);
void DeleteBucketWebSite(const string& bucketid);

string GetSignature(const string& ak, const string& sk, const string& req_method, const string& uri,
    const std::map<std::string, std::string>& param_list,
    const std::map<std::string, std::string>& header_list);
string AuthFormat(const string& value, bool encode_slash);
string Sha1Str(const string& content);
string HmacSha1(const string& key, const string& content);
string StrTolower(const string& str);
string Bin2Hex(const char* inpute, unsigned size);
string Bin2Hex(const string& bin);
unsigned Hexstr2Bin(char c);
string Hex2Bin(const char* hex);
string Hex2Bin(const string& hex);
std::string Md5Base64Str(const std::string& content);
std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);

//int main(int argc, char** argv)
//{
//    ifstream conf("server.conf", ios::in);
//    if(!conf)
//    {
//        cerr << "Can't open config file: server.conf" << endl;
//        return -1;
//    }
//
//    conf >> g_server_ip;
//    conf >> g_server_port;
//    conf >> g_domain_name;
//    conf >> g_appid;
//    conf >> g_ak;
//    conf >> g_sk;
//    conf.close();
//
//    if (argc < 2)
//    {
//        PrintHelpmsg(argv);
//        return -1;
//    }
//
//    if (strcmp(argv[1], "gs") == 0)
//    {
//        if (argc == 2)
//        {
//            GetService();
//        }
//        else if (argc > 2)
//        {
//            GetServiceCloudFunction(argv[2]);
//        }
//        else
//        {
//            PrintHelpmsg(argv);
//        }
//        return 0;
//    }
//
//    if (argc < 3)
//    {
//        PrintHelpmsg(argv);
//        return -1;
//    }
//
//    if (strcmp(argv[1], "gb") == 0)
//    {
//        if (argc >= 4)
//        {
//            GetBucket(argv[2], argv[3]);
//        }
//        else
//        {
//            GetBucket(argv[2], "");
//        }
//    }
//    else if (strcmp(argv[1], "gbc") == 0 )
//    {
//        GetBucketCors(argv[2]);
//    }
//    else if (strcmp(argv[1], "gbn") == 0)
//    {
//        GetBucketNotification(argv[2]);
//    }
//    else if (strcmp(argv[1], "pbn") == 0 && argc >=4)
//    {
//        PutBucketNotification(argv[2], argv[3]);
//    }
//    else if (strcmp(argv[1], "pb") == 0 && argc >=4 )
//    {
//        PutBucket(argv[2], argv[3]);
//    }
//    else if (strcmp(argv[1], "db") == 0)
//    {
//        DeleteBucket(argv[2]);
//    }
//    else if (strcmp(argv[1], "hb") == 0)
//    {
//        HeadBucket(argv[2]);
//    }
//    else if (strcmp(argv[1], "go") == 0 && argc == 5)
//    {
//        GetObject(argv[2], argv[3], argv[4]);
//    }
//    else if (strcmp(argv[1], "go") == 0 && argc == 6)
//    {
//        GetObject(argv[2], argv[3], argv[4], atoll(argv[5]));
//    }
//    else if (strcmp(argv[1], "go") == 0 && argc == 7)
//    {
//        GetObject(argv[2], argv[3], argv[4], atoll(argv[5]), atoll(argv[6]));
//    }
//    else if (strcmp(argv[1], "go") == 0 && argc == 8)
//    {
//        GetObject(argv[2], argv[3], argv[4], atoll(argv[5]), atoll(argv[6]), argv[7]);
//    }
//    else if (strcmp(argv[1], "ao") == 0 && argc == 7)
//    {
//        AppendObject(argv[2], argv[3], argv[4], atoll(argv[5]), atoll(argv[6]));
//    }
//    else if (strcmp(argv[1], "oo") == 0 && argc >= 6)
//    {
//        OptionsObject(argv[2], argv[3], argv[4], argv[5]);
//    }
//    else if (strcmp(argv[1], "po") == 0 && argc >= 5)
//    {
//        PutObject(argv[2], argv[3], argv[4]);
//    }
//    else if (strcmp(argv[1], "ho") == 0 && argc >= 4)
//    {
//        HeadObject(argv[2], argv[3]);
//    }
//    else if (strcmp(argv[1], "do") == 0 && argc >= 4)
//    {
//        DeleteObject(argv[2], argv[3]);
//    }
//    else if (strcmp(argv[1], "dmo") == 0 && argc >= 4)
//    {
//        DeleteMultiObject(argv[2], argv[3]);
//    }
//    else if (strcmp(argv[1], "fo") == 0 && argc >= 5)
//    {
//        ForbidObject(argv[2], argv[3], argv[4]);
//    }
//    else if (strcmp(argv[1], "cmu") == 0 && argc >= 6)
//    {
//        CompleteMultiObject(argv[2], argv[3], argv[4], argv[5]);
//    }
//    else if (strcmp(argv[1], "sbw") == 0 && argc >= 4)
//    {
//        SetBucketWebSite(argv[2], argv[3]);
//    }
//    else if (strcmp(argv[1], "gbw") == 0 && argc >= 3)
//    {
//        GetBucketWebSite(argv[2]);
//    }
//    else if (strcmp(argv[1], "dbw") == 0 && argc >= 3)
//    {
//        DeleteBucketWebSite(argv[2]);
//    }
//    else
//    {
//        PrintHelpmsg(argv);
//        return -1;
//    }
//
//    return 0;
//}

void PrintHelpmsg(char** argv)
{
    cout << "Get Service:               " << argv[0] << " gs" << endl;
    cout << "Get Service cloudfunction: " << argv[0] << " gs appid" << endl;
	cout << "Get    Bucket:          " << argv[0] << " gb  bucketid [prefix]" << endl;	
	cout << "Put    Bucket:          " << argv[0] << " pb  bucketid auth(private|public-read)" << endl;	
	cout << "Delete Bucket:          " << argv[0] << " db  bucketid" << endl;	   
    cout << "Head   Bucket:          " << argv[0] << " hb  bucketid" << endl;
    cout << "Get Bucket Cors:        " << argv[0] << " gbc bucketid" << endl;
    cout << "Get Bucket Notification:" << argv[0] << " gbn bucketid" << endl;
    cout << "Put Bucket Cors:        " << argv[0] << " pbc bucketid xml_file" << endl;
    cout << "Put Bucket Notification:" << argv[0] << " pbn bucketid xml_file" << endl;
    cout << "Get    Object:          " << argv[0] << " go  bucketid cos_path save_name [range_start] [range_end] [if-match]" << endl;
    cout << "Append Object:          " << argv[0] << " ao  bucketid cos_path local_file position append_size" << endl;
    cout << "Put    Object:          " << argv[0] << " po  bucketid cos_path local_file" << endl;
    cout << "Head   Object:          " << argv[0] << " ho  bucketid cos_path" << endl;
    cout << "Options Object:         " << argv[0] << " oo  bucketid cos_path origin method" << endl;
    cout << "Delete Object:          " << argv[0] << " do  bucketid cos_path" << endl;   
    cout << "Forbid Object:          " << argv[0] << " fo  bucketid cos_path val" << endl;
    cout << "Delete Multi Object:    " << argv[0] << " dmo bucketid xml_str" << endl;
    cout << "Init Multi upload:      " << argv[0] << " imu bucketid cos_path" << endl;
    cout << "Abort Multi upload:     " << argv[0] << " amu bucketid cos_path uploadid" << endl;
    cout << "Upload part:            " << argv[0] << " up  bucketid cos_path uploadid partnumber local_file" << endl;
    cout << "List part:              " << argv[0] << " lp  bucketid cos_path uploadid" << endl;
    cout << "Complete Multi upload:  " << argv[0] << " cmu bucketid cos_path uploadid xml_str" << endl;
    cout << "set bucket web site     " << argv[0] << " sbw bucketid xml_info" << endl;
    cout << "get bucket web site     " << argv[0] << " gbw bucketid" << endl;
    cout << "delete bucket web site  " << argv[0] << " dbw bucketid" << endl;    
}

void GetService()
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = "service.";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "get", "/", param_list, header_list);
    int a = snprintf(tmp, buf_size, "GET / HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),
        sign.c_str());



    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void GetServiceCloudFunction(const string& appid)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = "service.";
    host += g_domain_name;
    header_list["Host"] = host;
    param_list["appid"] = appid;

    string sign = GetSignature(g_ak, g_sk, "get", "/", param_list, header_list);
    int a = snprintf(tmp, buf_size, "GET /?appid=%s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        appid.c_str(),
        host.c_str(),
        sign.c_str());



    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void GetBucket(const string& bucketid, const string& prefix)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "get", "/", param_list, header_list);
    int a = 0;
    if (!prefix.empty())
    {
        a = snprintf(tmp, buf_size, "GET /?prefix=%s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
            prefix.c_str(),
            host.c_str(),
            sign.c_str());
    }
    else
    {
        a = snprintf(tmp, buf_size, "GET / HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",          
            host.c_str(),
            sign.c_str());
    }

   

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
   
}

void PutBucket(const string& bucketid, const string& auth)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    header_list["x-cos-acl"] = auth;

    string sign = GetSignature(g_ak, g_sk, "put", "/", param_list, header_list);
    

    int a = snprintf(tmp, buf_size, "PUT / HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nx-cos-acl: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),
        sign.c_str(),
        auth.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        cout << "recv error" << endl;
    }
    close(fd);
}

void DeleteBucket(const string& bucketid)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "delete", "/", param_list, header_list);

    int a = snprintf(tmp, buf_size, "DELETE / HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),
        sign.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void HeadBucket(const string& bucketid)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "head", "/", param_list, header_list);

    int a = snprintf(tmp, buf_size, "HEAD / HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),       
        sign.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

//下载cos文件到本地,cos_path为去掉域名后的路径，以/开头
//例如：http://publictest-1234432101.sztest.file.tencent-cloud.com/0/1.xml,
//cos_path为/0/1.xml
int GetObject(const string& bucketid, const string& cos_path, const string& local_save_name)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "get", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "GET %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),       
        sign.c_str());

    

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return -1;
    }

    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return -1;
    }

    memset(tmp, 0, buf_size);

    gettimeofday(&begin_t, NULL);
    int recv_len = my_recv(fd, http_head_complete, tmp, buf_size, 6);
    if (recv_len <= 0)
    {
        cout << "download " << cos_path << " error, http head recv error" << endl;
        close(fd);
        return -3;
    }

    char* space = strchr(tmp, ' ');
    if (space == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;
        
        close(fd);
        return -4;
    }

    int httpcode = atoi(space + 1);
    if (httpcode != 200)
    {
        cout << "download " << cos_path << " http responde code error" << endl;
        cout << tmp << endl;
        close(fd);       
        return -1;
    }

    char* cont_length = strstr(tmp, "Content-Length: ");
    if (cont_length == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -6;
    }

    unsigned long long data_size = atoll(cont_length + strlen("Content-Length: "));    

    char* head_ender = strstr(tmp, "\r\n\r\n");
    if (head_ender == NULL)
    {
        cout << "download " << cos_path << " recv http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -7;
    }
    head_ender += strlen("\r\n\r\n");

    string http_rsp_header;
    http_rsp_header.assign(tmp, head_ender - tmp);
    cout << http_rsp_header;

    ofstream out_file;
    out_file.open(local_save_name.c_str(), ios::out | ios::binary);
    if (!out_file)
    {
        cout << "open file " << local_save_name << " fail" << endl;
        out_file.close();
        close(fd);
        return -8;
    }

    int header_length = head_ender - tmp;
    int received_file_length = recv_len - header_length;
    if (received_file_length > 0)
    {
        out_file.write(head_ender, received_file_length);
    }    

    data_size -= received_file_length;
    while (data_size > 0)
    {
        recv_len = recv(fd, tmp, 2000, 0);
        if (recv_len < 0)
        {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
            {
                return -errno;
            }
        }
        else if (recv_len == 0)
        {
            return -20;
        }
        else
        {
            out_file.write(tmp, recv_len);
            data_size -= recv_len;
        }
    }

    gettimeofday(&end_t, NULL);
    unsigned time_take = (end_t.tv_sec - begin_t.tv_sec) * 1000 + (end_t.tv_usec - begin_t.tv_usec) / 1000;   
    out_file.close();

    struct stat file_info;
    if (stat(local_save_name.c_str(), &file_info) != 0)
    {
        cout << "stat " << local_save_name << " error" << endl;
        return -1;
    }
    unsigned long long filesize = file_info.st_size;
    double speed = (double)filesize / (double)time_take * 1000.0 / 1024.0;
    cout << "download ok, saved as " << local_save_name << " time take " << time_take << " ms, speed: " << speed << " KB/s" << endl;
    close(fd);

    return 0;
}

//用http range 方式下载cos文件到本地，指定start， end
int GetObject(const string& bucketid, const string& cos_path, const string& local_save_name, unsigned start, unsigned end)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "get", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "GET %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nRange: bytes=%u-%u\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        sign.c_str(),
        start,
        end);



    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return -1;
    }

    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return -1;
    }

    memset(tmp, 0, buf_size);

    gettimeofday(&begin_t, NULL);
    int recv_len = my_recv(fd, http_head_complete, tmp, buf_size, 6);
    if (recv_len <= 0)
    {
        cout << "download " << cos_path << " error, http head recv error" << endl;
        close(fd);
        return -3;
    }

    char* space = strchr(tmp, ' ');
    if (space == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;

        close(fd);
        return -4;
    }

    int httpcode = atoi(space + 1);
    if (httpcode != 200)
    {
        cout << "download " << cos_path << " http responde code error" << endl;
        cout << tmp << endl;
        close(fd);
        return -1;
    }

    char* cont_length = strstr(tmp, "Content-Length: ");
    if (cont_length == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -6;
    }

    unsigned long long data_size = atoll(cont_length + strlen("Content-Length: "));

    char* head_ender = strstr(tmp, "\r\n\r\n");
    if (head_ender == NULL)
    {
        cout << "download " << cos_path << " recv http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -7;
    }
    head_ender += strlen("\r\n\r\n");

    string http_rsp_header;
    http_rsp_header.assign(tmp, head_ender - tmp);
    cout << http_rsp_header;

    ofstream out_file;
    out_file.open(local_save_name.c_str(), ios::out | ios::binary);
    if (!out_file)
    {
        cout << "open file " << local_save_name << " fail" << endl;
        out_file.close();
        close(fd);
        return -8;
    }

    int header_length = head_ender - tmp;
    int received_file_length = recv_len - header_length;
    if (received_file_length > 0)
    {
        out_file.write(head_ender, received_file_length);
    }

    data_size -= received_file_length;
    while (data_size > 0)
    {
        recv_len = recv(fd, tmp, 2000, 0);
        if (recv_len < 0)
        {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
            {
                return -errno;
            }
        }
        else if (recv_len == 0)
        {
            return -20;
        }
        else
        {
            out_file.write(tmp, recv_len);
            data_size -= recv_len;
        }
    }

    gettimeofday(&end_t, NULL);
    unsigned time_take = (end_t.tv_sec - begin_t.tv_sec) * 1000 + (end_t.tv_usec - begin_t.tv_usec) / 1000;
    out_file.close();

    struct stat file_info;
    if (stat(local_save_name.c_str(), &file_info) != 0)
    {
        cout << "stat " << local_save_name << " error" << endl;
        return -1;
    }
    unsigned long long filesize = file_info.st_size;
    double speed = (double)filesize / (double)time_take * 1000.0 / 1024.0;
    cout << "download ok, saved as " << local_save_name << " time take " << time_take << " ms, speed: " << speed << " KB/s" << endl;
    close(fd);   

    return 0;    
}

//用http range 方式下载cos文件到本地，指定start到文件结尾
int GetObject(const string& bucketid, const string& cos_path, const string& local_save_name, unsigned start)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "get", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "GET %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nRange: bytes=%u-\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        sign.c_str(),
        start);



    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return -1;
    }

    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return -1;
    }

    memset(tmp, 0, buf_size);

    gettimeofday(&begin_t, NULL);
    int recv_len = my_recv(fd, http_head_complete, tmp, buf_size, 6);
    if (recv_len <= 0)
    {
        cout << "download " << cos_path << " error, http head recv error" << endl;
        close(fd);
        return -3;
    }

    char* space = strchr(tmp, ' ');
    if (space == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;

        close(fd);
        return -4;
    }

    int httpcode = atoi(space + 1);
    if (httpcode != 200)
    {
        cout << "download " << cos_path << " http responde code error" << endl;
        cout << tmp << endl;
        close(fd);
        return -1;
    }

    char* cont_length = strstr(tmp, "Content-Length: ");
    if (cont_length == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -6;
    }

    unsigned long long data_size = atoll(cont_length + strlen("Content-Length: "));

    char* head_ender = strstr(tmp, "\r\n\r\n");
    if (head_ender == NULL)
    {
        cout << "download " << cos_path << " recv http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -7;
    }
    head_ender += strlen("\r\n\r\n");

    string http_rsp_header;
    http_rsp_header.assign(tmp, head_ender - tmp);
    cout << http_rsp_header;

    ofstream out_file;
    out_file.open(local_save_name.c_str(), ios::out | ios::binary);
    if (!out_file)
    {
        cout << "open file " << local_save_name << " fail" << endl;
        out_file.close();
        close(fd);
        return -8;
    }

    int header_length = head_ender - tmp;
    int received_file_length = recv_len - header_length;
    if (received_file_length > 0)
    {
        out_file.write(head_ender, received_file_length);
    }

    data_size -= received_file_length;
    while (data_size > 0)
    {
        recv_len = recv(fd, tmp, 2000, 0);
        if (recv_len < 0)
        {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
            {
                return -errno;
            }
        }
        else if (recv_len == 0)
        {
            return -20;
        }
        else
        {
            out_file.write(tmp, recv_len);
            data_size -= recv_len;
        }
    }

    gettimeofday(&end_t, NULL);
    unsigned time_take = (end_t.tv_sec - begin_t.tv_sec) * 1000 + (end_t.tv_usec - begin_t.tv_usec) / 1000;
    out_file.close();

    struct stat file_info;
    if (stat(local_save_name.c_str(), &file_info) != 0)
    {
        cout << "stat " << local_save_name << " error" << endl;
        return -1;
    }
    unsigned long long filesize = file_info.st_size;
    double speed = (double)filesize / (double)time_take * 1000.0 / 1024.0;
    cout << "download ok, saved as " << local_save_name << " time take " << time_take << " ms, speed: " << speed << " KB/s" << endl;
    close(fd);

    return 0;
}

//用http range 方式下载cos文件到本地，指定start， end，指定If-Match信息
int GetObject(const string& bucketid, const string& cos_path, const string& local_save_name, unsigned start, unsigned end, const std::string& match)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "get", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "GET %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nRange: bytes=%u-%u\r\nIf-Match: %s\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        sign.c_str(),
        start,
        end,
        match.c_str());



    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return -1;
    }

    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return -1;
    }

    memset(tmp, 0, buf_size);

    gettimeofday(&begin_t, NULL);
    int recv_len = my_recv(fd, http_head_complete, tmp, buf_size, 6);
    if (recv_len <= 0)
    {
        cout << "download " << cos_path << " error, http head recv error" << endl;
        close(fd);
        return -3;
    }

    char* space = strchr(tmp, ' ');
    if (space == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;

        close(fd);
        return -4;
    }

    int httpcode = atoi(space + 1);
    if (httpcode != 200)
    {
        cout << "download " << cos_path << " http responde code error" << endl;
        cout << tmp << endl;
        close(fd);
        return -1;
    }

    char* cont_length = strstr(tmp, "Content-Length: ");
    if (cont_length == NULL)
    {
        cout << "download " << cos_path << " http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -6;
    }

    unsigned long long data_size = atoll(cont_length + strlen("Content-Length: "));

    char* head_ender = strstr(tmp, "\r\n\r\n");
    if (head_ender == NULL)
    {
        cout << "download " << cos_path << " recv http rsp error" << endl;
        cout << tmp << endl;
        close(fd);
        return -7;
    }
    head_ender += strlen("\r\n\r\n");

    string http_rsp_header;
    http_rsp_header.assign(tmp, head_ender - tmp);
    cout << http_rsp_header;

    ofstream out_file;
    out_file.open(local_save_name.c_str(), ios::out | ios::binary);
    if (!out_file)
    {
        cout << "open file " << local_save_name << " fail" << endl;
        out_file.close();
        close(fd);
        return -8;
    }

    int header_length = head_ender - tmp;
    int received_file_length = recv_len - header_length;
    if (received_file_length > 0)
    {
        out_file.write(head_ender, received_file_length);
    }

    data_size -= received_file_length;
    while (data_size > 0)
    {
        recv_len = recv(fd, tmp, 2000, 0);
        if (recv_len < 0)
        {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
            {
                return -errno;
            }
        }
        else if (recv_len == 0)
        {
            return -20;
        }
        else
        {
            out_file.write(tmp, recv_len);
            data_size -= recv_len;
        }
    }

    gettimeofday(&end_t, NULL);
    unsigned time_take = (end_t.tv_sec - begin_t.tv_sec) * 1000 + (end_t.tv_usec - begin_t.tv_usec) / 1000;
    out_file.close();

    struct stat file_info;
    if (stat(local_save_name.c_str(), &file_info) != 0)
    {
        cout << "stat " << local_save_name << " error" << endl;
        return -1;
    }
    unsigned long long filesize = file_info.st_size;
    double speed = (double)filesize / (double)time_take * 1000.0 / 1024.0;
    cout << "download ok, saved as " << local_save_name << " time take " << time_take << " ms, speed: " << speed << " KB/s" << endl;
    close(fd);

    return 0;
}

//用put方式上传本地文件到cos, cos_path需要以/开头
//例如，如果cos_path为/test.txt, bucketid是publictest, appid是1234432101
//上传成功后可以从http://publictest-1234432101.sztest.file.tencent-cloud.com/test.txt下载文件
void PutObject(const string& bucketid, const string& cos_path, const string& local_file_name)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];
    

    ifstream in_file;
    in_file.open(local_file_name.c_str(), ios::in | ios::binary);
    if (!in_file)
    {
        cout << "open file " << local_file_name << " fail" << endl;
        in_file.close();
        return ;
    }

    struct stat file_info;
    if (stat(local_file_name.c_str(), &file_info) != 0)
    {
        cout << "stat " << local_file_name << " error" << endl;
        return ;
    }
    unsigned long long filesize = file_info.st_size;

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sha1 = ShaFile(local_file_name);
    string sign = GetSignature(g_ak, g_sk, "put", cos_path, param_list, header_list);
    int a = snprintf(tmp, buf_size, "PUT %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nx-cos-content-sha1:%s\r\nContent-Length: %llu\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        sign.c_str(),
        sha1.c_str(),
        filesize);

    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);
    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;

        memset(tmp, 0, buf_size);
        int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
        if (recv_len > 0)
        {
            cout << tmp << endl;
        }

        return;
    }

    while (!in_file.eof() && filesize > 0)
    {
        in_file.read(tmp, filesize > 1000 ? 1000 : filesize);
        send_len = my_send(fd, tmp, in_file.gcount());
        if (send_len != 0)
        {
            cout << "upload " << local_file_name << " send file data error" << endl;
            in_file.close();

            memset(tmp, 0, buf_size);
            int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
            if (recv_len > 0)
            {
                cout << tmp << endl;
            }

            return ;
        }
        filesize -= in_file.gcount();
    }
   
    in_file.close();

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
    if (recv_len > 0)
    {        
        cout << tmp << endl;          
    }

    gettimeofday(&end_t, NULL);
    filesize = file_info.st_size;
    unsigned time_take = (end_t.tv_sec - begin_t.tv_sec) * 1000 + (end_t.tv_usec - begin_t.tv_usec) / 1000;
    double speed = (double)filesize / (double)time_take * 1000.0 / 1024.0;
    cout << "upload  " << local_file_name << " ok, take " << time_take << " ms, speed: " << speed << " KB/s" << endl;

    delete[] tmp;
    close(fd);       
}

void DeleteObject(const string& bucketid, const string& cos_path)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "delete", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "DELETE %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        sign.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void DeleteMultiObject(const string& bucketid, const string& xml_str)
{
    string content_md5 = Md5Base64Str(xml_str);
    const unsigned buf_size = 10 * 1024 * 1024;

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    header_list["Content-MD5"] = content_md5;
    param_list["delete"] = "";

    string sign = GetSignature(g_ak, g_sk, "post", "/", param_list, header_list);

    char* tmp = new char[buf_size];
    int a = snprintf(tmp, buf_size, "POST /?delete HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-MD5: %s\r\nContent-Length: %zu\r\n\r\n%s",        
        host.c_str(),
        sign.c_str(),
        content_md5.c_str(),
        xml_str.size(),
        xml_str.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void HeadObject(const string& bucketid, const string& cos_path)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "head", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "HEAD %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        sign.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void CompleteMultiObject(const string& bucketid, const string& cos_path, const string& uploadid, const string& xml_str)
{
    
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];
    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;   
    param_list["uploadid"] = uploadid;
    string sign = GetSignature(g_ak, g_sk, "post", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "POST %s?uploadid=%s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: %zu\r\n\r\n%s",
        cos_path.c_str(),
        uploadid.c_str(),
        host.c_str(),
        sign.c_str(),        
        xml_str.size(),
        xml_str.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void GetBucketCors(const string& bucketid)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    param_list["cors"] = "";

    string sign = GetSignature(g_ak, g_sk, "get", "/", param_list, header_list);    

    int a = snprintf(tmp, buf_size, "GET /?cors HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),
        sign.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void GetBucketNotification(const string& bucketid)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    param_list["notification"] = "";

    string sign = GetSignature(g_ak_cloud_function, g_sk_cloud_function, "get", "/", param_list, header_list);

    int a = snprintf(tmp, buf_size, "GET /?notification HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),
        sign.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    } 

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void PutBucketNotification(const string& bucketid, const string& xml_file)
{    
    ifstream in_file;
    in_file.open(xml_file.c_str(), ios::in | ios::binary);
    if (!in_file)
    {
        cout << "open file " << xml_file << " fail" << endl;
        in_file.close();
        return;
    }

    struct stat file_info;
    if (stat(xml_file.c_str(), &file_info) != 0)
    {
        cout << "stat " << xml_file << " error" << endl;
        return;
    }
    unsigned long long filesize = file_info.st_size;

    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    param_list["notification"] = "";

    string sign = GetSignature(g_ak_cloud_function, g_sk_cloud_function, "put", "/", param_list, header_list);

    int a = snprintf(tmp, buf_size, "PUT /?notification HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: %llu\r\n\r\n",
        host.c_str(),
        sign.c_str(),
        filesize);

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    while (!in_file.eof() && filesize > 0)
    {
        in_file.read(tmp, filesize > 1000 ? 1000 : filesize);
        send_len = my_send(fd, tmp, in_file.gcount());
        if (send_len != 0)
        {
            cout << "send " << xml_file << " error" << endl;
            in_file.close();
            return;
        }
        filesize -= in_file.gcount();
    }

    in_file.close();

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void OptionsObject(const string& bucketid, const string& cos_path, const string& origin, const string& reqmethod)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    header_list["Origin"] = origin;
    header_list["Access-Control-Request-Method"] = reqmethod;

    string sign = GetSignature(g_ak, g_sk, "options", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "OPTIONS %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nOrigin: %s\r\nAccess-Control-Request-Method: %s\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        sign.c_str(),
        origin.c_str(),
        reqmethod.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

void ForbidObject(const string& bucketid, const string& cos_path, const string& val)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    header_list["Forbid"] = val;
    param_list["forbidden"] = "";

    string sign = GetSignature(g_ak, g_sk, "put", cos_path, param_list, header_list);

    int a = snprintf(tmp, buf_size, "PUT %s?forbidden HTTP/1.1\r\nHost: %s\r\nForbid:%s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        cos_path.c_str(),
        host.c_str(),
        val.c_str(),
        sign.c_str());

    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 6);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }
    else
    {
        perror("recv error");
    }
    close(fd);
}

string GetSignature(const string& ak, const string& sk, const string& req_method, const string& uri, 
    const std::map<std::string, std::string>& param_list,
    const std::map<std::string, std::string>& header_list)
{
    string authorization = "q-sign-algorithm=sha1&q-ak=";
    authorization += ak;
    authorization += "&q-sign-time=";
    time_t now = time(NULL);
    authorization += NumberToString<time_t>(now - 1000);
    authorization += ";";
    authorization += NumberToString<time_t>(now + 1000);
    authorization += "&q-key-time=";
    authorization += NumberToString<time_t>(now - 1000);
    authorization += ";";
    authorization += NumberToString<time_t>(now + 1000);
    authorization += "&q-header-list=";
    if (!header_list.empty())
    {
        for (std::map<std::string, std::string>::const_iterator it = header_list.begin();
            it != header_list.end(); ++it)
        {
            authorization += StrTolower(it->first);
            authorization += ";";
        }
        authorization.erase(authorization.size() - 1);
    }
    authorization += "&q-url-param-list=";     
    if (!param_list.empty())
    {
        for (std::map<std::string, std::string>::const_iterator it = param_list.begin();
            it != param_list.end(); ++it)
        {
            authorization += StrTolower(it->first);
            authorization += ";";
        }
        authorization.erase(authorization.size() - 1);
    }
    authorization += "&q-signature=";   

    string sign_time = NumberToString<time_t>(now-1000);
    sign_time += ";";
    sign_time += NumberToString<time_t>(now + 1000);
    string sign_key = HmacSha1(sk, sign_time);

    string format_string = req_method;
    format_string += '\n';
    format_string += AuthFormat(uri, false);
    format_string += '\n';
    if (!param_list.empty())
    {
        for (std::map<std::string, std::string>::const_iterator it = param_list.begin();
            it != param_list.end(); ++it)
        {
            format_string += AuthFormat(it->first, true);
            format_string += "=";
            format_string += AuthFormat(it->second, true);
            format_string += "&";
        }

        format_string.erase(format_string.size() - 1);
    }
    format_string += '\n';

    if (!header_list.empty())
    {
        for (std::map<std::string, std::string>::const_iterator it = header_list.begin();
            it != header_list.end(); ++it)
        {
            format_string += StrTolower(it->first);
            format_string += "=";
            format_string += AuthFormat(it->second, true);
            format_string += "&";
        }
        format_string.erase(format_string.size() - 1);
    }
    format_string += '\n';   

    string string_to_sign = "sha1\n";
    string_to_sign += NumberToString<time_t>(now - 1000);
    string_to_sign += ";";
    string_to_sign += NumberToString<time_t>(now + 1000);
    string_to_sign += '\n';
    string_to_sign += Sha1Str(format_string);
    string_to_sign += '\n';   

    string signature = HmacSha1(sign_key, string_to_sign);
    authorization += signature;

    return authorization;
}

string AuthFormat(const string& value, bool encode_slash)
{
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;


    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
    {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
        }
        else if (c == '/' &&  !encode_slash)
        {
            escaped << c;
        }
        else
        {
            // Any other characters are percent-encoded      
            escaped << nouppercase << '%' << setw(2) << int((unsigned char)c);
        }

    }

    return escaped.str();
}

string Sha1Str(const string& content)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(content.data()), content.size(), hash);
    return Bin2Hex(reinterpret_cast<const char*>(hash), SHA_DIGEST_LENGTH);
}

string HmacSha1(const string& key, const string& content)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned len = EVP_MAX_MD_SIZE;
    HMAC(EVP_sha1(), key.data(), key.size(), reinterpret_cast<const unsigned char*>(content.data()), content.size(), hash, &len);
    return Bin2Hex(reinterpret_cast<const char*>(hash), len);
}

string StrTolower(const string& str)
{
    string ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}

string Bin2Hex(const char* inpute, unsigned size)
{
    char tmp[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    if (size == 0)
    {
        return "NULL";
    }
    string ret;
    unsigned char low, high;
    for (unsigned n = 0; n < size; ++n)
    {
        low = inpute[n] & 0x0f;
        high = inpute[n] & 0xf0;
        high >>= 4;
        ret += tmp[high];
        ret += tmp[low];
    }
    return ret;
}

string Bin2Hex(const string& bin)
{
    return Bin2Hex(bin.data(), bin.size());
}

unsigned Hexstr2Bin(char c)
{
    unsigned val = 0;
    if (c >= '0' && c <= '9')
    {
        val = c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        val = c - 'a' + 0xa;
    }
    else if (c >= 'A' && c <= 'F')
    {
        val = c - 'A' + 0xa;
    }
    return val;
}

string Hex2Bin(const char* hex)
{
    if (strcmp(hex, "NULL") == 0)
    {
        return "";
    }
    string ret;
    unsigned val;
    const char* p = hex;
    while (*p != '\0' && *(p + 1) != '\0')
    {
        val = Hexstr2Bin(*p++);
        val *= 16;
        val += Hexstr2Bin(*p++);
        ret += (unsigned char)val;
    }
    return ret;
}

string Hex2Bin(const string& hex)
{
    return Hex2Bin(hex.c_str());
}

std::string Md5Base64Str(const std::string& content)
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(content.data()), content.size(), hash);
    return base64_encode(reinterpret_cast<const unsigned char*>(hash), MD5_DIGEST_LENGTH);
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) 
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) 
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) 
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }

    return ret;

}

std::string base64_decode(std::string const& encoded_string) 
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) 
    {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) 
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::string ShaFile(const std::string& path)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    const unsigned buf_size = 1024 * 1024;
    char* tmp = new char[buf_size];


    ifstream in_file;
    in_file.open(path.c_str(), ios::in | ios::binary);
    if (!in_file)
    {
        cout << "cal sha open file " << path << " fail" << endl;
        in_file.close();
        return "";
    }

    struct stat file_info;
    if (stat(path.c_str(), &file_info) != 0)
    {
        cout << "stat " << path << " error" << endl;
        return "";
    }
    unsigned long long filesize = file_info.st_size;

    SHA_CTX c;
    SHA1_Init(&c);

    while (!in_file.eof() && filesize > 0)
    {
        in_file.read(tmp, filesize > buf_size ? buf_size : filesize);
        SHA1_Update(&c, tmp, in_file.gcount() );
        filesize -= in_file.gcount();
    }

    in_file.close();
    delete[] tmp;
    SHA1_Final(hash, &c);

    return Bin2Hex(reinterpret_cast<const char*>(hash), SHA_DIGEST_LENGTH);
}


void GetBucketWebSite(const string& bucketid)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];



    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    string sign = GetSignature(g_ak, g_sk, "get", "/", param_list, header_list);
    int a = snprintf(tmp, buf_size, "GET /?website HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),
        sign.c_str());
    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);
    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
    if (recv_len > 0)
    {
        std::cout << "==== get bucket_website rsp====" << std::endl;
        cout << tmp << endl;
    }

    gettimeofday(&end_t, NULL);
    delete[] tmp;
    close(fd);
}


void SetBucketWebSite(const string& bucketid, const string& xml_file_name)
{
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    //read file
    ifstream in_file;
    in_file.open(xml_file_name.c_str(), ios::in | ios::binary);
    if (!in_file)
    {
        cout << "open file " << xml_file_name << " fail" << endl;
        in_file.close();
        return ;
    }

    struct stat file_info;
    if (stat(xml_file_name.c_str(), &file_info) != 0)
    {
        cout << "stat " << xml_file_name << " error" << endl;
        return ;
    }
    unsigned long long filesize = file_info.st_size;
    in_file.read(tmp,  filesize);
    std::string xml_content;
    xml_content.append(tmp, filesize);
    std::string content_md5 = Md5Base64Str(xml_content);
    //string sha1 = ShaFile(file_name);
    string sign = GetSignature(g_ak, g_sk, "put", "/", param_list, header_list);
    int a = snprintf(tmp, buf_size, "PUT /?website HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nCONTENT-MD5:%s\r\nContent-Length: %llu\r\n\r\n%s",
        host.c_str(),
        sign.c_str(),
        content_md5.c_str(),
        filesize,
        xml_content.c_str());
    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);
    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
    if (recv_len > 0)
    {        
        std::cout << "========= set bucket_website rsp =========" << std::endl;
        cout << tmp << endl;          
    }

    gettimeofday(&end_t, NULL);
    delete[] tmp;
    close(fd);       
}

void DeleteBucketWebSite(const string& bucketid)
{       
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];
    
    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;

    //string sha1 = ShaFile(file_name);
    string sign = GetSignature(g_ak, g_sk, "delete", "/", param_list, header_list);
    int a = snprintf(tmp, buf_size, "DELETE /?website HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: 0\r\n\r\n",
        host.c_str(),
        sign.c_str());
    //std::cout << "head_buf:" << tmp << std::endl; 
    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);
    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }
    
    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;
        return;
    }

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }

    gettimeofday(&end_t, NULL);
    delete[] tmp;
    close(fd);
}

//以append方式上传本地文件到cos，注意一次不一定可以把文件传完，可能需要多次调用
void AppendObject(const string& bucketid, const string& cos_path, const string& local_file_name, unsigned long long position, unsigned long long append_size)
{   
    char* file_buf = new char[append_size];
    const unsigned buf_size = 10 * 1024 * 1024;
    char* tmp = new char[buf_size];

    ifstream in_file;
    in_file.open(local_file_name.c_str(), ios::in | ios::binary);
    if (!in_file)
    {
        cout << "open file " << local_file_name << " fail" << endl;
        in_file.close();
        return;
    }
    
    in_file.seekg(position);
    if (!in_file)
    {
        cout << "invalid position " << position << " of " << local_file_name << endl;
        return;
    }
    
    int ret = snprintf(tmp, buf_size, "%llu", position);
    string position_str(tmp, ret);

    in_file.read(file_buf, append_size);
    

    map<string, string> param_list;
    map<string, string> header_list;
    string host = bucketid;
    host += "-";
    host += g_appid;
    host += ".";
    host += g_domain_name;
    header_list["Host"] = host;
    param_list["append"] = "";
    param_list["position"] = position_str;

   
    string sign = GetSignature(g_ak, g_sk, "post", cos_path, param_list, header_list);
    int a = snprintf(tmp, buf_size, "POST %s?append&position=%llu HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Length: %lld\r\n\r\n",
        cos_path.c_str(),
        position,
        host.c_str(),
        sign.c_str(),
        in_file.gcount());

    struct timeval begin_t, end_t;
    gettimeofday(&begin_t, NULL);
    int fd = my_connect(g_server_ip.c_str(), g_server_port);
    if (fd <= 0)
    {
        cout << "connect httpapi fail" << endl;
        return;
    }

    int send_len = my_send(fd, tmp, a);
    if (send_len != 0)
    {
        cout << "send request error" << endl;

        memset(tmp, 0, buf_size);
        int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
        if (recv_len > 0)
        {
            cout << tmp << endl;
        }

        return;
    }

   
    send_len = my_send(fd, file_buf, in_file.gcount());
    if (send_len != 0)
    {
        cout << "upload " << local_file_name << " send file data error" << endl;
        in_file.close();

        memset(tmp, 0, buf_size);
        int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
        if (recv_len > 0)
        {
            cout << tmp << endl;
        }

        return;
    }   

    memset(tmp, 0, buf_size);
    int recv_len = my_recv(fd, http_complete, tmp, buf_size, 2);
    if (recv_len > 0)
    {
        cout << tmp << endl;
    }

    gettimeofday(&end_t, NULL);   
    unsigned time_take = (end_t.tv_sec - begin_t.tv_sec) * 1000 + (end_t.tv_usec - begin_t.tv_usec) / 1000;
    double speed = (double)in_file.gcount() / (double)time_take * 1000.0 / 1024.0;
    cout << "upload  " << local_file_name << " ok, take " << time_take << " ms, speed: " << speed << " KB/s" << endl;

    delete[] tmp;
    delete[] file_buf;
    in_file.close();
    close(fd);
}

