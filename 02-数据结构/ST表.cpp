#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// ST 表：静态区间最值(RMQ)，n log n 预处理，O(1) 查询，不支持修改
// 查询用两段长度为 2^k 的区间覆盖 [l,r]，可重叠
template<typename T,int NMAX=N,int LOG=18>
struct STmax
{
    T f[NMAX][LOG];
    int lg[NMAX],n;
    void build(int n_,T *src)
    {
        n=n_;
        lg[1]=0;
        for(int i=2;i<=n;i++)lg[i]=lg[i>>1]+1;//预处理 log2 向下取整
        for(int i=1;i<=n;i++)f[i][0]=src[i];
        for(int j=1;(1<<j)<=n;j++)
            for(int i=1;i+(1<<j)-1<=n;i++)
                f[i][j]=max(f[i][j-1],f[i+(1<<(j-1))][j-1]);
    }
    T query(int l,int r)
    {
        int k=lg[r-l+1];
        return max(f[l][k],f[r-(1<<k)+1][k]);
    }
};

template<typename T,int NMAX=N,int LOG=18>
struct STmin
{
    T f[NMAX][LOG];
    int lg[NMAX],n;
    void build(int n_,T *src)
    {
        n=n_;
        lg[1]=0;
        for(int i=2;i<=n;i++)lg[i]=lg[i>>1]+1;
        for(int i=1;i<=n;i++)f[i][0]=src[i];
        for(int j=1;(1<<j)<=n;j++)
            for(int i=1;i+(1<<j)-1<=n;i++)
                f[i][j]=min(f[i][j-1],f[i+(1<<(j-1))][j-1]);
    }
    T query(int l,int r)
    {
        int k=lg[r-l+1];
        return min(f[l][k],f[r-(1<<k)+1][k]);
    }
};

int n;
int a[105];
STmax<int,105,8> stx;
STmin<int,105,8> stn;

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：每个单点区间 + 随机区间，最大/最小都与暴力对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=20;t++)
    {
        n=rnd(1,60);
        for(int i=1;i<=n;i++)a[i]=rnd(-1000,1000);
        stx.build(n,a);
        stn.build(n,a);
        for(int l=1;l<=n;l++)
            for(int r=l;r<=n;r++)
            {
                int mx=-0x3f3f3f3f,mn=0x3f3f3f3f;
                for(int i=l;i<=r;i++)mx=max(mx,a[i]),mn=min(mn,a[i]);
                cnt++;
                if(stx.query(l,r)!=mx||stn.query(l,r)!=mn)bad++;
            }
    }
    printf("ST表(区间最大/最小) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[3,1,4,1,5]，max[1,5]=5 min[2,4]=1 max[3,3]=4
    n=5;
    int s[6]={0,3,1,4,1,5};
    for(int i=1;i<=5;i++)a[i]=s[i];
    stx.build(n,a);
    stn.build(n,a);
    printf("小样例: max[1,5]=%d min[2,4]=%d max[3,3]=%d\n",stx.query(1,5),stn.query(2,4),stx.query(3,3));
    return 0;
}
