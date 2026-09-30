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

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：随机区间加 + 随机区间和，与暴力数组对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=20;t++)
    {
        n=rnd(1,60);
        for(int i=1;i<=n;i++)a[i]=rnd(-50,50),br[i]=a[i];
        seg.src=a;
        seg.build(1,n,1);
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,2),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                ll v=rnd(-30,30);
                seg.update(l,r,v,1,n,1);
                for(int i=l;i<=r;i++)br[i]+=v;
            }
            else
            {
                ll x=seg.query(l,r,1,n,1),z=0;
                for(int i=l;i<=r;i++)z+=br[i];
                cnt++;
                if(x!=z)bad++;
            }
        }
    }
    printf("线段树(区间加区间和) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[1,2,3,4,5]，[2,4] 加 10 后区间和 [1,5]=25、[3,3]=13
    n=5;
    for(int i=1;i<=5;i++)a[i]=i;
    seg.src=a;
    seg.build(1,n,1);
    seg.update(2,4,10,1,n,1);
    printf("小样例: sum[1,5]=%lld sum[3,3]=%lld\n",seg.query(1,5,1,n,1),seg.query(3,3,1,n,1));
    return 0;
}
