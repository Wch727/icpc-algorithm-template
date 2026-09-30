#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 线段树：区间加 + 区间和，懒标记下推，每次操作 O(log n)
template<typename T,int NMAX=N>
struct SegmentTree
{
    T tr[NMAX*4],lazy[NMAX*4];
    T *src;//建树用的原数组(1-indexed)
    #define lp (p<<1)
    #define rp ((p<<1)|1)
    #define mid ((l+r)>>1)
    void build(int l,int r,int p)
    {
        lazy[p]=0;
        if(l==r)
        {
            tr[p]=src[l];
            return;
        }
        build(l,mid,lp);
        build(mid+1,r,rp);
        tr[p]=tr[lp]+tr[rp];
    }
    void push_down(int p,int l,int r)//把 p 的标记发给两个儿子
    {
        if(lazy[p]==0)return;
        lazy[lp]+=lazy[p],tr[lp]+=lazy[p]*(mid-l+1);
        lazy[rp]+=lazy[p],tr[rp]+=lazy[p]*(r-mid);
        lazy[p]=0;
    }
    void update(int L,int R,T v,int l,int r,int p)
    {
        if(L<=l&&r<=R)//(l,r)<=(L,R)
        {
            lazy[p]+=v,tr[p]+=v*(r-l+1);
            return;
        }
        push_down(p,l,r);
        if(L<=mid)update(L,R,v,l,mid,lp);
        if(R>mid)update(L,R,v,mid+1,r,rp);
        tr[p]=tr[lp]+tr[rp];
    }
    T query(int L,int R,int l,int r,int p)
    {
        if(L<=l&&r<=R)return tr[p];
        push_down(p,l,r);
        T res=0;
        if(L<=mid)res+=query(L,R,l,mid,lp);
        if(R>mid)res+=query(L,R,mid+1,r,rp);
        return res;
    }
};

int n;
ll a[205],br[205];
SegmentTree<ll,205> seg;
