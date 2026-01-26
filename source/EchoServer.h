#pragma once

#include "TcpServer.h"
class EchoServer
{
private:
    void OnConnected(std::shared_ptr<Connection> conn);

    void OnMessage(std::shared_ptr<Connection> conn, Buffer *buf);

    void OnClose(std::shared_ptr<Connection> conn);
public:
    EchoServer(int port);

    void StartServer();

private:
    TcpServer _server;
};