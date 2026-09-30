// 适用：模指数、模乘；指数按二进制拆开，幂底每轮平方。
// 参数：n>=0 是指数，mod>0；qmul 的 b 是乘数，qpow 的 n 才是指数。
// 关键：先取模再乘，__int128 版扩宽乘积后取模，ll 版仍需控制中间值。
// 易错：qmul 的 res+a 与 a<<1 都可能在 mod 接近 2^63 时溢出；其 safe 幂也继承此限制。
// 边界：普通 qpow 初始化 res=1，n=0 且 mod=1 会返回 1；负底数也不归一。
// 结论：大模数优先选 qpow128/qmul128；非负指数范围内时间 O(log n)，辅助空间 O(1)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;
const int N=100005;

// O(log n)，快速幂 (a^n) % mod
// O(log n)，求 a^n mod mod；要求 a 非负、模乘不溢出，注意零指数模 1 的约定。
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
// O(log b)，倍增求 a*b mod mod；虽减少乘法，仍要求加法和左移不溢出有符号 ll。
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
// O(1)，先将 a*b 升为 __int128；负乘数的结果可能是负余数，按需归一。
ll qmul128(ll a,ll b,ll mod)
{
    lll t=(lll)a*b;
    return (ll)(t%mod);
}

// O(log n)，快速幂 + 快速乘版，要求 mod*mod 会爆 long long 时用
// O(log n*log mod)，每次模乘调用倍增；名称 safe 不消除 qmul 的加法溢出风险。
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
