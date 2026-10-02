// 适用：大整数素性判断和质因数分解；实际接口是正的有符号 ll。
// 参数：n<=LLONG_MAX；分解入口 factor 要 n>=1，pollard_rho 只接受合数。
// 关键：n-1=d*2^s，强伪素数测试不断平方；固定七个底数覆盖接口可表示范围。
// 随机：Rho 返回的是任意非平凡因子，不保证素数，继续递归分解。
// 模乘和 Rho 的平方加常数均用 __int128；仅支持正的 signed ll。
// 复杂度：七底数素性判定 O(log n) 次模乘；Rho 期望约 O(n^(1/4))，无确定时间上界。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// O(log^3 n)，Miller-Rabin 素性判定，确定性基组覆盖 64 位
// O(1)，计算非负 a*b mod mod；mod>0，先扩为 __int128。
ll qmul(ll a,ll b,ll mod)
{
    return (ll)((lll)a*b%mod);
}

// O(log n)，模 mod 快速幂；n 是非负指数。
ll qpow(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    while(n)
    {
        if(n&1)res=qmul(res,a,mod);
        a=qmul(a,a,mod);
        n>>=1;
    }
    return res;
}

// O(log n) 次宽整数模乘，返回 n 是否为素数；底数为 n 的倍数时跳过。
bool miller_rabin(ll n)
{
    if(n<2)return false;
    if(n==2||n==3)return true;
    if(n%2==0)return false;
    // 写成 n-1 = d*2^s
    ll d=n-1;
    int s=0;
    while(!(d&1))d>>=1,s++;
    ll base[7]={2,325,9375,28178,450775,9780504,1795265022};
    for(int i=0;i<7;i++)
    {
        ll a=base[i]%n;
        if(a==0)continue;
        ll x=qpow(a,d,n);
        if(x==1||x==n-1)continue;
        bool ok=false;
        for(int j=1;j<s;j++)
        {
            x=qmul(x,x,n);
            if(x==n-1){ok=true;break;}
        }
        if(!ok)return false;// 一定是合数
    }
    return true;
}

// O(n^(1/4))，Pollard-Rho 找 n 的一个非平凡因子，n 必须是合数
// 期望约 O(n^(1/4))，找合数 n 的因子；批量积减少 gcd 次数，d==n 表示本轮失败。
ll pollard_rho(ll n)
{
    if(n%2==0)return 2;
    if(n%3==0)return 3;
    while(true)
    {
        static mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
        ll c=uniform_int_distribution<ll>(1,n-1)(rnd);
        ll x=uniform_int_distribution<ll>(1,n-1)(rnd),y=x,d=1;
        // 倍增步长 + gcd 批量化
        ll q=1;
        for(ll len=1;d==1;len<<=1)
        {
            ll tx=x;
            for(ll i=1;i<=len;i++)
            {
                x=((lll)qmul(x,x,n)+c)%n;
                q=qmul(q,abs(x-y),n);
                if(i%127==0)
                {
                    d=__gcd(q,n);
                    if(d>1)break;
                }
            }
            if(d==1)d=__gcd(q,n);
            y=x;
            if(d==n){x=tx;break;}// 失败重来
        }
        if(d>1&&d<n)return d;
    }
}

// O(n^(1/4) log n)，递归分解出全部素因子（不排序、含重数）
// 随机递归分解，v 为追加输出而非覆盖；n=1 不追加，重复因子保留，结果需自行排序。
void factor(ll n,vector<ll> &v)
{
    if(n==1)return;
    if(miller_rabin(n)){v.push_back(n);return;}
    ll d=pollard_rho(n);
    // 随机递归分解，v 为追加输出而非覆盖；n=1 不追加，重复因子保留，结果需自行排序。
    factor(d,v);
    // 随机递归分解，v 为追加输出而非覆盖；n=1 不追加，重复因子保留，结果需自行排序。
    factor(n/d,v);
}
