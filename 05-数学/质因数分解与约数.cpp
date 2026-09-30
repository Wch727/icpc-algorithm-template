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
