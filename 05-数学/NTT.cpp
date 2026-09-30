#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1<<19;

const ll mod=998244353;// 常用 NTT 模数，原根 3，mod-1=119*2^23
const ll g=3;

// O(log n)，快速幂
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
void poly_mul(ll a[],int n,ll b[],int m,ll c[],int &clen)
{
    int len=1;
    while(len<n+m-1)len<<=1;
    static ll x[N],y[N];
    for(int i=0;i<len;i++)x[i]=(i<n?a[i]:0),y[i]=(i<m?b[i]:0);
    ntt(x,len,0);
    ntt(y,len,0);
    for(int i=0;i<len;i++)x[i]=x[i]*y[i]%mod;
    ntt(x,len,1);
    clen=n+m-1;
    for(int i=0;i<clen;i++)c[i]=x[i];
}

// O(n^2)，暴力卷积，对拍基准
void mul_naive(ll a[],int n,ll b[],int m,ll c[],int &clen)
{
    clen=n+m-1;
    for(int i=0;i<clen;i++)c[i]=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            c[i+j]=(c[i+j]+a[i]*b[j])%mod;
}
