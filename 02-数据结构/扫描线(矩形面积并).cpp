// 扫描线求矩形面积并、周长并，O(n log n)
// 周长：两个方向分别累计覆盖长度变化，同坐标先加入后删除
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n;
int rx1[N],ry1[N],rx2[N],ry2[N],ys[N<<1];

struct ScanTree{
    ll cov[N<<3],len[N<<3],num[N<<3];
    bool lc[N<<3],rc[N<<3];
    int *ys;
    void init(int *y){ys=y;}
    void clear(int cnt)
    {
        for(int i=1;i<=(cnt<<2);i++)cov[i]=len[i]=num[i]=0,lc[i]=rc[i]=0;
    }
    void push_up(int p,int l,int r)
    {
        if(cov[p]>0)
        {
            len[p]=(ll)ys[r+1]-ys[l];
            num[p]=1,lc[p]=rc[p]=1;
        }
        else if(l==r)len[p]=num[p]=0,lc[p]=rc[p]=0;
        else
        {
            int lp=p<<1,rp=p<<1|1;
            len[p]=len[lp]+len[rp];
            num[p]=num[lp]+num[rp]-(rc[lp]&&lc[rp]);
            lc[p]=lc[lp],rc[p]=rc[rp];
        }
    }
    void update(int L,int R,ll v,int l,int r,int p)
    {
        if(L<=l&&r<=R)
        {
            cov[p]+=v;
            push_up(p,l,r);
            return;
        }
        int mid=(l+r)>>1;
        if(L<=mid)update(L,R,v,l,mid,p<<1);
        if(R>mid)update(L,R,v,mid+1,r,p<<1|1);
        push_up(p,l,r);
    }
}seg;

struct Ev{
    int x,lo,hi,typ;
    bool operator<(const Ev &o)const{return x!=o.x?x<o.x:typ>o.typ;}
}ev[N<<1];
struct SweepRes{ll area,diff;};

SweepRes sweep(int *a1,int *b1,int *a2,int *b2)
{
    int ycnt=0,ec=0;
    for(int i=1;i<=n;i++)
    {
        if(a1[i]>=b1[i]||a2[i]>=b2[i])continue;
        ys[++ycnt]=a2[i],ys[++ycnt]=b2[i];
        ev[++ec]={a1[i],a2[i],b2[i],1};
        ev[++ec]={b1[i],a2[i],b2[i],-1};
    }
    SweepRes ans{0,0};
    if(!ec)return ans;
    sort(ys+1,ys+ycnt+1);
    ycnt=unique(ys+1,ys+ycnt+1)-ys-1;
    sort(ev+1,ev+ec+1);
    seg.init(ys),seg.clear(ycnt);
    for(int i=1;i<=ec;i++)
    {
        if(i>1)ans.area+=seg.len[1]*((ll)ev[i].x-ev[i-1].x);
        int l=lower_bound(ys+1,ys+ycnt+1,ev[i].lo)-ys;
        int r=lower_bound(ys+1,ys+ycnt+1,ev[i].hi)-ys-1;
        ll last=seg.len[1];
        seg.update(l,r,ev[i].typ,1,ycnt-1,1);
        ans.diff+=llabs(seg.len[1]-last);
    }
    return ans;
}

ll union_area()
{
    return sweep(rx1,rx2,ry1,ry2).area;
}

ll union_perimeter()
{
    SweepRes a=sweep(rx1,rx2,ry1,ry2),b=sweep(ry1,ry2,rx1,rx2);
    return a.diff+b.diff;
}

const int TESTN=100005;
int g[64][64];
