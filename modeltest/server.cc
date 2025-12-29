#include "../source/Common.hpp"
#include "../source/Socket.hpp"

#include <vector>
#include <string>
#include <cerrno>
#include <unistd.h>

int main()
{
    Socket listen_sock;
    if (!listen_sock.ServerInit(8888, "0.0.0.0"))
    {
        ERR_LOG("ServerInit failed");
        return 1;
    }

    std::vector<int> conns;
    char buf[4096];

    while (true)
    {
        // 1) 非阻塞 accept：可能返回 -1 并且 errno=EAGAIN，这不是错误
        int cfd = listen_sock.Accept();
        if (cfd >= 0)
        {
            INF_LOG("New connection fd=%d", cfd);
            conns.push_back(cfd);
        }
        else
        {
            // Accept() 里你当前是：accept失败就 ERR_LOG 并返回 -1
            // 非阻塞下 accept 没连接时 errno=EAGAIN/EWOULDBLOCK 是正常情况
            if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
            {
                ERR_LOG("accept fatal error, errno=%d", errno);
            }
        }

        // 2) 遍历所有连接，非阻塞 recv
        for (size_t i = 0; i < conns.size(); )
        {
            Socket peer(conns[i]); // 注意：这个临时对象析构会 Close()，所以不能这么写
            // ↑↑↑ 这里是一个大坑：你的 Socket 析构函数会 Close()，
            // 如果你用临时 Socket 包一下 fd，会导致循环结束就把连接关掉！

            // 所以：这里我们直接用 ::recv / ::send，避免 Socket 析构误关 fd
            int fd = conns[i];
            int n = ::recv(fd, buf, sizeof(buf) - 1, 0);

            if (n > 0)
            {
                buf[n] = 0;
                INF_LOG("recv from fd=%d: %s", fd, buf);

                // echo 回去
                (void)::send(fd, buf, n, 0);
                ++i;
            }
            else if (n == 0)
            {
                // 对端关闭
                INF_LOG("client closed fd=%d", fd);
                ::close(fd);
                conns.erase(conns.begin() + i);
            }
            else
            {
                // n < 0
                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                {
                    // 暂时没数据，正常
                    ++i;
                }
                else
                {
                    ERR_LOG("recv error fd=%d errno=%d", fd, errno);
                    ::close(fd);
                    conns.erase(conns.begin() + i);
                }
            }
        }

        // 3) 防止空转 100% CPU（你没用 epoll/poll 的情况下）
        sleep(1); // 1ms
    }

    return 0;
}
