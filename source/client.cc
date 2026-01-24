// #include "Common.hpp"
// #include "Socket.hpp"
// #include <unordered_map>
// #include <string>
// #include <unistd.h>

// int main()
// {
//     Socket client;
//     client.ClientInit(8888, "127.0.0.1");

//     std::string mess("I am muduo client");

//     for (int i = 0; i < 5; ++i)
//     {
//         client.SendNonBlock(mess.c_str(), mess.size());
//         sleep(1);
//     }
//     sleep(10);
// }

#include "Socket.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

int main()
{
    Socket client;
    if (!client.ClientInit(8888, "127.0.0.1"))
    {
        std::cerr << "connect failed\n";
        return 1;
    }

    std::cout << "connected. type message, 'quit' to exit.\n";
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "quit")
            break;

        if (!line.empty())
            client.SendNonBlock(line.c_str(), line.size());

        // 试着接收一次回显
        char buf[4096] = {0};
        ssize_t n = client.RecvNonBlock(buf, sizeof(buf));
        if (n > 0)
        {
            std::cout << "echo: " << std::string(buf, n) << "\n";
        }
        usleep(100 * 1000);
    }

    return 0;
}