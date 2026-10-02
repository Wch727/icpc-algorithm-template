// 适用：模 998244353 的整数卷积；没有 FFT 的浮点舍入误差。
// 下标：多项式系数 0..n-1/0..m-1；数组长度是项数而非次数。
// 长度：n,m>=1；补零长度 len 为 2 的幂，必须 <=N 且整除 mod-1。
// 关键：蝶形把两半的偶奇贡献合并；逆变换使用逆单位根并乘长度逆元。
// 易错：输入须先归一到 [0,mod)；负系数直接传入会留下负余数。
// 复杂度：卷积 O(L log L)、空间 O(L)，L 是补零长度；静态缓冲不能并发共享。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1<<19;

const ll mod=998244353;// 常用 NTT 模数，原根 3，mod-1=119*2^23
const ll g=3;

// O(log n)，快速幂
// O(log n)，a 为底数、n 为非负指数；此处模数约 1e9，ll 模乘安全。
ll qpow(ll a,ll n,ll mod)
{
    ll res=1;
    a%=mod;
    while(n)
    {
        if(n&1)(res*=a)%=mod;
        (a*=a)%=mod;
        n>>=1;
    }
    return res;
}

// O(n log n)，NTT 迭代版 in-place；inv=0 正变换，inv=1 逆变换
// n 必须是 2 的幂且 n | (mod-1)，len 传实际长度
// O(n log n)，a[0..n-1] 原地变换，inv=0/1 表示正/逆；位反转保证迭代合并顺序。
void ntt(ll a[],int n,int inv)
{
    // 位反转置换
    for(int i=1,j=0;i<n;i++)
    {
        int bit=n>>1;
        for(;j&bit;bit>>=1)j^=bit;
        j^=bit;
        if(i<j)swap(a[i],a[j]);
    }
    for(int len=2;len<=n;len<<=1)
    {
        ll w=qpow(g,(mod-1)/len,mod);
        if(inv)w=qpow(w,mod-2,mod);
        for(int i=0;i<n;i+=len)
        {
            ll wn=1;
            for(int k=0;k<len/2;k++)
            {
                ll u=a[i+k],v=a[i+k+len/2]*wn%mod;
                a[i+k]=(u+v)%mod;
                a[i+k+len/2]=(u-v+mod)%mod;
                wn=wn*w%mod;
            }
        }
    }
    if(inv)
    {
        ll ninv=qpow(n,mod-2,mod);
        for(int i=0;i<n;i++)a[i]=a[i]*ninv%mod;
    }
}

// O(n log n)，多项式乘法，结果为 a[0..n-1] * b[0..m-1] 的系数
// O(L log L)，输出 c[0..clen-1]；clen 引用返回 n+m-1，c 至少有这些槽。
void poly_mul(ll a[],int n,ll b[],int m,ll c[],int &clen)
{
    if(n<=0||m<=0){clen=0;return;}
    int len=1;
    while(len<n+m-1)len<<=1;
    assert(len<=N);
    static ll x[N],y[N];
    for(int i=0;i<len;i++)x[i]=(i<n?(a[i]%mod+mod)%mod:0),y[i]=(i<m?(b[i]%mod+mod)%mod:0);
    ntt(x,len,0);
    ntt(y,len,0);
    for(int i=0;i<len;i++)x[i]=x[i]*y[i]%mod;
    ntt(x,len,1);
    clen=n+m-1;
    for(int i=0;i<clen;i++)c[i]=x[i];
}
