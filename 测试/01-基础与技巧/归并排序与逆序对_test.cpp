// 归并排序与逆序对 的测试与对拍代码
// 模板本体：01-基础与技巧/归并排序与逆序对.cpp
#include "../../01-基础与技巧/归并排序与逆序对.cpp"

int main()
{
    srand(20240521);
    // 自测1：排序与 sort 对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%60+1;
        for(int i=1;i<=n;i++)a[i]=rand()%7;
        vector<int> b(n+1);// vector 走堆，避免自测里大数组爆栈
        for(int i=1;i<=n;i++)b[i]=a[i];
        sort(b.begin()+1,b.end());
        inv=0;
        merge_sort(1,n);
        for(int i=1;i<=n;i++)
            if(a[i]!=b[i])
            {
                printf("fail merge_sort t=%d i=%d\n",t,i);
                return 0;
            }
    }
    printf("merge_sort self-check OK\n");

    // 自测2：逆序对与 O(n^2) 暴力对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%50+1;
        for(int i=1;i<=n;i++)a[i]=rand()%5;// 有重复，验证相等不计入
        ll bs=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(a[i]>a[j])bs++;
        inv=0;
        merge_sort(1,n);
        if(inv!=bs)
        {
            printf("fail inv t=%d got=%lld want=%lld\n",t,inv,bs);
            return 0;
        }
        for(int i=1;i<=n;i++)a[i]=(int)(rand()%21-10);
        bs=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(a[i]>a[j])bs++;
        ll got=count_inv(1,n);
        if(got!=bs)
        {
            printf("fail count_inv t=%d got=%lld want=%lld\n",t,got,bs);
            return 0;
        }
    }
    printf("inversion self-check OK\n");

    // 自测3：洛谷 P1908 样例
    n=6;
    int v[7]={0,5,4,2,6,3,1};
    for(int i=1;i<=n;i++)a[i]=v[i];
    inv=0;
    merge_sort(1,n);
    printf("P1908 inv=%lld (want 11)\n",inv);
    for(int i=1;i<=n;i++)printf("%d ",a[i]);
    printf("\n");

    // 自测4：n=1e6 速度
    n=1000000;
    for(int i=1;i<=n;i++)a[i]=rand();
    inv=0;
    clock_t st=clock();
    merge_sort(1,n);
    printf("n=%d inv=%lld time=%.3fs\n",n,inv,(double)(clock()-st)/CLOCKS_PER_SEC);
    return 0;
}
