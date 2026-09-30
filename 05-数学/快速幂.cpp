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

int main()
{
    // 1) 小数据与暴力逐次相乘对拍
    int bad=0;
    for(int a=0;a<=30;a++)
        for(int e=0;e<=30;e++)
        {
            ll t=1;
            for(int i=1;i<=e;i++)t=t*a%1000000007;
            if(qpow(a,e,1000000007)!=t)bad++;
            if(qpow_safe(a,e,1000000007)!=t)bad++;
        }
    // 2) 大指数，与快速乘版互相对拍
    mt19937_64 rnd(20050101);
    for(int i=1;i<=2000;i++)
    {
        ll m=rnd()%1000000000+2;
        ll a=rnd()%m;
        ll e=rnd()%1000000000000000000ULL;
        if(qpow(a,e,m)!=qpow128(a,e,m))bad++;
        if(qpow_safe(a,e,m)!=qpow128(a,e,m))bad++;
    }
    // 3) 快速乘对大数取模（mod 接近 2^62，普通乘法必溢出）
    ll M=(1LL<<62)-1;
    for(int i=1;i<=2000;i++)
    {
        ll a=rnd()%M,b=rnd()%M;
        if(qmul(a,b,M)!=qmul128(a,b,M))bad++;
    }
    printf("qpow 2^10=1024 -> %lld\n",qpow(2,10,1000000007));
    printf("qpow 2^1000000005 mod 1e9+7 = %lld\n",qpow(2,1000000005,1000000007));
    printf("qmul 1234567890123456789*9876543210987654321 mod 2^62-1 = %lld\n",
           qmul(1234567890123456789LL,9876543210987654321ULL%M,M));
    printf("qmul(7,M-1,M) = %lld, via __int128 = %lld\n",qmul(7,M-1,M),qmul128(7,M-1,M));
    printf("qpow_safe(3,1e18,1e9+7) = %lld\n",qpow_safe(3,1000000000000000000LL,1000000007));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1226 > 2 10 9 -> 2^10 mod 9=7
// 边界：mod=1 时结果恒 0；指数 0 时结果为 1%mod

/*
自测记录：
  1) a,e <= 30 与朴素循环取模对拍；
  2) 随机 mod/a/e（e 到 1e18）与 __int128 版本对拍；
  3) 快速乘在 mod=2^62-1 上与 __int128 对拍。
*/
