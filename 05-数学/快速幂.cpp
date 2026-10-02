// 适用：模指数、模乘；指数按二进制拆开，幂底每轮平方。
// 参数：n>=0 是指数，mod>0；qmul 的 b 是乘数，qpow 的 n 才是指数。
// 乘法用 __int128；倍增模乘用比较减法，mod 接近 LLONG_MAX 也不溢出。
// 底数、乘数可负，返回 [0,mod)；零次幂是 1%mod。

// 结论：大模数优先选 qpow128/qmul128；非负指数范围内时间 O(log n)，辅助空间 O(1)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;
const int N=100005;

// O(log n)，快速幂 (a^n) % mod

ll qpow(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    if(a<0)a+=mod;
    while(n)
    {
        if(n&1)res=(__int128)res*a%mod;
        a=(__int128)a*a%mod;
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
        if(b&1)res=res>=mod-a?res-(mod-a):res+a;
        a=a>=mod-a?a-(mod-a):a+a;
        b>>=1;
    }
    return res;
}


ll qmul128(ll a,ll b,ll mod)
{
    lll t=(lll)a*b;
    t%=mod; return (ll)(t<0?t+mod:t);
}

// O(log n)，快速幂 + 快速乘版，要求 mod*mod 会爆 long long 时用
// O(log n*log mod)，仅用无溢出的倍增模乘。
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
// O(log n)，使用宽整数模乘并归一底数；n=0 正确返回 1%mod。
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
