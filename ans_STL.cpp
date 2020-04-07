#include<bits/stdc++.h>
using namespace std;
struct node{
	int x,y;
}s[50][300];
int cal(int a){
	int sum=0,t=a;
	for(int k=2;a!=1;k++){
		if(a%k==0&&k!=t)sum++;
		while(a%k==0)a/=k;
	}
	return sum;
}
int cmp(node q,node p){
	if(q.y!=p.y)return q.y>p.y;
	return q.x>p.x;
}
int main(){
	int n,a,i,j;
	scanf("%d",&n);
	for(i=1;i<=n;i++){
		for(j=1;j<=10;j++){
			scanf("%d",&a);
			s[i][j+(i-1)*8].x=a;
			s[i][j+(i-1)*8].y=cal(a);
		}
		sort(s[i]+1,s[i]+i*8+3,cmp);
		printf("%d %d\n",s[i][1],s[i][i*8+2]);
		for(j=2;j<i*8+2;j++){
			s[i+1][j-1].x=s[i][j].x;
			s[i+1][j-1].y=s[i][j].y;
		}
	}
	return 0;
}