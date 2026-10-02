#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// O(n log n)，统计严格逆序对；会将 a[l..r] 排好序。
const int N=1000005;
int a[N],tmp[N];

ll count_inv(int l,int r)// 相等不算逆序对
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
