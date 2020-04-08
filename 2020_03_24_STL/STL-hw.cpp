#include <iostream>
#include <string>
#include<stdio.h>
#include <map>
#include<math.h>
#include<iomanip>
#include <iterator>
#include<queue>
#include<string.h>
#include <algorithm>
using namespace std;

int prime_factor(int n);
bool priority(int a,int b);
int not_prime[3170]={0};//质数集合

int main()
{
    int num=0;
    not_prime[0]=not_prime[1]=1;
    for(int i=2;i<3170;++i)
    {
        if(not_prime[i]==0)
        {
            for(int k=2;k*i<3170;++k)
            {
                not_prime[k*i]=1;
            }
        }
    }

    cout<<prime_factor(9888888)<<endl<<prime_factor(9999998)<<endl;


    vector<int> my_priority;
    int n;
    scanf("%d",&num);
    while(num--)
    {
        for(int i=0;i<10;++i)
        {
            scanf("%d",&n);
            my_priority.push_back(n);
        }
        sort(my_priority.begin(),my_priority.end(),priority);
        
        auto big=my_priority.end()-1;
        auto small=my_priority.begin();
        printf("%d %d\n",*big,*small);
        my_priority.erase(big);
        my_priority.erase(small);
    }
    return 0;
}


int prime_factor(int n)
{
    int num=0;
    
    for(int i=2;i<3170;i++)
    {
        if(!not_prime[i]&&n%i==0)
        {
            num++;
            while(n%i==0)
            {
                n=n/i;
            }
            if(n==1)
            {
                break;
            }
        }
    }

    if(n!=1)
    {
        return num+1;
    }
    return num;
}

bool priority(const int a,const int b)//true表示b比a更优先
{
    if(prime_factor(a)<prime_factor(b))
    {
        return true;
    }
    else if(prime_factor(a)>prime_factor(b))
    {
        return false;
    }
    else
    {
        if(a<b)
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }
}
/*
10   2
 7   0
66   3
 4   1
 5   0
30   3
91   2
100  2
 8   1
 9   1
*/