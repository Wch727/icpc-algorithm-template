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
