#pragma once
#include "Connection.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include <unordered_map>

using ServerCallBack = std::function<void(int)>;
using Functor = std::function<void()>;

class TcpServer
{
private:
    void AcceptCallback(int io_fd);
    void RemoveConnect(std::shared_ptr<Connection> conn);
public:
    TcpServer(int port);

    void AcceptorStart();

    void SetThreadCountAndCreate(int count);

    void EnableIncativeEventRelease(int timeout);


    void SetConnectionCallback(const ConnectedCallBack& cb);
    void SetMessageCallback(const MessageCallBack& cb);
    void SetCloseCallback(const CloseCallBack& cb);
    void SetServerCloseCallback();
    void SetAnyEventCallback(const AnyEventCallBack& cb);

    void RunAfter(int timeout, Functor func);
private:
    uint64_t _id; // 每个链接的id
    uint64_t _timer_id; //定时任务id
    int _thread_count; // 外部可以控制线程的数量
    int _port; // 服务器监听的端口

    bool _enable_inactive_event_release; // 是否开启非活跃事件的释放功能
    int _timeout; // 如果设置了释放功能，多长时间没有消息就释放

    EventLoop* _base_loop; // 主线程的loop，只负责接收新链接
    Acceptor _acceptor; // 这个acceptor属于baseloop
    std::unordered_map<uint64_t, std::shared_ptr<Connection>> _conns;// 服务器管理的链接
    ThreadLoopPool _pool; // 线程池

    ConnectedCallBack _connect_cb; // 新链接来的时候外部设置的链接回调函数
    MessageCallBack _mess_cb;  // 新链接来的时候外部设置的信息处理函数
    CloseCallBack _close_cb;  // 新链接来的时候外部设置的关闭链接函数
    CloseCallBack _server_cb;
    AnyEventCallBack _any_event_cb;  // 新链接来的时候外部设置的任意事件处理函数
};