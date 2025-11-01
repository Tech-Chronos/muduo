#include <iostream>
#include <cstdio>
using namespace std;

void test1()
{
    int a = 0;
    const int *p = &a;
    ///*p = 20;

    int* const q = &a;
    cout << *p << endl;
}

int main()
{
    test1();
    return 0;
}