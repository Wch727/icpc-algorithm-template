#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 线段树：区间乘 + 区间加 + 区间和，每次操作 O(log n)
// T 为不超过 64 位的整数，mod>=0；不取模时结果须能放入 T。
// 标记约定：先乘后加，儿子值 = 儿子值*mul + add*区间长度
template<typename T,int NMAX=N>
struct SegmentTree
{
    T tr[NMAX*4],add[NMAX*4],mul[NMAX*4],mod;
    T *src;//建树用的原数组(1-indexed)
    #define lp (p<<1)
    #define rp ((p<<1)|1)
    #define mid ((l+r)>>1)
    T mo(__int128 x)
    {
        if(!mod)
            return (T)x;
        x%= mod;
        return (T)(x < 0 ? x + mod : x);
    }
    void build(int l,int r,int p)
    {
        add[p]=0,mul[p]=1;//加标记 0，乘标记 1
        if(l==r)
        {
            tr[p]=mo(src[l]);
            return;
        }
        build(l,mid,lp);
        build(mid+1,r,rp);
        tr[p]=mo((__int128)tr[lp]+tr[rp]);
    }
    void push_down(int p,int l,int r)
    {
        if(mul[p]==1&&add[p]==0)return;
        tr[lp]=mo((__int128)tr[lp]*mul[p]+(__int128)add[p]*(T)(mid-l+1));
        mul[lp]=mo((__int128)mul[lp]*mul[p]);
        add[lp]=mo((__int128)add[lp]*mul[p]+add[p]);
        tr[rp]=mo((__int128)tr[rp]*mul[p]+(__int128)add[p]*(T)(r-mid));
        mul[rp]=mo((__int128)mul[rp]*mul[p]);
        add[rp]=mo((__int128)add[rp]*mul[p]+add[p]);
        mul[p]=1,add[p]=0;
    }
    void update_add(int L,int R,T v,int l,int r,int p)
    {
        if(L<=l&&r<=R)//(l,r)<=(L,R)
        {
            tr[p]=mo((__int128)tr[p]+(__int128)v*(T)(r-l+1));
            add[p]=mo((__int128)add[p]+v);
            return;
        }
        push_down(p,l,r);
        if(L<=mid)update_add(L,R,v,l,mid,lp);
        if(R>mid)update_add(L,R,v,mid+1,r,rp);
        tr[p]=mo((__int128)tr[lp]+tr[rp]);
    }
    void update_mul(int L,int R,T v,int l,int r,int p)
    {
        if(L<=l&&r<=R)
        {
            tr[p]=mo((__int128)tr[p]*v);
            mul[p]=mo((__int128)mul[p]*v);
            add[p]=mo((__int128)add[p]*v);//加法标记也要乘
            return;
        }
        push_down(p,l,r);
        if(L<=mid)update_mul(L,R,v,l,mid,lp);
        if(R>mid)update_mul(L,R,v,mid+1,r,rp);
        tr[p]=mo((__int128)tr[lp]+tr[rp]);
    }
    T query(int L,int R,int l,int r,int p)
    {
        if(L<=l&&r<=R)return tr[p];
        push_down(p,l,r);
        T res=0;
        if(L<=mid)res=mo((__int128)res+query(L,R,l,mid,lp));
        if(R>mid)res=mo((__int128)res+query(L,R,mid+1,r,rp));
        return res;
    }
};

int n;
ll a[205],br[205];
SegmentTree<ll,205> seg;
