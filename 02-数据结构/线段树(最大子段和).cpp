#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=500005;

// 线段树：单点修改 + 区间查询最大子段和(不能为空)
// 每个结点维护 sum / lmax(左端起) / rmax(右端起) / mx，merge O(1)，每次操作 O(log n)
template<typename T,int NMAX=N>
struct SegmentTree
{
    struct Node
    {
        T sum,lmax,rmax,mx;
        Node(){}
        Node(T x){sum=lmax=rmax=mx=x;}
    }tr[NMAX*4];
    T *src;//建树用的原数组(1-indexed)
    #define lp (p<<1)
    #define rp ((p<<1)|1)
    #define mid ((l+r)>>1)
    Node merge(Node a,Node b)
    {
        Node c;
        c.sum=a.sum+b.sum;
        c.lmax=max(a.lmax,a.sum+b.lmax);
        c.rmax=max(b.rmax,b.sum+a.rmax);
        c.mx=max(max(a.mx,b.mx),a.rmax+b.lmax);//跨过中点的那一段
        return c;
    }
    void build(int l,int r,int p)
    {
        if(l==r)
        {
            tr[p]=Node(src[l]);
            return;
        }
        build(l,mid,lp);
        build(mid+1,r,rp);
        tr[p]=merge(tr[lp],tr[rp]);
    }
    void update(int L,T v,int l,int r,int p)//把 a[L] 改成 v
    {
        if(l==r)
        {
            tr[p]=Node(v);
            return;
        }
        if(L<=mid)update(L,v,l,mid,lp);
        else update(L,v,mid+1,r,rp);
        tr[p]=merge(tr[lp],tr[rp]);
    }
    Node query(int L,int R,int l,int r,int p)
    {
        if(L<=l&&r<=R)return tr[p];
        if(R<=mid)return query(L,R,l,mid,lp);
        if(L>mid)return query(L,R,mid+1,r,rp);
        return merge(query(L,R,l,mid,lp),query(L,R,mid+1,r,rp));
    }
};

int n;
ll a[205],br[205];
SegmentTree<ll,205> seg;
