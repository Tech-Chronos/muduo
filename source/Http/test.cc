#include "util.h"

int main()
{
    // std::string str = "abc,,,b,.,,.cvbf,fda,";
    // std::string sep = ",";
    // std::vector<std::string> arr;
    // Util tool;
    // tool.Split(str, sep , &arr);
    // //Util::Split(str, sep , &arr);
    // for (auto& e : arr)
    // {
    //     std::cout << e << std::endl;
    // }

    // std::string str;
    // std::string filename = "../Connection.cc";
    // Util::ReadFile(filename, &str);
    // std::cout << str << std::endl;

    // Util::WriteFile("test.txt", &str);
    std::string str = Util::UrlEncode("password=C++", false);
    std::cout << str << std::endl;
}