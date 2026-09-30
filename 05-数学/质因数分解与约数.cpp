// 适用：单个正整数的因子、约数、phi；大批查询可换线性筛，大整数分解可换 Rho。
// 参数：所有函数的 n>=1；返回向量下标 0 起，factor 的第二项为因子指数。
// 关键：试除到剩余 n 的平方根；循环结束剩余数若大于 1 必为素数。
// 结论：约数个数为指数加一的乘积；phi(n)=n*各不同素因子的 (1-1/p) 乘积。
// 易错：i*i 接近 ll 上界会溢出，div_sum 虽宽整数累加，最终返回仍须能存入 ll。
// 复杂度：试除与枚举均 O(sqrt n) 上界，约数向量空间 O(约数个数)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// O(sqrt n)，试除法分解质因数，返回 (素因子,指数) 对，按因子升序
// O(sqrt n)，返回升序 (素因子,重数)；输入 n=1 返回空因子表。
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
// O(sqrt n)，返回无序正约数；i==n/i 仅加入一次，自行排序以获得升序。
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
// O(sqrt n)，返回正约数个数；完全平方数的中间约数不重复计。
int div_cnt(ll n)
{
    int r=0;
    for(ll i=1;i*i<=n;i++)
        if(n%i==0)r+=(i*i==n?1:2);
    return r;
}

// O(sqrt n)，约数之和，防溢出用 __int128 累加
// O(sqrt n)，对互补约数求和；__int128 中间值最后转回 ll。
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
// O(sqrt n)，每个不同素因子只应用一次 r=r/p*(p-1)，phi(1)=1。
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
