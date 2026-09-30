#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 分治：把问题拆成同类的子问题 -> 递归求解 -> 合并答案
// 模板三段式：1) 边界 return  2) 分 mid 递归  3) 跨 mid 的部分单独算 + 合并
// 复杂度看合并：O(1) 合并出 O(log n) 层（快速幂），O(n) 合并出 O(n log n)（归并/最大子段和）
ll n,a[N],tmp[N];

// ---- 快速幂（分治思想：a^n 拆成 a^(n/2) 平方）----
// 递归版最能体现分治：n 为偶 -> a^(n/2)^2；n 为奇 -> 再乘一个 a
ll qpow(ll a,ll b,ll mod)
{
    if(mod==1)return 0;
    if(b==0)return 1%mod;
    ll half=qpow(a,b>>1,mod);
    ll res=half*half%mod;
    if(b&1)res=res*a%mod;
    return res;
}

// 迭代版（二进制拆分 b 的每一位），赛场上更快更常用
ll qpow_iter(ll a,ll b,ll mod)
{
    if(mod==1)return 0;
    ll res=1%mod;
    a%=mod;
    while(b>0)
    {
        if(b&1)res=res*a%mod;
        a=a*a%mod;
        b>>=1;
    }
    return res;
}

// ---- 归并思想：排序 + 统计逆序对 ----
// merge_sort(l,r) 返回 [l,r] 内的逆序对数量，同时把区间排好序
// 关键：只有「跨左右」的逆序对需要在合并时数，左右内部由递归负责
ll merge_sort(int l,int r)
{
    if(l>=r)return 0;
    int mid=(l+r)>>1;
    ll cnt=merge_sort(l,mid)+merge_sort(mid+1,r);
    int i=l,j=mid+1,k=l;
    while(i<=mid&&j<=r)
    {
        if(a[i]<=a[j])tmp[k++]=a[i++];// 取等号：相等不算逆序，答案才不重复统计
        else tmp[k++]=a[j++],cnt+=mid-i+1;// a[j] 比 a[i..mid] 都小
    }
    while(i<=mid)tmp[k++]=a[i++];
    while(j<=r)tmp[k++]=a[j++];
    for(int p=l;p<=r;p++)a[p]=tmp[p];
    return cnt;
}

// ---- 最大子段和（分治）----
// 跨中点的答案 = 左半边最大后缀和 + 右半边最大前缀和
// 不能只递归两边，因为最优子段可能横跨 mid
ll max_sub(int l,int r)
{
    if(l==r)return a[l];// 边界：单个元素就是它自己
    int mid=(l+r)>>1;
    ll lans=max_sub(l,mid),rans=max_sub(mid+1,r);
    ll sum=0,lmax=LLONG_MIN;
    for(int i=mid;i>=l;i--)sum+=a[i],lmax=max(lmax,sum);// 左半边最大后缀
    sum=0;
    ll rmax=LLONG_MIN;
    for(int i=mid+1;i<=r;i++)sum+=a[i],rmax=max(rmax,sum);// 右半边最大前缀
    return max(max(lans,rans),lmax+rmax);// 三者取大
}
