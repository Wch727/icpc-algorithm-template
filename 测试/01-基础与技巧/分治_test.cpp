// 分治 的测试与对拍代码
// 模板本体：01-基础与技巧/分治.cpp
#include "../../01-基础与技巧/分治.cpp"

// 暴力：所有子段枚举，用来对拍

ll brute_max_sub(int l,int r)
{
    ll res=LLONG_MIN;
    for(int i=l;i<=r;i++)
    {
        ll sum=0;
        for(int j=i;j<=r;j++)sum+=a[j],res=max(res,sum);
    }
    return res;
}

// 暴力：逆序对 O(n^2)
ll brute_inv(int l,int r)
{
    ll cnt=0;
    for(int i=l;i<=r;i++)
        for(int j=i+1;j<=r;j++)
            if(a[i]>a[j])cnt++;
    return cnt;
}

int main()
{
    srand(20240602);
    // 自测1：快速幂 递归 vs 迭代 vs 暴力累乘
    for(int t=1;t<=5000;t++)
    {
        ll b=rand()%1000000000,mod=rand()%1000000000+1;
        ll x=rand()%1000+1;
        ll g1=qpow(x,b,mod),g2=qpow_iter(x,b,mod);
        if(g1!=g2)
        {
            printf("fail qpow x=%lld b=%lld mod=%lld got=%lld,%lld\n",x,b,mod,g1,g2);
            return 0;
        }
        if(b<=3000)// 指数小时才敢暴力
        {
            ll want=1%mod;
            for(ll i=1;i<=b;i++)want=want*x%mod;
            if(g1!=want)
            {
                printf("fail qpow brute x=%lld b=%lld mod=%lld got=%lld want=%lld\n",x,b,mod,g1,want);
                return 0;
            }
        }
    }
    printf("qpow self-check OK\n");

    // 自测2：边界——mod=1 必须返回 0；b=0 必须返回 1%mod
    if(qpow_iter(12345,0,1000000007)!=1||qpow_iter(12345,999,1)!=0)
    {
        printf("fail qpow boundary\n");
        return 0;
    }
    printf("qpow boundary self-check OK\n");

    // 自测3：归并排序 + 逆序对，与暴力对拍
    for(int t=1;t<=2000;t++)
    {
        n=rand()%40+1;
        for(int i=1;i<=n;i++)a[i]=rand()%20;// 有重复，验证取等号的处理
        ll want=brute_inv(1,n);
        ll got=merge_sort(1,n);
        if(got!=want)
        {
            printf("fail inv t=%d n=%lld got=%lld want=%lld\n",t,n,got,want);
            return 0;
        }
        for(int i=2;i<=n;i++)// 顺便检查排好序了
            if(a[i-1]>a[i])
            {
                printf("fail merge sort order t=%d\n",t);
                return 0;
            }
    }
    printf("merge sort + inversion self-check OK\n");

    // 自测4：最大子段和分治，与暴力对拍（含全负数、全正数）
    for(int t=1;t<=3000;t++)
    {
        n=rand()%30+1;
        int mode=rand()%3;
        for(int i=1;i<=n;i++)
        {
            if(mode==0)a[i]=rand()%41-20;// 正负混合
            else if(mode==1)a[i]=-rand()%20-1;// 全负
            else a[i]=rand()%20+1;// 全正
        }
        ll got=max_sub(1,n),want=brute_max_sub(1,n);
        if(got!=want)
        {
            printf("fail max_sub t=%d n=%lld got=%lld want=%lld\n",t,n,got,want);
            return 0;
        }
    }
    printf("max subarray divide self-check OK\n");

    // 自测5：套题演示——P1908 逆序对思想 & 洛谷 P1115 最大子段和
    n=6;
    ll w[7]={0,5,4,2,6,3,1};// 逆序对共 11 个
    for(int i=1;i<=n;i++)a[i]=w[i];
    printf("P1908 逆序对=%lld (want 11)\n",merge_sort(1,n));

    n=7;
    ll w2[8]={0,2,-4,3,-1,2,-4,3};// 最大子段和 4：3-1+2 或 3
    for(int i=1;i<=n;i++)a[i]=w2[i];
    printf("P1115 最大子段和=%lld (want 4)\n",max_sub(1,n));

    n=1;
    a[1]=-7;// 单元素全负，答案就是 -7，不能返回 0
    printf("单元素负数最大子段和=%lld (want -7)\n",max_sub(1,n));

    // 自测6：n=1e5 时归并排序 + 逆序对耗时（树状数组/归并都能过）
    n=100000;
    for(int i=1;i<=n;i++)a[i]=rand();
    clock_t st=clock();
    ll inv=merge_sort(1,n);
    printf("n=%lld 逆序对数=%lld time=%.3fs\n",n,inv,(double)(clock()-st)/CLOCKS_PER_SEC);
    return 0;
}
