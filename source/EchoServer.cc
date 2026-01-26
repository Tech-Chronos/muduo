#include "EchoServer.h"

EchoServer::EchoServer(int port)
    : _server(port)
{
    _server.SetThreadCountAndCreate(3);
    _server.SetConnectionCallback(std::bind(&EchoServer::OnConnected, this, std::placeholders::_1));
    _server.SetMessageCallback(std::bind(&EchoServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
    _server.SetCloseCallback(std::bind(&EchoServer::OnClose, this, std::placeholders::_1));

}

void EchoServer::OnConnected(std::shared_ptr<Connection> conn)
{
    INF_LOG("new conn id=%d", conn->GetConnId());
}

void EchoServer::OnMessage(std::shared_ptr<Connection> conn, Buffer *buf)
{
    if (buf->GetReadableDataNum() > 0)
    {
        std::string data(buf->GetReadPos(), buf->GetReadableDataNum());
        buf->MoveReadOffset(buf->GetReadableDataNum());
        conn->Send(const_cast<char *>(data.data()), data.size());
    }
}

void EchoServer::OnClose(std::shared_ptr<Connection> conn)
{
    std::cout << "conn closed id=" << conn->GetConnId() << std::endl;
}

void EchoServer::StartServer()
{
    _server.AcceptorStart();
}