// 二分答案 的测试与对拍代码
// 模板本体：01-基础与技巧/二分答案.cpp
#include "../../01-基础与技巧/二分答案.cpp"

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
