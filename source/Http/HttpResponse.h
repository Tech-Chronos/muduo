#pragma once
#include <string>
#include <unordered_map>

class HttpReaponse
{
public:
    HttpReaponse(int code = 200);
    void Reset();

    void SetHeaders(const std::string& key, const std::string& val);
    bool HasHeaders(const std::string& key);
    std::string GetHeaders(const std::string& key);

    void SetRedirect(const std::string& redirect_uri, int code = 302);

    bool IsKeepAlive();
public:
    int _code;
    std::unordered_map<std::string, std::string> _headers;
    std::string _body;
    bool _redirect_flag;
    std::string _redirect_uri;
};