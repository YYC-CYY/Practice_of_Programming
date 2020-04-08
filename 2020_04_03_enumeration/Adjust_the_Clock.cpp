#include<iostream>
#include<map>
#include<vector>

using namespace std;

int main()
{
    vector<int> adjust;
    int time[10];
    int change[10];
    int num=10000;
    for(int i=1;i<=9;i++)
    {
        cin>>time[i];
    }
    int sum=0;
    for(change[1]=0;change[1]<4;++change[1])
    {
        for(change[2]=0;change[2]<4;++change[2])
        {
            for(change[3]=0;change[3]<4;++change[3])
            {
                sum=0;

                change[4]=(4-(time[1]+change[1]+change[2])%4)%4;
                change[5]=(4-(time[2]+change[1]+change[2]+change[3])%4)%4;
                change[6]=(4-(time[3]+change[2]+change[3])%4)%4;
                change[7]=(4-(time[4]+change[1]+change[4]+change[5])%4)%4;
                change[9]=(4-(time[6]+change[3]+change[5]+change[6])%4)%4;
                change[8]=(4-(time[8]+change[5]+change[7]+change[9])%4)%4;

                sum+=(time[1]+change[1]+change[2]+change[4])%4;
                sum+=(time[2]+change[1]+change[2]+change[3]+change[5])%4;
                sum+=(time[3]+change[2]+change[3]+change[6])%4;
                sum+=(time[4]+change[1]+change[4]+change[5]+change[7])%4;
                sum+=(time[5]+change[1]+change[3]+change[5]+change[7]+change[9])%4;
                sum+=(time[6]+change[3]+change[5]+change[6]+change[9])%4;
                sum+=(time[7]+change[4]+change[7]+change[8])%4;
                sum+=(time[8]+change[5]+change[7]+change[8]+change[9])%4;
                sum+=(time[9]+change[6]+change[8]+change[9])%4;
                if(sum==0)
                {
                    for(int i=1;i<=9;i++)
                    {
                        while(change[i]--)
                        {
                            cout<<i<<" ";
                        }
                    }
                    return 0;
                    /*if(num>change[1]+change[2]+change[3]+change[4]+change[5]+change[6]+change[7]+change[8]+change[9])
                    {
                        adjust.clear();
                        for(int i=1;i<=9;i++)
                        {
                            while(change[i]--)
                            {
                                adjust.push_back(i);
                            }
                        }
                    }*/
                }
            }
        }
    }
    /*vector<int>::iterator it;
    it=adjust.begin();
    for(;it<adjust.end();it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;*/
    return 0;
}
/*
typedef pair<int,int> pos;
vector<pos> change[10];
    change[1].emplace_back(1,1);
    change[1].emplace_back(1,2);
    change[1].emplace_back(2,1);
    change[1].emplace_back(2,2);

    change[2].emplace_back(1,1);
    change[2].emplace_back(1,2);
    change[2].emplace_back(1,3);

    change[3].emplace_back(1,2);
    change[3].emplace_back(1,3);
    change[3].emplace_back(2,2);
    change[3].emplace_back(2,3);

    change[4].emplace_back(1,1);
    change[4].emplace_back(2,1);
    change[4].emplace_back(3,1);

    change[5].emplace_back(1,2);
    change[5].emplace_back(2,1);
    change[5].emplace_back(2,2);
    change[5].emplace_back(2,3);
    change[5].emplace_back(3,2);

    change[6].emplace_back(1,3);
    change[6].emplace_back(2,3);
    change[6].emplace_back(3,3);

    change[7].emplace_back(2,1);
    change[7].emplace_back(2,2);
    change[7].emplace_back(3,1);
    change[7].emplace_back(3,2);

    change[8].emplace_back(3,1);
    change[8].emplace_back(3,2);
    change[8].emplace_back(3,3);

    change[9].emplace_back(2,2);
    change[9].emplace_back(2,3);
    change[9].emplace_back(3,2);
    change[9].emplace_back(3,3);    

    int time[4][4];
    for(int i=1;i<4;i++)
    {
        for(int j=1;j<4;j++)
        {
            cin>>time[i][j];
        }
    }
    */