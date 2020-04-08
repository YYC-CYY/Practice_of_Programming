#include<iostream>
#include<string>
#include<cstring>
#include<string.h>

using namespace std;

int dest[30];
int press[40]={0};
int len;
int num=1000;

bool is_fine()
{
    for(int i=1;i<=len;i++)
    {
        if((dest[i]+press[i-1]+press[i]+press[i+1])%2)
        {
            return false;
        }
    }
    return true;
}

void change()
{
    int c=1;
    press[c]=0;
    while(1)
    {
        c=1;
        press[c]++;
        while(press[c]>1)
        {
            press[c]=0;
            c++;
            press[c]++;
        }
        if(press[len+1])
        {
            return;
        }
        if(is_fine())
        {
            int sum=0;
            for(int i=1;i<=len;i++)
            {
                sum+=press[i];
            }
            num=num>sum?sum:num;
        }
    }
}

int main()
{
    char s[40],d[40];
    cin>>s;
    cin.get();
    cin>>d;
    len=strlen(s);
    for(int i=0;i<len;i++)
    {
        dest[i+1]=d[i]-'0';
        dest[i+1]+=s[i]-'0';
        dest[i+1]%=2;
    }
    change();
    if(num==1000)
    {
        cout<<"impossible"<<endl;
    }
    else
    {
        cout<<num;
    }
    
    return 0;
}