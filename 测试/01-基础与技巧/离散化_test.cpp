// 离散化 的测试与对拍代码
// 模板本体：01-基础与技巧/离散化.cpp
#include "../../01-基础与技巧/离散化.cpp"

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
