#include<iostream>
#include<string>
#include<stdio.h>
#include<map>
#include<math.h>
#include<iomanip>
#include<iterator>
#include<queue>
#include<string.h>
#include<algorithm>
using namespace std;

struct info
{
    int x,y;
}s[50][500];

int prime_factor(int n)
{
    int num=0;
    int N=n;
    for(int i=2;n!=1;i++)
    {
        if(n%i==0&&i!=N)
        {
            num++;
        }
        while(n%i==0)
        {
            n=n/i;
        }
    }
    return num;
}

bool priority(info a,info b)//true表示a比b更优先
{
    if(a.y!=b.y)
    {
        return a.y>b.y;
    }
    return a.x>b.x;
}

int main()
{
    int num=0;

   // cout<<prime_factor(9888888)<<endl<<prime_factor(9999998)<<endl;


    int n;
    scanf("%d",&num);
    for(int i=1;i<=num;i++)
    {
        for(int j=1;j<=10;j++)
        {
            scanf("%d",&n);
            s[i][j+(i-1)*8].x=n;
            s[i][j+(i-1)*8].y=prime_factor(n);
        }
        sort(s[i]+1,s[i]+i*8+3,priority);
        printf("%d %d\n",s[i][1],s[i][i*8+2]);
        for(int j=2;j<i*8+2;j++)
        {
            s[i+1][j-1].x=s[i][j].x;
            s[i+1][j-1].y=s[i][j].y;
        }
    }
    return 0;
}
