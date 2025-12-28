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
        virtual const std::type_info& GetType() = 0;
        virtual Holder* Clone() = 0;
    };

    template<class T>
    class PlaceHolder:public Holder
    {
    public:
        PlaceHolder(const T& val)
            :_val(val)
        {}

        const std::type_info& GetType() override
        {
            return typeid(T);
        }

        Holder* Clone() override
        {
            return new PlaceHolder<T>(_val);
        }

    public:
        T _val;
    };
public:
    // 构造
    Any() :_content(nullptr) {}

    // 构造
    template<class T>
    Any(const T& val)
    {
        _content = new PlaceHolder<T>(val);
    }

    // 拷贝构造
    Any (const Any& other)
    {
        _content = (other._content == nullptr) ? nullptr : other._content->Clone();
    }

    template<class T>
    T* GetValAddr()
    {
        if (_content)
        {
            if(typeid(T) == _content->GetType())
                return &((static_cast<PlaceHolder<T>*>(_content))->_val);
        }
        return nullptr;
    }


    template<class T>
    Any& operator=(const T& val)
    {
//        if (_content)
//            delete _content;
//        _content = new PlaceHolder<T>(val);
        Any(val).Swap(*this);

        return *this;
    }

    Any& operator=(const Any& other)
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
    void Swap(Any& other)
    {
        std::swap(_content, other._content);
    }

private:
    Holder* _content;
};

void TestBasic()
{
    Any a(10);
    int* p = a.GetValAddr<int>();
    assert(p && *p == 10);

    Any b(std::string("hello"));
    auto s = b.GetValAddr<std::string>();
    assert(s && *s == "hello");

    std::cout << "[OK] TestBasic\n";
}

void TestTypeMismatch()
{
    Any a(10);

    double* d = a.GetValAddr<double>();
    assert(d == nullptr);   // 必须返回 nullptr，而不是 UB

    std::cout << "[OK] TestTypeMismatch\n";
}

void TestCopyCtor()
{
    Any a(std::string("world"));
    Any b(a);   // 拷贝构造

    auto pa = a.GetValAddr<std::string>();
    auto pb = b.GetValAddr<std::string>();

    assert(pa && pb);
    assert(*pa == *pb);
    assert(pa != pb);   // ❗地址不同，说明是深拷贝

    std::cout << "[OK] TestCopyCtor\n";
}

void TestCopyAssign()
{
    Any a(100);
    Any b(std::string("abc"));

    b = a;

    auto p = b.GetValAddr<int>();
    assert(p && *p == 100);

    std::cout << "[OK] TestCopyAssign\n";
}

void TestOverwrite()
{
    Any a(1);

    for (int i = 0; i < 1000; ++i)
    {
        a = std::string("iteration");
        a = i;
    }

    auto p = a.GetValAddr<int>();
    assert(p && *p == 999);

    std::cout << "[OK] TestOverwrite\n";
}

void TestEmpty()
{
    Any a;

    assert(a.GetValAddr<int>() == nullptr);
    assert(a.GetValAddr<std::string>() == nullptr);

    std::cout << "[OK] TestEmpty\n";
}

struct Point
{
    int x, y;
};

void TestUserType()
{
    Any a(Point{1, 2});

    auto p = a.GetValAddr<Point>();
    assert(p);
    assert(p->x == 1 && p->y == 2);

    std::cout << "[OK] TestUserType\n";
}

int main()
{
    TestBasic();
    TestTypeMismatch();
    TestCopyCtor();
    TestCopyAssign();
    TestOverwrite();
    TestEmpty();
    TestUserType();

    std::cout << "🎉 All Any interface tests passed!\n";
    return 0;
}
