#include <iostream>
using namespace std;
// 在此处补充你的代码
template<class T1,class T2,class T3>
class combine
{
public:
    T1 a;T2 b;
    combine(T1 _a,T2 _b):a(_a),b(_b){}
    T3 s;
    T3 operator()(T3 num)
    {
        s=a(num);
        s+=b(num);
        return a(s);
    }

};
int main()
{
    auto Square = [] (double a) { return a * a; };
    auto Inc = [] (double a) { return a + 1; };
    cout << combine<decltype(Square),decltype(Inc),int>(Square,Inc)(3) << endl;
    cout << combine<decltype(Inc),decltype(Square),double>(Inc,Square)(2.5) << endl;

    return 0;
}