#include <iostream>
#include <cstring>
using namespace std;
template <int bitNum>
struct MyBitset 
{
	char a[bitNum/8+1];
	MyBitset() { memset(a,0,sizeof(a));};
	void Set(int i,int v) {
		char & c = a[i/8];
		int bp = i % 8;
		if( v ) 
			c |= (1 << bp);//乘以2^bp，将c的第bp+1位变为1，其他位不变
		else 
			c &= ~(1 << bp);//乘以2^bp,第bp+1位为1
	}
// 在此处补充你的代码
    class inner 
    {
    public:
        int num[20];
        int flag;
        inner()
        {
            flag=1;
            memset(num,0,sizeof(int)*20);
        }
        int & operator()(int x,MyBitset& k)
        {
            if(flag)
            {
                for(int i=0;i<=bitNum/8+1;i++)
                {
                    int l=k.a[i];
                    int c=8*i;
                    while(l>0)
                    {
                        num[c]=l%2;
                        c++;
                        l=l/2;
                    }
                }
                flag=0;
            }
            return num[x];
        }
    };

    inner b;
    int& operator[](int x)
    {
        return b(x,*this);
    }

    void Print() {
		for(int i = 0;i < bitNum; ++i) 
			cout << (*this) [i];
		cout << endl;
	}

};

int main()
{
	int n;
	int i,j,k,v;
	while( cin >>  n) {
		MyBitset<20> bs;
		for(int i = 0;i < n; ++i) {
			int t;
			cin >> t;
			bs.Set(t,1);
		}
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		bs[i] = bs[j] = bs[k];
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		(bs[i] = bs[j]) = bs[k];
		bs.Print();
	}
	return 0;
}