#include "util.h"

// "abc,b,cvbf,fda"
int Util::Split(const std::string &src, const std::string &sep, std::vector<std::string> *arr)
{
    int offset = 0;
    while (offset < src.size())
    {
        int pos = src.find(sep, offset);
        if (pos == std::string::npos)
        {
            arr->push_back(src.substr(offset));
            break;
        }
        else
        {
            if (offset == pos) 
            {
                offset = pos + sep.size();
                continue;
            }
            arr->push_back(src.substr(offset, pos - offset));
            offset = pos + sep.size();
        }
    }
    return arr->size();
}


bool Util::ReadFile(const std::string& filename, std::string* buf)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs)
    {
        ERR_LOG("OPEN FILE %s ERROR!", filename.c_str());
        return false;
    }
    ifs.seekg(0, ifs.end);
    int filesize = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    buf->resize(filesize);

    ifs.read(&(*buf)[0], filesize);
    if (!ifs)
    {
        ERR_LOG("READ FILE %s ERROR", filename.c_str());
        ifs.close();
        return false;
    }
    ifs.close();
    return true;
}

bool Util::WriteFile(const std::string& filename, std::string* buf)
{
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
    if (!ofs)
    {
        ERR_LOG("OPEN FILE %s ERROR!", filename.c_str());
        return false;
    }
    ofs.write(buf->c_str(), buf->size());
    if (ofs.good() == false)
    {
        ERR_LOG("WRITE FILE %s ERROR!", filename.c_str());
        ofs.close();
        return false;
    }
    ofs.close();
    return true;
}

std::string Util::UrlEncode(const std::string& url, bool space_to_plus)
{
    std::string res;
    for (auto& e: url)
    {
        // 如果是 . - _ ~，或者是数字、字母就不用转化
        if (e == '.' || e == '-' || e == '_' || e == '~' || isalnum(e))
        {
            res += e;
            continue;
        }
        // 如果需要将空格转化成 + 号
        if (e == ' ' && space_to_plus)
        {
            res += '+';
            continue;
        } 

        // 剩下的都要转化
        char tmp[4] = { 0 };
        snprintf(tmp, 4, "%%%X", e);
        res += tmp;
    }
    return res;
}

int Util::HexToDecimal(char hex)
{
    if (hex > '0' && hex < '9') return hex - '0';
    if (hex > 'A' && hex < 'Z') return hex - 'A' + 10;
    if (hex > 'a' && hex < 'z') return hex - 'a' + 10;
    return -1;
}

std::string Util::UrlDecode(const std::string& url, bool plus_to_space)
{
    std::string ret;
    for (int i = 0; i < url.size(); ++i)
    {
        if (url[i] == '%')
        {
            int left = HexToDecimal(url[i + 1]);
            int right = HexToDecimal(url[i + 2]);
            
        }

    }
}



