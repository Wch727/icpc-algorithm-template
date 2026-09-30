// 适用：大 n 的 phi/mu 前缀和，筛不完所有数时用卷积恒等式递归。
// 参数：get_pre 的 n 是筛上界 M，1<=M<N；du_phi/du_mu 的 n 是查询上界。
// 关键：整除商 n/l 相同的闭区间为 [l,n/(n/l)]，一次合并整个区间。
// 结论：sum_phi[n] 为 1..n 的 phi 和，sum_mu[n] 为 Mertens 函数；n=0 返回前缀 0。
// 易错：多组重筛前清 vis、前缀和及缓存；n*(n+1)/2 和块乘积仍可能溢出 ll。
// 复杂度：M 取查询上界的约 2/3 次幂时总时间 O(n^(2/3))；筛空间 O(M)，缓存商值。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=5000006;// 预处理上界，一般取 n^(2/3) 量级

int M;
int prime[N],cnt;// 素数表
bool vis[N];
int phi[N];// 欧拉函数
int mu[N];// 莫比乌斯函数
ll sum_phi[N];// phi 前缀和
ll sum_mu[N];// mu 前缀和

unordered_map<ll,ll> mp_phi,mp_mu;// 记忆化

// O(n)，线性筛 phi / mu 并求前缀和
// O(n)，预处理 1..n，M 记录可直接查表范围；先筛函数值再累加前缀。
void get_pre(int n)
{
    M=n,cnt=0;
    vis[1]=true;
    phi[1]=1,mu[1]=1;
    for(int i=2;i<=n;i++)
    {
        if(!vis[i])
        {
            prime[++cnt]=i;
            phi[i]=i-1;// 素数的 phi
            mu[i]=-1;
        }
        for(int j=1;j<=cnt&&(ll)i*prime[j]<=n;j++)
        {
            int t=i*prime[j];
            vis[t]=true;
            if(i%prime[j]==0)
            {
                phi[t]=phi[i]*prime[j];
                mu[t]=0;
                break;// 每个合数只被最小质因子筛一次
            }
            phi[t]=phi[i]*(prime[j]-1);
            mu[t]=-mu[i];
        }
    }
    for(int i=1;i<=n;i++)
    {
        sum_phi[i]=sum_phi[i-1]+phi[i];
        sum_mu[i]=sum_mu[i-1]+mu[i];
    }
}

// O(n^(2/3))，杜教筛求 Σ_{i=1}^{n} phi(i)
// 用恒等式 Σ_{i=1}^{n} phi(i) = n*(n+1)/2 - Σ_{l=2}^{n} (r-l+1)*S(n/l)
// 适当预筛时 O(n^(2/3))，缓存同一 n；从 l=2 起排除待求的自身项。
ll du_phi(ll n)
{
    if(n<=M)return sum_phi[n];
    if(mp_phi.count(n))return mp_phi[n];
    ll res=n*(n+1)/2;
    for(ll l=2,r;l<=n;l=r+1)
    {
        r=n/(n/l);// 整除分块
        res-=(r-l+1)*du_phi(n/l);
    }
    return mp_phi[n]=res;
}

// O(n^(2/3))，杜教筛求 Σ_{i=1}^{n} mu(i)
// 用恒等式 1 = Σ_{l=1}^{n} (r-l+1)*S(n/l)，即 S(n) = 1 - Σ_{l=2}^{n} (r-l+1)*S(n/l)
// 适当预筛时 O(n^(2/3))，利用 mu*1 的前缀恒为 1，递归参数 n/l 严格减小。
ll du_mu(ll n)
{
    if(n<=M)return sum_mu[n];
    if(mp_mu.count(n))return mp_mu[n];
    ll res=1;
    for(ll l=2,r;l<=n;l=r+1)
    {
        r=n/(n/l);
        res-=(r-l+1)*du_mu(n/l);
    }
    return mp_mu[n]=res;
}

// O(n)，暴力前缀和，对拍用
// O(n*sqrt n) 上界，逐个试除求 phi 再累加；只适合小范围核对。
ll phi_naive_sum(ll n)
{
    ll res=0;
    for(ll i=1;i<=n;i++)
    {
        ll x=i,r=x;
        for(ll j=2;j*j<=x;j++)
            if(x%j==0)
            {
                r=r/j*(j-1);
                while(x%j==0)x/=j;
            }
        if(x>1)r=r/x*(x-1);
        res+=r;
    }
    return res;
}
