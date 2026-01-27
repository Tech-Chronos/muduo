#pragma once
#include "../EchoServer.h"
#include <fstream>

class Util
{
private:    
    static int HexToDecimal(char hex);
public:
    // 字符串分割函数
    static int Split(const std::string& src, const std::string& sep, std::vector<std::string>* arr);

    // 读取文件内容
    static bool ReadFile(const std::string& filename, std::string* buf);

    // 写文件
    static bool WriteFile(const std::string& filename, std::string* buf);

    // url encode
    static std::string UrlEncode(const std::string& url, bool space_to_plus);

    // url decode
    static std::string UrlDecode(const std::string& url, bool plus_to_space);
};