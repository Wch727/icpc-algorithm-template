#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 线段树：区间乘 + 区间加 + 区间和，每次操作 O(log n)
// 标记约定：先乘后加，儿子值 = 儿子值*mul + add*区间长度
template<typename T,int NMAX=N>
struct SegmentTree
{
    T tr[NMAX*4],add[NMAX*4],mul[NMAX*4],mod;
    T *src;//建树用的原数组(1-indexed)
    #define lp (p<<1)
    #define rp ((p<<1)|1)
    #define mid ((l+r)>>1)
    T mo(T x)
    {
        return mod?x%mod:x;//mod=0 表示不取模
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
        tr[p]=mo(tr[lp]+tr[rp]);
    }
    void push_down(int p,int l,int r)
    {
        if(mul[p]==1&&add[p]==0)return;
        tr[lp]=mo(tr[lp]*mul[p]+add[p]*(T)(mid-l+1));
        mul[lp]=mo(mul[lp]*mul[p]);
        add[lp]=mo(add[lp]*mul[p]+add[p]);
        tr[rp]=mo(tr[rp]*mul[p]+add[p]*(T)(r-mid));
        mul[rp]=mo(mul[rp]*mul[p]);
        add[rp]=mo(add[rp]*mul[p]+add[p]);
        mul[p]=1,add[p]=0;
    }
    void update_add(int L,int R,T v,int l,int r,int p)
    {
        if(L<=l&&r<=R)//(l,r)<=(L,R)
        {
            tr[p]=mo(tr[p]+v*(T)(r-l+1));
            add[p]=mo(add[p]+v);
            return;
        }
        push_down(p,l,r);
        if(L<=mid)update_add(L,R,v,l,mid,lp);
        if(R>mid)update_add(L,R,v,mid+1,r,rp);
        tr[p]=mo(tr[lp]+tr[rp]);
    }
    void update_mul(int L,int R,T v,int l,int r,int p)
    {
        if(L<=l&&r<=R)
        {
            tr[p]=mo(tr[p]*v);
            mul[p]=mo(mul[p]*v);
            add[p]=mo(add[p]*v);//加法标记也要乘
            return;
        }
        push_down(p,l,r);
        if(L<=mid)update_mul(L,R,v,l,mid,lp);
        if(R>mid)update_mul(L,R,v,mid+1,r,rp);
        tr[p]=mo(tr[lp]+tr[rp]);
    }
    T query(int L,int R,int l,int r,int p)
    {
        if(L<=l&&r<=R)return tr[p];
        push_down(p,l,r);
        T res=0;
        if(L<=mid)res=mo(res+query(L,R,l,mid,lp));
        if(R>mid)res=mo(res+query(L,R,mid+1,r,rp));
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

// 自测：大模数 1e9+7 和小模数 97 各跑一遍，与暴力数组对拍
void run(ll md,ll &bad,ll &cnt)
{
    for(int t=1;t<=10;t++)
    {
        n=rnd(1,50);
        seg.mod=md;
        for(int i=1;i<=n;i++)a[i]=rnd(0,100)%md,br[i]=a[i];
        seg.src=a;
        seg.build(1,n,1);
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,3),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==3)
            {
                ll x=seg.query(l,r,1,n,1),z=0;
                for(int i=l;i<=r;i++)z=(z+br[i])%md;
                cnt++;
                if(x!=z)bad++;
                continue;
            }
            ll v=rnd(0,60);
            if(op==1)
            {
                seg.update_mul(l,r,v,1,n,1);
                for(int i=l;i<=r;i++)br[i]=br[i]*v%md;
            }
            else
            {
                seg.update_add(l,r,v,1,n,1);
                for(int i=l;i<=r;i++)br[i]=(br[i]+v)%md;
            }
        }
    }
}

int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    run(1000000007,bad,cnt);
    run(97,bad,cnt);
    printf("线段树(区间加乘) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[1,1,1]，先 [1,3] 乘 2、再 [1,3] 加 3，区间和为 15
    n=3,seg.mod=1000000007;
    for(int i=1;i<=3;i++)a[i]=1;
    seg.src=a;
    seg.build(1,n,1);
    seg.update_mul(1,3,2,1,n,1);
    seg.update_add(1,3,3,1,n,1);
    printf("小样例: sum[1,3]=%lld sum[2,2]=%lld\n",seg.query(1,3,1,n,1),seg.query(2,2,1,n,1));
    return 0;
}
