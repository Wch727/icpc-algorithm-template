#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 离散化：排序去重 + lower_bound 映射，O(n log n)
int n,m;
ll a[N],b[N];

struct Discretization
{
    int k;// 不同元素个数
    ll v[N];// 存排序去重后的值
    void build(int len,ll* p)// 用 p[1..len] 建表，b 保留完整排序结果（带重复）供计数用
    {
        for(int i=1;i<=len;i++)b[i]=p[i];
        sort(b+1,b+1+len);
        k=0;
        for(int i=1;i<=len;i++)// 手动去重，避免 unique 把 b 后面的元素搬走
            if(i==1||b[i]!=b[i-1])v[++k]=b[i];
    }
    int get_rank(ll x)// 返回 1..k 的名次
    {
        return (int)(lower_bound(v+1,v+1+k,x)-v);
    }
    int get_rank0(ll x)// 返回 0..k-1，方便树状数组/数组下标
    {
        return get_rank(x)-1;
    }
    ll get_val(int r)// 名次反查原值，1<=r<=k
    {
        return v[r];
    }
    int get_lower(ll x)// <x 的个数，即 x 的前驱个数
    {
        return (int)(lower_bound(v+1,v+1+k,x)-v)-1;
    }
    int get_upper(ll x)// 原数组中 <=x 的个数（带重复计数），不是去重表里的下标
    {
        return (int)(upper_bound(b+1,b+1+n,x)-b)-1;
    }
}dc;
