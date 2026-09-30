#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 归并排序 + 归并求逆序对，O(n log n)
int n;
int a[N],tmp[N];
ll inv;// 逆序对数会超过 int，必须 long long

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
