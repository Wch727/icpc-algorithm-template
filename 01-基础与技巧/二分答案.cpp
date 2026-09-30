#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 整数二分：O(log V) 次 check；浮点二分：固定迭代次数，精度 1e-7 左右
ll n,m;
ll a[N];

// 二分答案两个方向，都在 [l,r] 上做，返回 -1 表示区间内无解
// bs_first_true：check 满足 F F F T T T（随 x 变大由假变真），返回第一个真
//                常用于「最小化最大值」：check(x)=能否做到最大值 <= x
// bs_last_true ：check 满足 T T T F F F（随 x 变大由真变假），返回最后一个真
//                常用于「最大化最小值」：check(x)=能否做到最小值 >= x
// 取中点都用 l+((r-l)>>1)：区间含负数时 (l+r)>>1 不再等于数学中点，会漏解

template<typename F>
ll bs_first_true(ll l,ll r,F check)
{
    ll ans=-1;
    while(l<=r)
    {
        ll mid=l+((r-l)>>1);
        if(check(mid))ans=mid,r=mid-1;
        else l=mid+1;
    }
    return ans;
}

template<typename F>
ll bs_last_true(ll l,ll r,F check)
{
    ll ans=-1;
    while(l<=r)
    {
        ll mid=l+((r-l+1)>>1);// 右偏，保证 r 能收到 mid-1 且区间一定缩小
        if(check(mid))ans=mid,l=mid+1;
        else r=mid-1;
    }
    return ans;
}

// 有序数组中第一个 >= x 的位置，不存在返回 n+1（等价 lower_bound）
ll lower_id(ll* p,ll len,ll x)
{
    ll l=1,r=len+1;// 答案单调，右端点开区间 n+1 表示无解
    while(l<r)
    {
        ll mid=(l+r)>>1;
        if(p[mid]>=x)r=mid;
        else l=mid+1;
    }
    return l;
}

// 浮点二分：找满足 check 的最小值，固定 100 次迭代，精度够用且不会死循环
template<typename F>
double bs_double(double l,double r,F check)
{
    for(int i=1;i<=100;i++)
    {
        double mid=(l+r)/2;
        if(check(mid))r=mid;
        else l=mid;
    }
    return r;
}

// 洛谷 P1182：把 n 个数切成不超过 m 段，最小化最大段和
bool check_p1182(ll x)
{
    ll cnt=1,len=0;
    for(int i=1;i<=n;i++)
    {
        if(len+a[i]<=x)len+=a[i];
        else cnt++,len=a[i];
    }
    return cnt<=m;
}

// 洛谷 P2440：把木头切成 k 段，最大化每段长度
ll k,wood[N];
bool check_p2440(ll x)
{
    if(x==0)return true;
    ll cnt=0;
    for(int i=1;i<=n;i++)cnt+=wood[i]/x;
    return cnt>=k;
}

int main()
{
    srand(20240516);
    // 自测1：两个方向分别与线性扫描对拍，区间含负数
    // bs_first_true 配单调不减条件 x>=thr；bs_last_true 配单调不增条件 x<=thr
    for(int t=1;t<=500;t++)
    {
        ll l=rand()%21-10,r=l+rand()%40;
        ll thr=rand()%61-20;
        ll w1=-1,w2=-1;
        for(ll x=l;x<=r;x++)
            if(x>=thr){w1=x;break;}// 第一个满足 x>=thr
        for(ll x=r;x>=l;x--)
            if(x<=thr){w2=x;break;}// 最后一个满足 x<=thr
        ll g1=bs_first_true(l,r,[&](ll x){return x>=thr;});
        ll g2=bs_last_true(l,r,[&](ll x){return x<=thr;});
        if(g1!=w1||g2!=w2)
        {
            printf("fail bs t=%d l=%lld r=%lld thr=%lld got=%lld,%lld want=%lld,%lld\n",t,l,r,thr,g1,g2,w1,w2);
            return 0;
        }
    }
    printf("bs_first_true/bs_last_true self-check OK\n");

    // 自测2：lower_id 与 STL lower_bound 对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%30+1;
        for(int i=1;i<=n;i++)a[i]=rand()%11;// 有重复
        sort(a+1,a+1+n);
        for(int v=-2;v<=12;v++)
        {
            ll want=(ll)(lower_bound(a+1,a+1+n,(ll)v)-a);
            if(lower_id(a,n,v)!=want)
            {
                printf("fail lower_id v=%d\n",v);
                return 0;
            }
        }
    }
    printf("lower_id self-check OK\n");

    // 自测3：浮点二分求 sqrt，与库函数对拍
    for(int t=1;t<=100;t++)
    {
        double x=(double)(rand()%100000+1)/1000.0;
        double got=bs_double(0,1000,[&](double v){return v*v>=x;});
        if(fabs(got-sqrt(x))>1e-6)
        {
            printf("fail bs_double x=%.6f got=%.10f want=%.10f\n",x,got,sqrt(x));
            return 0;
        }
    }
    printf("bs_double self-check OK\n");

    // 自测4：套题模板
    n=5,m=3;
    ll arr[6]={0,4,2,4,5,1};// 答案 6：4+2|4|5+1，段和最大为 6
    for(int i=1;i<=n;i++)a[i]=arr[i];
    ll sum=0,mx=0;
    for(int i=1;i<=n;i++)sum+=a[i],mx=max(mx,a[i]);
    printf("P1182 min-max=%lld (want 6)\n",bs_first_true(mx,sum,check_p1182));

    n=3,k=7;
    ll wd[4]={0,232,124,456};
    for(int i=1;i<=n;i++)wood[i]=wd[i];
    printf("P2440 max-len=%lld (want 114)\n",bs_last_true(0,100000000LL,check_p2440));
    return 0;
}
