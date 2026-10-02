#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int INF=0x3f3f3f3f;
int n;
int a[N],d[N],f[N],pre[N],tmp[N],mp[N],c[N];
int st[N];// 还原方案用的栈

// O(n^2)，最长严格上升子序列：f[i] 表示以 a[i] 结尾的最长长度
int lis_n2(int n,int a[])
{
    int ans=0;
    for(int i=1;i<=n;i++)
    {
        f[i]=1;
        for(int j=1;j<i;j++)
            if(a[j]<a[i])f[i]=max(f[i],f[j]+1);
        ans=max(ans,f[i]);
    }
    return ans;
}

// O(n log n)，最长严格上升子序列
// d[len] 表示长度 len 的上升子序列的最小结尾，d 单调不减

int lis_nlogn(int n,int a[])
{
    int len=0;
    for(int i=1;i<=n;i++)
    {
        int p=lower_bound(d+1,d+len+1,a[i])-d;
        d[p]=a[i];
        len=max(len,p);
    }
    return len;
}

// O(n log n)，最长不降子序列：只把 >= 改成 >，二分找第一个比 a[i] 大的

int lnds_nlogn(int n,int a[])
{
    int len=0;
    for(int i=1;i<=n;i++)
    {
        int p=upper_bound(d+1,d+len+1,a[i])-d;
        d[p]=a[i];
        len=max(len,p);
    }
    return len;
}

// O(n log n)，最长严格下降子序列：用降序比较器，避免 INT_MIN 取负溢出

int lds_nlogn(int n,int a[])
{
    int len=0;
    for(int i=1;i<=n;i++)
    {
        int p=lower_bound(d+1,d+len+1,a[i],greater<int>())-d;
        d[p]=a[i];
        len=max(len,p);
    }
    return len;
}

// O(n log n)，两个排列(值 1..n)的 LCS 转 LIS
// a 中每个值的位置记下来，按 b 的顺序排成 c，c 的 LIS 就是 LCS
int lcs_perm(int n,int a[],int b[])
{
    for(int i=1;i<=n;i++)mp[a[i]]=i;// 值 -> 在 a 中的位置
    for(int i=1;i<=n;i++)c[i]=mp[b[i]];
    return lis_nlogn(n,c);
}

// O(n^2)，记录前驱并输出一组最优解
void lis_scheme(int n,int a[])
{
    int ans=0,best=0,top=0;
    for(int i=1;i<=n;i++)
    {
        f[i]=1,pre[i]=0;
        for(int j=1;j<i;j++)
            if(a[j]<a[i]&&f[j]+1>f[i])f[i]=f[j]+1,pre[i]=j;
        if(f[i]>ans)ans=f[i],best=i;
    }
    while(best)st[++top]=a[best],best=pre[best];// 逆着走前驱
    printf("长度 %d，一组方案：",ans);
    for(int i=top;i>=1;i--)printf("%d ",st[i]);
    printf("\n");
}
