#include "../source/Common.hpp"
#include "../source/Socket.hpp"

#include <iostream>
#include <string>
#include <unistd.h>
#include <cerrno>

int main()
{
    Socket cli;
    if (!cli.ClientInit(8888, "127.0.0.1"))
    {
        ERR_LOG("ClientInit failed");
        return 1;
    }

    std::string line;
    char buf[4096];

    while (true)
    {
        std::cout << "Input> ";
        if (!std::getline(std::cin, line)) break;

        if (line == "quit") break;

        // 发出去（加个换行方便观察）
        line.push_back('\n');
        int s = cli.Send(line.c_str(), line.size());
        if (s < 0)
        {
            ERR_LOG("send failed");
            break;
        }

        // 非阻塞/阻塞都行：这里简单等回显（轮询）
        while (true)
        {
            int n = cli.Recv(buf, sizeof(buf));
            if (n > 0)
            {
                std::cout << "Echo> " << buf;
                break;
            }
            else if (n == 0)
            {
                // 暂时没数据（非阻塞场景），等一下再收
                usleep(1000);
                continue;
            }
            else
            {
                // -1：对端关闭或错误
                ERR_LOG("server closed or error");
                return 0;
            }
        }
    }

    return 0;
}
