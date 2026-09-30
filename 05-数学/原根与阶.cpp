#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
const int N=1000006;

int n;
int prime[N],cnt;// 素数表（顺便当最小质因子筛用）
int spf[N];// 最小质因子

// O(n)，线性筛，顺便记最小质因子
void get_prime(int n)
{
    cnt=0;
    for(int i=2;i<=n;i++)
    {
        if(!spf[i])prime[++cnt]=i,spf[i]=i;// i 是素数
        for(int j=1;j<=cnt&&(ll)i*prime[j]<=n;j++)
        {
            spf[i*prime[j]]=prime[j];
            if(i%prime[j]==0)break;// 只被最小质因子筛一次
        }
    }
}

// O(log n)，快速幂（模乘用 __int128 防溢出）
ll qpow(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    while(n)
    {
        if(n&1)res=(ll)((lll)res*a%mod);
        a=(ll)((lll)a*a%mod);
        n>>=1;
    }
    return res;
}

// O(sqrt n)，试除分解质因数（不要求 n 是素数），返回去重后的素因子
vector<ll> factor(ll n)
{
    vector<ll> v;
    for(ll i=2;i*i<=n;i++)
        if(n%i==0)
        {
            v.push_back(i);
            while(n%i==0)n/=i;
        }
    if(n>1)v.push_back(n);
    return v;
}

// O(sqrt n)，单个数的欧拉函数，顺序筛 phi(p) 时要用（p 不一定是素数）
ll phi_of(ll n)
{
    ll r=n;
    for(ll i=2;i*i<=n;i++)
        if(n%i==0)
        {
            r=r/i*(i-1);
            while(n%i==0)n/=i;
        }
    if(n>1)r=r/n*(n-1);
    return r;
}

// O(sqrt p log p)，求 a 模 p 的阶，要求 gcd(a,p)=1（p 可以是合数）
// 做法：ord | phi(p)，枚举 phi(p) 的素因子逐个除掉，除掉一个因子后接着试同一个因子
ll get_order(ll a,ll p)
{
    if(__gcd(a,p)!=1)return -1;// 不互素没有阶
    if(p==1)return 1;
    ll ph=phi_of(p);
    vector<ll> f=factor(ph);
    ll ord=ph;
    for(int i=0;i<(int)f.size();i++)
        while(ord%f[i]==0&&qpow(a,ord/f[i],p)==1)ord/=f[i];// 能除就除
    return ord;
}

// O(sqrt(p) + log^2 p)，求模 p 的最小原根（p 为素数，p=2 特判返回 1）
ll get_root(ll p)
{
    if(p==2)return 1;
    vector<ll> f=factor(p-1);
    for(ll g=2;g<p;g++)
    {
        bool ok=true;
        for(int i=0;i<(int)f.size();i++)
            if(qpow(g,(p-1)/f[i],p)==1)// 只要有一个为 1 就不是原根
            {
                ok=false;
                break;
            }
        if(ok)return g;
    }
    return -1;// p 不是素数时可能走到这里
}

// O(sqrt(p) log p)，原根判定：g 是模 p 原根 <=> ord_p(g)=phi(p)
// 对素数 p 就是 ord=p-1；合数模数下用 phi(p)，只有 2,4,p^k,2p^k 才有原根
bool is_root(ll g,ll p)
{
    if(__gcd(g,p)!=1)return false;
    if(p==1)return true;
    return get_order(g,p)==phi_of(p);
}

// O(p sqrt(p) log p)，求任意模数 n 的最小原根（n 为 2,4,p^k,2p^k 时才有解），无解返回 -1
ll get_root_general(ll n)
{
    if(n==1)return 0;
    for(ll g=1;g<n;g++)
        if(is_root(g,n))return g;
    return -1;
}

// O(p)，暴力求阶，对拍用
ll order_naive(ll a,ll p)
{
    if(__gcd(a,p)!=1)return -1;
    ll cur=1%p;
    for(ll k=1;k<=p;k++)
    {
        cur=cur*a%p;
        if(cur==1)return k;
    }
    return -1;
}

// O(p)，暴力判定原根：g 的幂跑满一个完整循环才回到 1（即 ord=phi(p)）
bool is_root_naive(ll g,ll p)
{
    if(__gcd(g,p)!=1)return false;
    static int seen[N];
    for(int i=1;i<p;i++)seen[i]=0;
    ll cur=1%p;
    for(int k=1;k<p;k++)
    {
        cur=cur*g%p;
        if(seen[cur])return false;
        seen[cur]=1;
    }
    return true;
}

// O(p)，暴力判定任意模数的原根，对拍用
bool is_root_naive_general(ll g,ll n)
{
    if(n==1)return true;
    if(__gcd(g,n)!=1)return false;
    ll lim=phi_of(n);
    static int seen[N];
    for(int i=0;i<n;i++)seen[i]=0;
    ll cur=1%n;
    for(ll k=1;k<=lim;k++)
    {
        cur=cur*g%n;
        if(seen[cur])return false;
        seen[cur]=1;
    }
    return true;
}
