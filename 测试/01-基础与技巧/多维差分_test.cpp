#include<bits/stdc++.h>
using namespace std;
namespace two {
#include "../../01-基础与技巧/二维差分.cpp"
}
namespace high {
#include "../../01-基础与技巧/高维差分.cpp"
}
int main()
{
    mt19937 rng(20261001);
    for(int t=0;t<50;t++)
    {
        two::n=4,two::m=5;
        long long want[5][6]={0};
        for(int i=1;i<=4;i++)for(int j=1;j<=5;j++)
            two::a[i][j]=want[i][j]=(int)(rng()%21)-10;
        two::build();
        for(int q=0;q<20;q++)
        {
            int x1=rng()%4+1,x2=rng()%4+1,y1=rng()%5+1,y2=rng()%5+1;
            if(x1>x2)swap(x1,x2);
            if(y1>y2)swap(y1,y2);
            int v=(int)(rng()%21)-10;
            two::add(x1,y1,x2,y2,v);
            for(int i=x1;i<=x2;i++)for(int j=y1;j<=y2;j++)want[i][j]+=v;
        }
        two::restore();
        for(int i=1;i<=4;i++)for(int j=1;j<=5;j++)assert(two::a[i][j]==want[i][j]);
    }
    for(int t=0;t<200;t++)
    {
        high::n=rng()%5+1,high::m=rng()%5+1,high::k=rng()%5+1;
        int n=high::n,m=high::m,k=high::k;
        memset(high::d,0,sizeof high::d);
        long long want[6][6][6]={0};
        bool nonzero=t%2;
        if(nonzero)
        {
            for(int x=1;x<=n;x++)for(int y=1;y<=m;y++)for(int z=1;z<=k;z++)
                high::d[x][y][z]=want[x][y][z]=(int)(rng()%21)-10;
            high::build();
        }
        for(int q=0;q<(t%5?30:0);q++)
        {
            int x1=rng()%n+1,x2=rng()%n+1;
            int y1=rng()%m+1,y2=rng()%m+1;
            int z1=rng()%k+1,z2=rng()%k+1;
            if(x1>x2)swap(x1,x2);
            if(y1>y2)swap(y1,y2);
            if(z1>z2)swap(z1,z2);
            if(q==0)x1=y1=z1=1,x2=n,y2=m,z2=k; // 整体修改及右端点+1
            if(q==1)x1=x2=n,y1=y2=m,z1=z2=k; // 最后一个单点
            long long v=q==0?4000000000LL:(int)(rng()%21)-10;
            high::add(x1,y1,z1,x2,y2,z2,v);
            for(int x=x1;x<=x2;x++)for(int y=y1;y<=y2;y++)for(int z=z1;z<=z2;z++)
                want[x][y][z]+=v;
        }
        high::restore();
        for(int x=1;x<=n;x++)for(int y=1;y<=m;y++)for(int z=1;z<=k;z++)
            assert(high::d[x][y][z]==want[x][y][z]);
    }
    puts("二维及三维差分随机对拍：OK");
}
