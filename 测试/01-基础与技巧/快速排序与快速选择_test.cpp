// 快速排序与快速选择 的测试与对拍代码
// 模板本体：01-基础与技巧/快速排序与快速选择.cpp
#include "../../01-基础与技巧/快速排序与快速选择.cpp"

int main()
{
    srand(20240520);
    // 自测1：与 sort 对拍，包含大量重复和有序数据
    for(int t=1;t<=500;t++)
    {
        n=rand()%60+1;
        int mode=t%4;
        for(int i=1;i<=n;i++)
        {
            if(mode==0)a[i]=rand()%5;// 重复极多
            else if(mode==1)a[i]=i;// 已升序
            else if(mode==2)a[i]=n-i+1;// 已降序
            else a[i]=rand()%1000-500;
        }
        vector<int> b(n+1);// vector 走堆，避免自测里大数组爆栈
        for(int i=1;i<=n;i++)b[i]=a[i];
        sort(b.begin()+1,b.end());
        quick_sort(1,n);
        for(int i=1;i<=n;i++)
            if(a[i]!=b[i])
            {
                printf("fail quick_sort t=%d i=%d\n",t,i);
                return 0;
            }
    }
    printf("quick_sort self-check OK\n");

    // 自测2：快速选择与暴力对拍
    for(int t=1;t<=500;t++)
    {
        n=rand()%60+1;
        for(int i=1;i<=n;i++)a[i]=rand()%7;
        vector<int> b(n+1);// vector 走堆，避免自测里大数组爆栈
        for(int i=1;i<=n;i++)b[i]=a[i];
        sort(b.begin()+1,b.end());
        for(int kth=1;kth<=n;kth++)
        {
            int got=quick_select(1,n,kth);
            if(got!=b[kth])
            {
                printf("fail quick_select t=%d k=%d got=%d want=%d\n",t,kth,got,b[kth]);
                return 0;
            }
        }
    }
    printf("quick_select self-check OK\n");

    // 自测3：大样例速度 + 有序数据不退化
    n=1000000;
    for(int i=1;i<=n;i++)a[i]=i;// 最坏情况测试
    clock_t st=clock();
    quick_sort(1,n);
    printf("n=%d sorted-input time=%.3fs\n",n,(double)(clock()-st)/CLOCKS_PER_SEC);
    for(int i=1;i<=n;i++)
        if(a[i]!=i)
        {
            printf("fail big sort\n");
            return 0;
        }
    n=1000000;
    for(int i=1;i<=n;i++)a[i]=rand()%1000;
    int kth=n/2;
    st=clock();
    printf("select k=%d -> %d time=%.3fs\n",kth,quick_select(1,n,kth),(double)(clock()-st)/CLOCKS_PER_SEC);

    // 自测4：洛谷 P1923 的调用方式
    n=5,k=2;
    int v[6]={0,4,2,2,5,1};
    for(int i=1;i<=n;i++)a[i]=v[i];
    printf("kth_small(0-indexed 2)=%d (want 2)\n",quick_select(1,n,k+1));
    return 0;
}
