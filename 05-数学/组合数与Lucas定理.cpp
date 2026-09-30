// 适用：素数模数的组合计数；n 很大而 p 较小时用 Lucas 按 p 进制拆位。
// 参数：C 的 n 为总数、m 为选取数；阶乘表下标 0..预处理界，查询必须同模数。
// 前提：p 为素数，预处理界<N 且 <p；Lucas 需预处理到 p-1，因此 p 不能超过容量。
// 关键：阶乘逆元从 n 逆推；Lucas 每一位组合数为 0 则整体为 0。
// 易错：不能把固定 mod=1e9+7 直接拿来预处理 p-1；普通 ll 模乘要求 p^2 可存。
// 复杂度：阶乘 O(预处理界+log p)，C 为 O(1)，Lucas O(log_p n)；pf 每层还要枚举余数。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;

const ll mod=1000000007;
ll fact[N],inv_fact[N];

// O(log n)，快速幂
// O(log n)，a 是底数、n 是非负指数、mod 是正模数。
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

// O(N)，阶乘与阶乘逆元预处理
// 注意：必须保证 N < p，否则 fact[N] === 0 (mod p) 会让逆元链整体失效
// O(n+log p)，预处理 0..n 的 fact/inv_fact；p 是素数且 n<p。
void C_init(int n,ll p)
{
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%p;
    inv_fact[n]=qpow(fact[n],p-2,p);
    for(int i=n;i>=1;i--)inv_fact[i-1]=inv_fact[i]*i%p;
}

// O(1)，C(n,m) mod p，要求 0<=n<p 且 p 为素数
// O(1)，求 C(n,m) mod p；n 在预处理范围，m 不合法返回 0。
ll C(ll n,ll m,ll p)
{
    if(m<0||m>n||n<0)return 0;
    return fact[n]*inv_fact[m]%p*inv_fact[n-m]%p;
}

const int C_MAX=200000;// 阶乘预处理的数组上限；调用时传的 n 要同时满足 n < p

// O(log_p n)，Lucas 定理：C(n,m) mod p = C(n%p,m%p)*C(n/p,m/p)，p 为素数
// 要求已 C_init(p-1,p)（即预处理到 p-1）；p 很大且 n<p 时直接算 C(n,m)
// O(log_p n)，n/m 非负，p 为小素数；取 n%p/m%p 为本位组合数再递归。
ll Lucas(ll n,ll m,ll p)
{
    if(m<0||m>n)return 0;
    if(m==0)return 1;
    return C(n%p,m%p,p)*Lucas(n/p,m/p,p)%p;
}

// O(log_p n)，小素数下算 C(n,m) mod p 的对拍基准
// 思路：把 n! 里的 p 因子全部抽走，剩下的部分 mod p 可逆
// k(n) = n/p + k(n/p) 为 v_p(n!)，num/den 为去掉 p 因子后的乘积
// O(log_p n)，返回 n! 中素数 p 的指数，每次 n/=p 累加商。
ll kfac(ll n,ll p)
{
    ll r=0;
    while(n)n/=p,r+=n;
    return r;
}

// O(p*log_p n) 上界，去除 n! 的 p 因子后求余数；完整块用 Wilson 定理变号。
ll pf(ll n,ll p)
{
    ll r=1;
    while(n)
    {
        ll t=n/p;
        // t! 的 (n/p) 段的完整阶乘 mod p
        for(ll i=1;i<=n%p;i++)r=r*i%p;
        if(t&1)r=(p-r)%p;// (p-1)! === -1 (mod p)
        n=t;
    }
    return r;
}

// O(p*log_p n+log p)，小素数参考算法；p 因子指数大于 0 时组合数模 p 为 0。
ll C_naive(ll n,ll m,ll p)
{
    if(m<0||m>n)return 0;
    ll e=kfac(n,p)-kfac(m,p)-kfac(n-m,p);
    if(e>0)return 0;// p 整除组合数
    ll num=pf(n,p);
    ll den=pf(m,p)*pf(n-m,p)%p;
    return num*qpow(den,p-2,p)%p;
}
