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

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

// 自测：随机区间开方 + 随机区间和，与暴力数组对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=20;t++)
    {
        n=rnd(1,60);
        for(int i=1;i<=n;i++)a[i]=rnd(0,1000000),br[i]=a[i];
        a[1]=0,a[n]=1;//边界：0 和 1 开方不变
        br[1]=a[1],br[n]=a[n];
        seg.src=a;
        seg.build(1,n,1);
        for(int q=1;q<=300;q++)
        {
            int op=rnd(1,2),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                seg.update(l,r,1,n,1);
                for(int i=l;i<=r;i++)br[i]=(ll)sqrt((double)br[i]);
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
    printf("线段树(区间开方) vs 暴力: %s, 查询=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：a=[1,4,9,16]，[1,4] 开方后为 [1,2,3,4]，和为 10
    n=4;
    for(int i=1;i<=4;i++)a[i]=(ll)(i+0)*(i+0);
    seg.src=a;
    seg.build(1,n,1);
    seg.update(1,4,1,n,1);
    printf("小样例: sum[1,4]=%lld sum[2,3]=%lld\n",seg.query(1,4,1,n,1),seg.query(2,3,1,n,1));
    return 0;
}
