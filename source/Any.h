#pragma once
#include <iostream>
#include <string>
#include <assert.h>

class Any
{
private:
    class Holder
    {
    public:
        virtual ~Holder() {}
        virtual const std::type_info &GetType() = 0;
        virtual Holder *Clone() = 0;
    };

    template <class T>
    class PlaceHolder : public Holder
    {
    public:
        PlaceHolder(const T &val)
            : _val(val)
        {
        }

        const std::type_info &GetType() override
        {
            return typeid(T);
        }

        Holder *Clone() override
        {
            return new PlaceHolder<T>(_val);
        }

    public:
        T _val;
    };

public:
    // 构造
    Any()
        : _content(nullptr)
    {
    }

    // 构造
    template <class T>
    Any(const T &val)
    {
        _content = new PlaceHolder<T>(val);
    }

    // 拷贝构造
    Any(const Any &other)
    {
        _content = (other._content == nullptr) ? nullptr : other._content->Clone();
    }

    template <class T>
    T *GetValAddr()
    {
        if (_content)
        {
            if (typeid(T) == _content->GetType())
                return &((static_cast<PlaceHolder<T> *>(_content))->_val);
        }
        return nullptr;
    }

    template <class T>
    Any &operator=(const T &val)
    {
        //        if (_content)
        //            delete _content;
        //        _content = new PlaceHolder<T>(val);
        Any(val).Swap(*this);

        return *this;
    }

    Any &operator=(const Any &other)
    {
        if (&other != this)
        {
            Any(other).Swap(*this);
        }
        return *this;
    }

    ~Any()
    {
        delete _content;
    }

private:
    void Swap(Any &other)
    {
        std::swap(_content, other._content);
    }

private:
    Holder *_content;
};
