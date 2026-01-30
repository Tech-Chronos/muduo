#include "HttpServer.h"

void HttpServer::Route(const HttpRequest& req, HttpResponse* resp)
{
    if (req._method == "GET" || req._method == "HEAD")
    {
        
    }
    
}

void HttpServer::OnConnected(std::shared_ptr<Connection> con)
{
    con->SetContext(HttpRequest());
}

void HttpServer::OnMessage(std::shared_ptr<Connection> con, Buffer* buf)
{
    // 1. 获取上下文
    HttpContext* context = con->GetContext()->GetValAddr<HttpContext>();
    HttpResponse resp;
    // 2. 处理buf中的数据放到HttpRequest
    context->RecvHttpRequest(buf);
    // 获取req
    HttpRequest req = context->GetRequest();
    // 2.1 如果处理的时候 RespCode 大于 400，说明发生错误，要给客户端发送错误响应
    if (context->GetRespCode() >= 400)
    {
        ERR_LOG("Clinet Request Error");
        MakeErrorResp(req, &resp);
        return;
    }

    // 2.2 当上下文没有处理完成的时候，要继续接受，不能立刻构造resp
    if (context->GetStatus() != RECV_REQ_OVER)
    {
        DBG_LOG("Server Need To Go On Receiving ...");
        return;
    }

    // 3. 将上下文处理完成的req发送给路由表，进行处理请求
    Route();
}