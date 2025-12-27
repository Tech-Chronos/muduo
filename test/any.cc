#include <iostream>

class Any
{
private:
    // 纯虚类
    class Holder
    {
    public:
        virtual ~Holder() {}
        // 获取类型
        virtual std::type_info GetType() = 0;
        // 克隆出一个新的子类对象
        virtual Holder* clone() = 0;
    };

    // 继承Holder类
    template <class T>
    class PlaceHolder : public Holder
    {
    public:
        PlaceHolder(const T& val)
            :_val(val)
        {}

        std::type_info GetType() override
        {
            return typeid(T);
        }

        Holder* clone() override
        {
            return new PlaceHolder(_val);
        }

    public:
        T _val;
    };

private:
    void Swap(Any& other)
    {
        std::swap(_content, other._content);
    }

public:
    // 默认构造函数
    Any() :_content(nullptr) {}

    // 给一个任意类型的val，构建出一个新的Any对象
    template <class T>
    Any(const T& val)
    {
        _content = new PlaceHolder<T>(val);
    }

    // 拷贝构造 
    Any(const Any& other)
    {
        _content = other._content ? other._content->clone() : nullptr;
    }

    // 返回这个val的地址
    template<class T>
    T* Get()
    {
        if (_content)
            assert(typeid(T) == _content->GetType());
        return &(static_cast<PlaceHolder<T>*>(_content))->_val;
    }

    // 赋值重载
    template<class T>
    Any& operator=(const T& _val)
    {
        Any(_val).Swap(*this);
        return *this;
    }

    Any& operator=(Any other)
    {
        Swap(other);
        return *this;
    }

    ~Any()
    {
        delete _content;
    }

private:
    Holder* _content;
};