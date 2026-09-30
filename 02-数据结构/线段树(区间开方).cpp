#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 线段树：区间开方(向下取整) + 区间和
// 关键性质：一个数最多开方 6 次就变成 1，之后开方不变，用 flag 剪枝
// 均摊复杂度 O((n+m) log n)
template<typename T,int NMAX=N>
struct SegmentTree
{
    T tr[NMAX*4];
    bool flag[NMAX*4];//该区间是否全 <=1，是就不用再递归
    T *src;//建树用的原数组(1-indexed)
    #define lp (p<<1)
    #define rp ((p<<1)|1)
    #define mid ((l+r)>>1)
    void build(int l,int r,int p)
    {
        if(l==r)
        {
            tr[p]=src[l],flag[p]=(tr[p]<=1);
            return;
        }
        build(l,mid,lp);
        build(mid+1,r,rp);
        tr[p]=tr[lp]+tr[rp],flag[p]=flag[lp]&&flag[rp];
    }
    void update(int L,int R,int l,int r,int p)//[L,R] 每个数开方
    {
        if(R<l||r<L)return;//无交集
        if(flag[p])return;//全 0/1，开方不变
        if(l==r)
        {
            tr[p]=(T)sqrt((double)tr[p]),flag[p]=(tr[p]<=1);
            return;
        }
        update(L,R,l,mid,lp);
        update(L,R,mid+1,r,rp);
        tr[p]=tr[lp]+tr[rp],flag[p]=flag[lp]&&flag[rp];
    }
    T query(int L,int R,int l,int r,int p)
    {
        if(L<=l&&r<=R)return tr[p];
        T res=0;
        if(L<=mid)res+=query(L,R,l,mid,lp);
        if(R>mid)res+=query(L,R,mid+1,r,rp);
        return res;
    }
};

int n;
ll a[205],br[205];
SegmentTree<ll,205> seg;
