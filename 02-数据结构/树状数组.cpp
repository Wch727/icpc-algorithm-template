// @code common
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int M=1005;//二维树状数组的默认边长

// @code bit
template<typename T,int NMAX=N>
struct BIT
{
    T tr[NMAX];
    int n;
    void init(int n_)//下标 1..n_
    {
        n=n_;
        for(int i=1;i<=n;i++)tr[i]=0;
    }
    void add(int x,T v)
    {
        for(;x<=n;x+=x&-x)tr[x]+=v;
    }
    T sum(int x)//前缀和 [1,x]
    {
        T s=0;
        for(;x>0;x-=x&-x)s+=tr[x];
        return s;
    }
    T query(int l,int r)//区间和 [l,r]
    {
        if(l>r)return 0;
        return sum(r)-sum(l-1);
    }
    // 前缀二分；使用条件见对应说明。
    int kth(T k)
    {
        int p=0,lg=1;
        T s=0;
        while((lg<<1)<=n)lg<<=1;//不超过 n 的最大 2 的幂
        for(int j=lg;j;j>>=1)
            if(p+j<=n&&s+tr[p+j]<k)s+=tr[p+j],p+=j;
        return p+1;
    }
};

// @code difference
template<typename T,int NMAX=N>
struct DiffBIT
{
    BIT<T,NMAX> c;
    int n;
    void init(int n_)
    {
        n=n_,c.init(n_);
    }
    void update(int l,int r,T v)
    {
        c.add(l,v);
        if(r+1<=n)c.add(r+1,-v);//边界：r=n 时右端点不越界
    }
    T query(int x)
    {
        return c.sum(x);
    }
};

// @code bit2d
template<typename T,int NMAX=M>
struct BIT2D
{
    T tr[NMAX][NMAX];
    int n,m;
    void init(int n_,int m_)
    {
        n=n_,m=m_;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)tr[i][j]=0;
    }
    void add(int x,int y,T v)
    {
        for(int i=x;i<=n;i+=i&-i)
            for(int j=y;j<=m;j+=j&-j)tr[i][j]+=v;
    }
    T sum(int x,int y)//左上角前缀和
    {
        T s=0;
        for(int i=x;i>0;i-=i&-i)
            for(int j=y;j>0;j-=j&-j)s+=tr[i][j];
        return s;
    }
    T query(int x1,int y1,int x2,int y2)//子矩阵和
    {
        return sum(x2,y2)-sum(x1-1,y2)-sum(x2,y1-1)+sum(x1-1,y1-1);
    }
};

// @code inversions
int n;
int a[N],tmp[N];
BIT<int> bit_inv;

ll calc_inv(int len)
{
    for(int i=1;i<=len;i++)tmp[i]=a[i];
    sort(tmp+1,tmp+len+1);
    int tot=unique(tmp+1,tmp+len+1)-tmp-1;
    bit_inv.init(tot);
    ll ans=0;
    for(int i=1;i<=len;i++)
    {
        int rk=lower_bound(tmp+1,tmp+tot+1,a[i])-tmp;
        ans+=i-1-bit_inv.sum(rk);//前面比 a[i] 严格大的个数
        bit_inv.add(rk,1);
    }
    return ans;
}
