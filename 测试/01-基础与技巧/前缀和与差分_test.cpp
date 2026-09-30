// 前缀和与差分 的测试与对拍代码
// 模板本体：01-基础与技巧/前缀和与差分.cpp
#include "../../01-基础与技巧/前缀和与差分.cpp"

int main()
{
    srand(20240515);
    n=6,m=6;
    // 自测1：一维前缀和 / 差分 与暴力对拍
    for(int t=1;t<=200;t++)
    {
        n=rand()%20+1;
        for(int i=1;i<=n;i++)a[i]=rand()%21-10;
        build_pre();
        for(int l=1;l<=n;l++)
            for(int r=l;r<=n;r++)
            {
                ll sum=0;
                for(int i=l;i<=r;i++)sum+=a[i];
                if(query_1d(l,r)!=sum)
                {
                    printf("fail 1d pre\n");
                    return 0;
                }
            }
        ll cpy[N];
        for(int i=1;i<=n;i++)cpy[i]=a[i];
        build_dif();
        for(int k=1;k<=5;k++)// 随机若干次区间加
        {
            int l=rand()%n+1,r=rand()%n+1;
            if(l>r)swap(l,r);
            ll v=rand()%11-5;
            add_1d(l,r,v);
            for(int i=l;i<=r;i++)cpy[i]+=v;
        }
        get_1d();
        for(int i=1;i<=n;i++)
            if(a[i]!=cpy[i])
            {
                printf("fail 1d dif\n");
                return 0;
            }
    }
    printf("1d pre/dif self-check OK\n");

    // 自测2：二维前缀和样例 + 二维差分与暴力对拍
    n=m=5;
    memset(s,0,sizeof(s));
    int v[6][6]={{0,0,0,0,0,0},
                 {0,1,2,3,4,5},
                 {0,6,7,8,9,10},
                 {0,11,12,13,14,15},
                 {0,16,17,18,19,20},
                 {0,21,22,23,24,25}};
    ll brute[6][6];
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            s[i][j]=v[i][j],brute[i][j]=v[i][j];
    build_pre2();
    printf("rect(2,2,4,4)=%lld\n",query_2d(2,2,4,4));// 7+8+9+12+13+14+17+18+19=117
    for(int t=1;t<=200;t++)
    {
        memset(d,0,sizeof(d));
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                brute[i][j]=0;
        int k=rand()%6+1;
        for(int z=1;z<=k;z++)
        {
            int x1=rand()%n+1,y1=rand()%m+1,x2=rand()%n+1,y2=rand()%m+1;
            if(x1>x2)swap(x1,x2);
            if(y1>y2)swap(y1,y2);
            ll val=rand()%21-10;
            add_2d(x1,y1,x2,y2,val);
            for(int i=x1;i<=x2;i++)
                for(int j=y1;j<=y2;j++)
                    brute[i][j]+=val;
        }
        get_2d();
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                if(d[i][j]!=brute[i][j])
                {
                    printf("fail 2d dif at (%d,%d)\n",i,j);
                    return 0;
                }
    }
    printf("2d pre/dif self-check OK\n");
    return 0;
}
/* 样例（洛谷 P3397 地毯，用二维差分还原每格覆盖数）
3 2
1 1 2 2
2 2 3 3
输出：
1 1 0
1 2 1
0 1 1
*/
