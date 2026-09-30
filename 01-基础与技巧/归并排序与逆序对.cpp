// 适用：排序同时统计 i<j 且 a[i]>a[j] 的对数；a[1..n]，tmp 为同容量缓冲。
// 两个接口都会改变原数组顺序；多组数据调用 merge_sort 前自行将 inv 清零。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 归并排序 + 归并求逆序对，O(n log n)
int n;
int a[N],tmp[N];
ll inv;// 逆序对数会超过 int，必须 long long

// O(len log len) 时间、O(len) 缓冲，len=r-l+1；排序闭区间 [l,r]，并累加全局 inv。
void merge_sort(int l,int r)// 稳定排序，值相同的相对顺序不变
{
    if(l>=r)return;
    int m=(l+r)>>1;
    merge_sort(l,m);
    merge_sort(m+1,r);
    int i=l,j=m+1,cnt=l;
    while(i<=m&&j<=r)
    {
        if(a[i]<=a[j])tmp[cnt++]=a[i++];// 取等号，相等不算逆序
        else
        {
            inv+=m-i+1;// a[i..m] 都比 a[j] 大
            tmp[cnt++]=a[j++];
        }
    }
    while(i<=m)tmp[cnt++]=a[i++];
    while(j<=r)tmp[cnt++]=a[j++];
    for(int i=l;i<=r;i++)a[i]=tmp[i];
}

// O(len log len) 时间、O(len) 缓冲；闭区间 [l,r] 的逆序对数，直接返回不依赖 inv。
// 右元素更小时左侧剩余元素全大于它，所以一次增加 m-i+1，而非逐对枚举。
ll count_inv(int l,int r)// 只要逆序对数，返回答案
{
    if(l>=r)return 0;
    int m=(l+r)>>1;
    ll res=count_inv(l,m)+count_inv(m+1,r);
    int i=l,j=m+1,cnt=l;
    while(i<=m&&j<=r)
    {
        if(a[i]<=a[j])tmp[cnt++]=a[i++];
        else res+=m-i+1,tmp[cnt++]=a[j++];
    }
    while(i<=m)tmp[cnt++]=a[i++];
    while(j<=r)tmp[cnt++]=a[j++];
    for(int i=l;i<=r;i++)a[i]=tmp[i];
    return res;
}
