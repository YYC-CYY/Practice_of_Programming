#include<iostream>

using namespace std;

int wall[20][20];
int paint[20][20];
int n;//墙的大小
int times=0;

bool painting()
{
    for(int i=1;i<n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            paint[i+1][j]=(wall[i][j]+paint[i][j]+paint[i-1][j]+paint[i][j-1]+paint[i][j+1])%2;
            times+=paint[i+1][j];
        }
    }
    for(int j=1;j<=n;j++)
    {
        if(wall[n][j]!=(paint[n][j-1]+paint[n][j]+paint[n][j+1]+paint[n-1][j])%2)
        {
            times=0;
            return false;
        }
    }
    return true;
}

void enumerate()
{
    int col;
    bool success;
    while(!painting())
    {
        paint[1][1]++;
        col=1;
        while(paint[1][col]>1)
        {
            paint[1][col]=0;
            col++;
            paint[1][col]++;
        }
        if(paint[1][n+1])
        {
            break;
        }
    }
    return;
}

int main()
{
    char s;
    cin>>n;
    for(int i=1;i<=n;i++)
    {
        cin.get();
        for(int j=1;j<=n;j++)
        {
            paint[i][j]=0;
            cin>>s;
            if(s=='y')
            {
                wall[i][j]=0;//不需要
            }
            else
            {
                times++;
                wall[i][j]=1;
            }
        }
    }
    for(int i=0;i<n+2;i++)
    {
        wall[0][i]=wall[n+1][i]=0;
        wall[i][0]=wall[i][n+1]=0;
        paint[0][i]=paint[n+1][i]=0;
        paint[i][0]=paint[i][n+1]=0;
    }
    if(times==0)
    {
        cout<<0<<endl;
        return 0;
    }
    times=0;
    enumerate();
    for(int i=1;i<=n;i++)
    {
        times+=paint[1][i];
    }
    if(times!=0)
    {  
        cout<<times<<endl;
    }
    else
    {
        cout<<"inf"<<endl;
    }
    return 0;
}