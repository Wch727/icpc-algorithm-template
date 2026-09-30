// 主席树（可持久化线段树）：静态区间第 k 小 / 区间内某值域的数个数
// 建树 O(n log n)，单次询问 O(log n)，空间 O(n log n)
// 思路：对每个前缀 [1..i] 建一棵权值线段树，区间 [l,r] 的信息就是 root[r]-root[l-1]
// 调用约定：kth(root[l-1],root[r],1,sz,k)、cnt(root[l-1],root[r],1,sz,x)
//           第一个参数是「小版本」，写反了会算出 0 或负数
// 坑：insert 里 sum 必须在递归返回之后按两个孩子重算，不能在递归前写 sum[old]+1
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
const int LOG=20;       // log2(N)+2
int n,sz;               // n 元素个数，sz 离散化后互不相同的值个数
int a[N],b[N];          // a 原数组，b 离散化后的值（升序去重）

struct ChairTree{
    int ls[N*LOG],rs[N*LOG],sum[N*LOG];
    int root[N],tot;
    void init()
    {
        tot=0,root[0]=0;
        ls[0]=rs[0]=sum[0]=0;
    }
    // 在 old 版本上插入 pos，返回新版本根，O(log n)
    int insert(int old,int l,int r,int pos)
    {
        int p=++tot;
        ls[p]=ls[old],rs[p]=rs[old];
        if(l==r){sum[p]=sum[old]+1;return p;}
        int mid=(l+r)>>1;
        if(pos<=mid)ls[p]=insert(ls[old],l,mid,pos);
        else rs[p]=insert(rs[old],mid+1,r,pos);
        sum[p]=sum[ls[p]]+sum[rs[p]];       // 必须放在递归之后
        return p;
    }
    // [l-1,r] 版本相减，求第 k 小，返回离散化下标，O(log n)
    int kth(int u,int v,int l,int r,int k)
    {
        while(l<r)
        {
            int mid=(l+r)>>1;
            int cnt=sum[ls[v]]-sum[ls[u]];      // 值落在左半边的有几个
            if(k<=cnt)u=ls[u],v=ls[v],r=mid;
            else k-=cnt,u=rs[u],v=rs[v],l=mid+1;
        }
        return l;
    }
    // [l-1,r] 里值 <= x（离散化下的位置 x）的个数，O(log n)
    int count_le(int u,int v,int l,int r,int x)
    {
        if(x<=0)return 0;                   // 比最小值还小
        if(r<=x)return sum[v]-sum[u];       // 整段都 <= x
        int mid=(l+r)>>1;
        int res=count_le(ls[u],ls[v],l,mid,x);
        if(x>mid)res+=count_le(rs[u],rs[v],mid+1,r,x);
        return res;
    }
    // 区间 [l,r] 里值等于 x（离散化位置）的个数
    int cnt_value(int u,int v,int x){return count_le(u,v,1,sz,x)-count_le(u,v,1,sz,x-1);}
}ct;

// 建树：把 a[1..n] 离散化后建成主席树，去重后的值个数写回全局 sz
void build()
{
    for(int i=1;i<=n;i++)b[i]=a[i];
    sort(b+1,b+n+1);
    sz=unique(b+1,b+n+1)-b-1;
    ct.init();
    for(int i=1;i<=n;i++)
    {
        int pos=lower_bound(b+1,b+sz+1,a[i])-b;
        ct.root[i]=ct.insert(ct.root[i-1],1,sz,pos);
    }
}

// 值域 [lo,hi]（真实值）内的数个数：把 lo-1、hi 转到离散化位置再做差
int count_range(int L,int R,int lo,int hi)
{
    int p=lower_bound(b+1,b+sz+1,lo)-b-1;   // 最后一个位置，它的值 < lo
    int q=upper_bound(b+1,b+sz+1,hi)-b-1;   // 最后一个位置，它的值 <= hi
    if(q<=0)return 0;
    if(p<0)p=0;
    return ct.count_le(ct.root[L-1],ct.root[R],1,sz,q)-ct.count_le(ct.root[L-1],ct.root[R],1,sz,p);
}
