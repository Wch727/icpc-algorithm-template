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

int main()
{
    srand(12345);
    n=0;
    // 自测1：手写样例，值域很大
    ll raw[6]={0,1000000000LL,-500,1000000000LL,7,-500};
    n=5;
    for(int i=1;i<=n;i++)a[i]=raw[i];
    dc.build(n,a);
    printf("k=%d\n",dc.k);
    for(int i=1;i<=n;i++)printf("%d ",dc.get_rank(a[i]));
    printf("\n");
    for(int i=1;i<=dc.k;i++)printf("%lld ",dc.get_val(i));
    printf("\n");
    printf("lower(-500)=%d upper(-500)=%d\n",dc.get_lower(-500),dc.get_upper(-500));// 去重后 -500 排名 1
    int cl=0,cu=0;
    for(int i=1;i<=n;i++)
    {
        if(a[i]<-500)cl++;
        if(a[i]<=-500)cu++;
    }
    printf("brute lower=%d upper=%d\n",cl,cu);
    if(cl!=dc.get_lower(-500)||cu!=dc.get_upper(-500))printf("fail: lower/upper\n");

    // 自测2：与暴力 map 对拍
    for(int t=1;t<=200;t++)
    {
        n=rand()%50+1;
        for(int i=1;i<=n;i++)a[i]=(ll)(rand()%21)-10;// 值域很小，重复多
        dc.build(n,a);
        // 暴力：有序去重
        vector<ll> all;
        for(int i=1;i<=n;i++)all.push_back(a[i]);
        sort(all.begin(),all.end());
        all.erase(unique(all.begin(),all.end()),all.end());
        if((int)all.size()!=dc.k)
        {
            printf("fail: k at t=%d\n",t);
            return 0;
        }
        for(int i=1;i<=n;i++)
        {
            int id=(int)(lower_bound(all.begin(),all.end(),a[i])-all.begin())+1;
            if(dc.get_rank(a[i])!=id||dc.get_rank0(a[i])!=id-1)
            {
                printf("fail: rank at t=%d\n",t);
                return 0;
            }
        }
        for(int r=1;r<=dc.k;r++)
            if(dc.get_val(r)!=all[r-1])
            {
                printf("fail: val at t=%d\n",t);
                return 0;
            }
    }
    printf("discretization self-check OK\n");

    // 自测3：n=1e6 量级跑一遍看耗时
    n=1000000;
    for(int i=1;i<=n;i++)a[i]=(ll)rand()*rand()%1000000007LL;
    clock_t st=clock();
    dc.build(n,a);
    long long sum=0;
    for(int i=1;i<=n;i++)sum+=dc.get_rank(a[i]);
    double cost=(double)(clock()-st)/CLOCKS_PER_SEC;
    printf("n=%d k=%d rank_sum=%lld time=%.3fs\n",n,dc.k,sum,cost);
    return 0;
}
