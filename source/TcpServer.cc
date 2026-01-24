#include "TcpServer.h"

void TcpServer::AcceptCallback(int io_fd)
{
    _id++;
    std::shared_ptr<Connection> new_con = std::make_shared<Connection>(_id, io_fd, _pool.NextLoop());
    new_con->SetConnectedCallback(_connect_cb);
    new_con->SetMessageDealCallback(_mess_cb);
    new_con->SetCloseCallback(_close_cb);
    new_con->SetServerCloseCallback(_server_cb);
    new_con->SetAnyEventCallback(_any_event_cb);

    new_con->Established();
    if (_enable_inactive_event_release)
        new_con->AddInactiveEventRelease(_timeout);

    _conns[_id] = new_con;
}

// 防止其他线程修改这个conns，只允许主线程修改
void TcpServer::RemoveConnect(std::shared_ptr<Connection> conn)
{
    _base_loop->RunInLoop([this, conn]
    {
        auto it = _conns.find(conn->GetConnId());
        if (it != _conns.end())  _conns.erase(it); 
    });
}

TcpServer::TcpServer(int port)
    : _id(0), _timer_id(0), _thread_count(0), _port(port), _base_loop(new EventLoop()), _enable_inactive_event_release(false), _acceptor(port, _base_loop), _pool(_base_loop)
{
    _acceptor.SetAcceptCallback(std::bind(&TcpServer::AcceptCallback, this, std::placeholders::_1));
    _acceptor.Listen();
    SetServerCloseCallback();
}

void TcpServer::SetThreadCountAndCreate(int count)
{
    _thread_count = count;
    _pool.SetThreadCount(count);
    _pool.CreateThread();
}

void TcpServer::EnableIncativeEventRelease(int timeout)
{
    _enable_inactive_event_release = true;
    _timeout = timeout;
}

void TcpServer::AcceptorStart()
{
    _base_loop->Start();
}

void TcpServer::SetConnectionCallback(const ConnectedCallBack &cb)
{
    _connect_cb = cb;
}

void TcpServer::SetMessageCallback(const MessageCallBack &cb)
{
    _mess_cb = cb;
}

void TcpServer::SetCloseCallback(const CloseCallBack &cb)
{
    _close_cb = cb;
}

void TcpServer::SetServerCloseCallback()
{
    _server_cb = std::bind(&TcpServer::RemoveConnect, this, std::placeholders::_1);
}

void TcpServer::SetAnyEventCallback(const AnyEventCallBack &cb)
{
    _any_event_cb = cb;
}

// 增加定时任务
void TcpServer::RunAfter(int timeout, Functor func)
{
    _base_loop->RunInLoop([this, timeout, func] 
    {
        uint64_t tid = _timer_id++;
        _base_loop->AddTimer(tid, timeout, func);
    });
}
