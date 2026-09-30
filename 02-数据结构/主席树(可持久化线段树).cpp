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

int main()
{
    srand(20240513);

    // 1. 手测：a = 1 5 2 6 3 7 4
    n=7;
    int ini[]={0,1,5,2,6,3,7,4};
    for(int i=1;i<=n;i++)a[i]=ini[i];
    build();
    printf("range[2,5] k=3 -> %d\n",b[ct.kth(ct.root[1],ct.root[5],1,sz,3)]);
    printf("range[1,7] k=1 -> %d, k=7 -> %d\n",b[ct.kth(ct.root[0],ct.root[7],1,sz,1)],b[ct.kth(ct.root[0],ct.root[7],1,sz,7)]);
    printf("cnt[2,5] in [1,3] = %d\n",count_range(2,5,1,3));

    // 2. 随机多轮对拍：区间第 k 小 / 区间计数 与暴力比较
    bool ok=true;
    for(int T=1;T<=20&&ok;T++)
    {
        n=rand()%30+1;
        for(int i=1;i<=n;i++)a[i]=rand()%50-25;     // 有重复、有负数
        build();
        for(int q=1;q<=40;q++)
        {
            int l=rand()%n+1,r=rand()%n+1;
            if(l>r)swap(l,r);
            int kk=rand()%(r-l+1)+1;
            vector<int> tmp(a+l,a+r+1);
            sort(tmp.begin(),tmp.end());
            if(b[ct.kth(ct.root[l-1],ct.root[r],1,sz,kk)]!=tmp[kk-1]){printf("第 %d 轮 kth 错 l=%d r=%d kk=%d\n",T,l,r,kk);ok=false;break;}
            int lo=rand()%40-20,hi=rand()%40-20;
            if(lo>hi)swap(lo,hi);
            int want=0;
            for(int i=l;i<=r;i++)if(a[i]>=lo&&a[i]<=hi)want++;
            int got=count_range(l,r,lo,hi);
            if(got!=want){printf("第 %d 轮 cnt 错 l=%d r=%d [%d,%d] got=%d want=%d\n",T,l,r,lo,hi,got,want);ok=false;break;}
        }
        printf("chair round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 全重复值：第 k 小恒为该值
    n=10;
    for(int i=1;i<=n;i++)a[i]=7;
    build();
    printf("all same: sz=%d kth1=%d kth10=%d cnt(all)=%d\n",sz,
        b[ct.kth(ct.root[0],ct.root[10],1,sz,1)],b[ct.kth(ct.root[0],ct.root[10],1,sz,10)],
        count_range(1,10,7,7));

    // 4. 规模测试：n=100000 个互不相同的值
    n=100000;
    for(int i=1;i<=n;i++)a[i]=i*7%100000;      // 互不相同的排列
    build();
    int kmin=ct.kth(ct.root[0],ct.root[n],1,sz,1);
    int kmax=ct.kth(ct.root[0],ct.root[n],1,sz,n);
    printf("big: kth(1,100000,1)=%d(应=%d) kth(1,100000,100000)=%d(应=%d) nodes=%d\n",
        b[kmin],b[1],b[kmax],b[sz],ct.tot);
    printf("big cnt in [1,50000] = %d (应=50000)\n",count_range(1,n,1,50000));
    int l=30000,r=70000,kk=12345;
    vector<int> tmp(a+l,a+r+1);
    sort(tmp.begin(),tmp.end());
    printf("big kth(30000,70000,12345)=%d (应=%d)\n",b[ct.kth(ct.root[l-1],ct.root[r],1,sz,kk)],tmp[kk-1]);
    return 0;
}
