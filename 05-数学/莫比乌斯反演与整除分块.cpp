#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000006;

int n;
int prime[N],cnt;// prime[1..cnt] 存的素数
bool vis[N];// 合数标记
int mu[N];// 莫比乌斯函数
ll sum_mu[N];// mu 的前缀和（杜教筛 / 整除分块都要用）

// O(n)，线性筛出 mu 并求前缀和
void get_mu(int n)
{
    cnt=0;
    mu[1]=1;
    for(int i=2;i<=n;i++)
    {
        if(!vis[i])
        {
            prime[++cnt]=i;
            mu[i]=-1;// 素数有 1 个质因子
        }
        for(int j=1;j<=cnt&&(ll)i*prime[j]<=n;j++)
        {
            int t=i*prime[j];
            vis[t]=true;
            if(i%prime[j]==0)
            {
                mu[t]=0;// 出现平方因子
                break;// 每个合数只被最小质因子筛一次
            }
            mu[t]=-mu[i];
        }
    }
    for(int i=1;i<=n;i++)sum_mu[i]=sum_mu[i-1]+mu[i];
}

// O(sqrt n)，整除分块：求 Σ_{i=1}^{n} mu[i]*(n/i)
ll sum_mu_div(int n)
{
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=n/(n/l);// [l,r] 内 n/i 相同
        res+=(ll)(sum_mu[r]-sum_mu[l-1])*(n/l);
    }
    return res;
}

// O(sqrt n)，整除分块：求 Σ_{i=1}^{n} mu[i]*g(n/i)，g 是任意整数函数
// 要求 g 可 O(1) 求值；分块时 g 的参数 n/i 只会取到 O(sqrt n) 个不同值
ll sum_mu_mul_g(int n,ll g[],int gn)
{
    if(n>gn)return -1;// g 的定义域不够，调用者自己保证 n<=gn
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=n/(n/l);// [l,r] 内 n/i 相同
        res+=(ll)(sum_mu[r]-sum_mu[l-1])*g[n/l];
    }
    return res;
}

// O(log n)，数论分块求 Σ_{i=1}^{n} floor(n/i)，用于自测分块边界
ll sum_floor(int n)
{
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=n/(n/l);
        res+=(ll)(r-l+1)*(n/l);
    }
    return res;
}

// O(sqrt n)，模板核心：ΣΣ[gcd(i,j)==1]，i<=n,j<=m
// 反演后 = Σ_{d=1}^{n} mu[d]*(n/d)*(m/d)
ll sum_coprime(int n,int m)
{
    if(n>m)swap(n,m);
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=min(n/(n/l),m/(m/l));// 两个商同时不变的最远位置
        res+=(ll)(sum_mu[r]-sum_mu[l-1])*(n/l)*(m/l);
    }
    return res;
}

// O(n^2)，暴力数 gcd，对拍用
ll sum_coprime_naive(int n,int m)
{
    ll res=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            if(__gcd(i,j)==1)res++;
    return res;
}

// O(n)，暴力枚举倍数求 Σ mu[i]*(n/i)，对拍整除分块用
ll sum_mu_div_naive(int n)
{
    ll res=0;
    for(int i=1;i<=n;i++)res+=(ll)mu[i]*(n/i);
    return res;
}

// O(sqrt n)，由 mu 前缀和暴力求 Σ mu[i]，对拍线性筛前缀和用
int mertens_naive(int n)
{
    int res=0;
    for(int i=1;i<=n;i++)res+=mu[i];
    return res;
}
