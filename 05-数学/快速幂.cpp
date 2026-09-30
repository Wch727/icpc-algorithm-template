#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;
const int N=100005;

// O(log n)，快速幂 (a^n) % mod
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

// O(log n)，快速乘 (a*b) % mod，防 long long 溢出（mod < 2^63）
ll qmul(ll a,ll b,ll mod)
{
    ll res=0;
    a%=mod;
    if(a<0)a+=mod;
    b%=mod;
    if(b<0)b+=mod;
    while(b)
    {
        if(b&1)res=(res+a)%mod;
        a=(a<<1)%mod;
        b>>=1;
    }
    return res;
}

// O(1)，直接用 __int128 的乘法取模，比 qmul 快，可当对拍基准
ll qmul128(ll a,ll b,ll mod)
{
    lll t=(lll)a*b;
    return (ll)(t%mod);
}

// O(log n)，快速幂 + 快速乘版，要求 mod*mod 会爆 long long 时用
ll qpow_safe(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    if(a<0)a+=mod;
    while(n)
    {
        if(n&1)res=qmul(res,a,mod);
        a=qmul(a,a,mod);
        n>>=1;
    }
    return res;
}

// O(log n)，用 __int128 做乘法再取模，跑得比 qmul 快，与 qpow_safe 互为对照
ll qpow128(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    if(a<0)a+=mod;
    while(n)
    {
        if(n&1)res=qmul128(res,a,mod);
        a=qmul128(a,a,mod);
        n>>=1;
    }
    return res;
}
