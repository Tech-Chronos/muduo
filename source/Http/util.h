#pragma once
#include "../EchoServer.h"

class Util
{
public:
    static int Split(const std::string& src, const std::string& sep, std::vector<std::string>* arr);
};