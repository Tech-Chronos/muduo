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
