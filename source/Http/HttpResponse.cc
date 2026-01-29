#include "HttpResponse.h"

HttpResponse::HttpResponse(int code)
    : _code(code)
    , _redirect_flag(false)
{}

void HttpResponse::Reset()
{
    _code = 200;
    _headers.clear();
    _body.clear();
    _redirect_flag = false;
    _redirect_uri.clear();
}

void HttpResponse::SetHeaders(const std::string& key, const std::string& val)
{
    _headers.insert(std::make_pair(key, val));
}

bool HttpResponse::HasHeaders(const std::string& key)
{
    auto it = _headers.find(key);
    if (it == _headers.end()) return false;
    return true;
}

std::string HttpResponse::GetHeaders(const std::string& key)
{
    if (HasHeaders(key))
    {
        return _headers[key];
    }
    return "";
}

void HttpResponse::SetRedirect(const std::string &redirect_uri, int code)
{
    _code = code;
    _redirect_flag = true;
    _redirect_uri = redirect_uri;
}

bool HttpResponse::IsKeepAlive()
{
    if (HasHeaders("Connection") && GetHeaders("Connection") == "keep-alive")
        return true;
    return false;
}