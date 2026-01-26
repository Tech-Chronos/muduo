// #include "Common.hpp"
// #include "Buffer.hpp"
// #include "Socket.hpp"
// #include "Channel.h"
// #include "Poller.h"
// #include "EventLoop.h"
// #include "TimerWheel.h"
// #include "Connection.h"
// #include "Acceptor.h"
// #include <unordered_map>

// std::unordered_map<uint64_t, std::shared_ptr<Connection>> conns;
// // 线程构造的时候会创建一个EventLoop，并且让这个loop到start中等待事件的到来
// //std::vector<ThreadLoop> threads(2);
// EventLoop base_loop;
// ThreadLoopPool pool(&base_loop);
// //int next_loop = 0;

// void ConnCb(std::shared_ptr<Connection> conn)
// {
//     conns.insert(make_pair(conn->GetConnId(), conn));
// }

// void ServerCloseCb(std::shared_ptr<Connection> conn)
// {
//     conns.erase(conn->GetConnId());
// }

// void MessageDealCb(std::shared_ptr<Connection> conn, Buffer* buf)
// {
//     INF_LOG("%s", buf->GetReadPos());
//     buf->MoveReadOffset(buf->GetReadableDataNum());
// }

// void AcceptHelper(int io_fd)
// {
//     static int con_id = 1;
//     std::shared_ptr<Connection> new_con = std::make_shared<Connection>(con_id, io_fd, pool.NextLoop());
//     new_con->SetConnectedCallback(std::bind(ConnCb, std::placeholders::_1));
//     new_con->SetServerCloseCallback(std::bind(ServerCloseCb, std::placeholders::_1));
//     new_con->SetMessageDealCallback(std::bind(MessageDealCb, std::placeholders::_1, std::placeholders::_2));
//     new_con->Established();

//     new_con->AddInactiveEventRelease(5);
//     ++con_id;
//     // next_loop = (next_loop + 1) % 2;
// }

// int main()
// {
//     // 主线程负责接收新链接
//     std::cout << "主线程 id: " << pthread_self() << std::endl;
//     pool.SetThreadCount(2);
//     pool.CreateThread();

//     Acceptor acc(8888, &base_loop);
//     acc.SetAcceptCallback(std::bind(AcceptHelper, std::placeholders::_1));
//     acc.Listen();

//     while (true)
//         base_loop.Start();
// }

#include "EchoServer.h"
#include "Buffer.hpp"
#include <iostream>

// void OnConnected(std::shared_ptr<Connection> conn)
// {
//     //std::cout << "new conn id=" << conn->GetConnId() << std::endl;
//     INF_LOG("new conn id=%d", conn->GetConnId());
// }

// void OnMessage(std::shared_ptr<Connection> conn, Buffer *buf)
// {
//     // 简单 echo
//     if (buf->GetReadableDataNum() > 0)
//     {
//         std::string data(buf->GetReadPos(), buf->GetReadableDataNum());
//         buf->MoveReadOffset(buf->GetReadableDataNum());
//         conn->Send(const_cast<char *>(data.data()), data.size());
//     }
// }

// void OnClose(std::shared_ptr<Connection> conn)
// {
//     std::cout << "conn closed id=" << conn->GetConnId() << std::endl;
// }

int main()
{
    // server.SetServerCloseCallback(); // 必须，否则不会从 _conns 移除
    // server.SetConnectionCallback(OnConnected);
    // server.SetMessageCallback(OnMessage);
    // server.SetCloseCallback(OnClose);

    // server.SetThreadCountAndCreate(2);
    // server.AcceptorStart();

    EchoServer server(8888);
    server.StartServer();
    return 0;
}