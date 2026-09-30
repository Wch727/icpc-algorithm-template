#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 手写快排（三数取中选轴）+ O(n) 快速选择求第 k 小
int n,k;
int a[N];

void quick_sort(int l,int r)// O(n log n) 平均，三数取中避免有序数据退化
{
    while(l<r)
    {
        int mid=(l+r)>>1;// 三数取中
        if(a[mid]<a[l])swap(a[mid],a[l]);
        if(a[r]<a[l])swap(a[r],a[l]);
        if(a[r]<a[mid])swap(a[r],a[mid]);
        swap(a[mid],a[l]);// 中位数放到左端当轴
        int pivot=a[l],i=l,j=r;
        do// 双指针划分，等于轴的元素留在原地
        {
            while(a[i]<pivot)i++;
            while(a[j]>pivot)j--;
            if(i<=j)swap(a[i],a[j]),i++,j--;
        }while(i<=j);
        if(j-l<r-i)// 先处理小的那半，栈深 O(log n)
        {
            quick_sort(l,j);
            l=i;
        }
        else
        {
            quick_sort(i,r);
            r=j;
        }
    }
}

// 第 k 小（k 从 1 开始），期望 O(n)，最坏 O(n^2)
int quick_select(int l,int r,int kth)
{
    while(l<=r)
    {
        int mid=(l+r)>>1;// 三数取中
        if(a[mid]<a[l])swap(a[mid],a[l]);
        if(a[r]<a[l])swap(a[r],a[l]);
        if(a[r]<a[mid])swap(a[r],a[mid]);
        swap(a[mid],a[l]);
        int pivot=a[l],i=l,j=r;
        do
        {
            while(a[i]<pivot)i++;
            while(a[j]>pivot)j--;
            if(i<=j)swap(a[i],a[j]),i++,j--;
        }while(i<=j);
        // 此时 [l,j] <= pivot，[i,r] >= pivot，且 j+1 位置就是 pivot 之一
        if(kth<=j)r=j;
        else if(kth>=i)l=i;
        else return a[j+1];
    }
    return a[l];
}

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
