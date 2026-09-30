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
