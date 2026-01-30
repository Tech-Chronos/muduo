#pragma once
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpContext.h"
#include "../TcpServer.h"

using HttpReqHandle = std::function<void(const HttpRequest&, HttpResponse*)>;

class HttpServer
{
public:
    void FileHandler(const std::string& filename);
    void Route(const HttpRequest& req, HttpResponse* resp);
    void OnConnected(std::shared_ptr<Connection> con);
    void OnMessage(std::shared_ptr<Connection> con, Buffer* buf);
    

private:
    std::unordered_map<std::string, HttpReqHandle> _get_route;
    std::unordered_map<std::string, HttpReqHandle> _put_route;
    std::unordered_map<std::string, HttpReqHandle> _post_route;
    std::unordered_map<std::string, HttpReqHandle> _delete_route;
    TcpServer _tcp_server;
    std::string _www_root; // 网络资源根目录
};  