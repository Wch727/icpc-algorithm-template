#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// O(sqrt n)，试除法分解质因数，返回 (素因子,指数) 对，按因子升序
vector<pair<ll,int>> factor(ll n)
{
    vector<pair<ll,int>> f;
    for(ll i=2;i*i<=n;i++)
    {
        if(n%i)continue;
        int c=0;
        while(n%i==0)n/=i,c++;
        f.push_back(make_pair(i,c));// 注意这里保序
    }
    if(n>1)f.push_back(make_pair(n,1));
    return f;
}

// O(sqrt n)，枚举所有正约数，输出无序
vector<ll> get_div(ll n)
{
    vector<ll> v;
    for(ll i=1;i*i<=n;i++)
        if(n%i==0)
        {
            v.push_back(i);
            if(i!=n/i)v.push_back(n/i);
        }
    return v;
}

// O(sqrt n)，约数个数（配合 factor 可做到 O(因子个数)）
int div_cnt(ll n)
{
    int r=0;
    for(ll i=1;i*i<=n;i++)
        if(n%i==0)r+=(i*i==n?1:2);
    return r;
}

// O(sqrt n)，约数之和，防溢出用 __int128 累加
ll div_sum(ll n)
{
    __int128 s=0;
    for(ll i=1;i*i<=n;i++)
        if(n%i==0)
        {
            s+=i;
            if(i!=n/i)s+=n/i;
        }
    return (ll)s;
}

// O(sqrt n)，单个数的欧拉函数
ll phi_one(ll n)
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

int main()
{
    int bad=0;
    // 1) 分解结果相乘 == 原数，且每个因子都是素数
    for(ll n=2;n<=20000;n++)
    {
        vector<pair<ll,int>> f=factor(n);
        __int128 prod=1;
        ll last=0;
        for(int i=0;i<(int)f.size();i++)
        {
            ll p=f[i].first;
            int c=f[i].second;
            if(p<=last)bad++;// 必须升序
            last=p;
            for(int k=2;(ll)k*k<=p;k++)
                if(p%k==0)bad++;// 因子必须是素数
            for(int k=1;k<=c;k++)prod*=p;
        }
        if(prod!=(__int128)n)bad++;
        // 2) 约数个数 / 约数之和 / 枚举约数互相对拍
        vector<ll> v=get_div(n);
        ll s=0;
        for(int i=0;i<(int)v.size();i++)s+=v[i];
        if((int)v.size()!=div_cnt(n))bad++;
        if(s!=div_sum(n))bad++;
    }
    // 再用恒等式对拍：sum_{d|n} phi(d) == n
    for(ll n=1;n<=20000;n++)
    {
        vector<ll> v=get_div(n);
        ll s=0;
        for(int i=0;i<(int)v.size();i++)s+=phi_one(v[i]);
        if(s!=n)bad++;
    }
    // 3) 大数（1e18 量级）分解
    ll big=999999999999999989LL;// 素数
    vector<pair<ll,int>> f1=factor(big);
    if(f1.size()!=1||f1[0].first!=big||f1[0].second!=1)bad++;
    ll big2=1000000007LL*1000000009LL;
    vector<pair<ll,int>> f2=factor(big2);
    __int128 prod=1;
    for(int i=0;i<(int)f2.size();i++)
        for(int k=0;k<f2[i].second;k++)prod*=f2[i].first;
    if(prod!=(__int128)big2)bad++;
    printf("factor(360) = ");
    vector<pair<ll,int>> f=factor(360);
    for(int i=0;i<(int)f.size();i++)printf("%lld^%d ",f[i].first,f[i].second);
    printf("\nd(360)=%d sigma(360)=%lld phi(360)=%lld\n",div_cnt(360),div_sum(360),phi_one(360));
    printf("factor(1000000007*1000000009) = %lld^%d %lld^%d\n",
           f2[0].first,f2[0].second,f2[1].first,f2[1].second);
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1075 输入 21 -> 7（最大的质因子）
// 样例：P3912 输入 10 -> 4（1..10 内素数个数）
// 边界：n=1 时无质因子、约数只有 1；n 为素数时因子是它自己

/*
自测记录：
  1) 2..20000 分解结果相乘等于原数、因子升序且均为素数；
  2) 约数枚举 / 约数个数 / 约数之和三者互拍；
  3) sum_{d|n} phi(d) == n 在 1..20000 上验证；
  4) 1e18 量级素数与大合数各分解一次验证。
*/
