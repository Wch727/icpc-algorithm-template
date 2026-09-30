#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 静态二维 KD 树：中位数建树 O(n log n)，查询最坏 O(n)
// 坐标绝对值不超过 1e9，距离返回平方；空集返回 LLONG_MAX
struct KDTree
{
    struct Point { ll x,y; };
    struct Node { Point p; ll lo[2],hi[2]; int l=0,r=0,sz=0; };
    vector<Point> a;
    vector<Node> tr;
    int rt=0;
    KDTree(vector<Point> b):a(b),tr(b.size()+1) { rt=build(0,(int)a.size()-1,0); }
    int build(int l,int r,int d)
    {
        if(l>r)return 0;
        int mid=(l+r)>>1,p=mid+1;
        nth_element(a.begin()+l,a.begin()+mid,a.begin()+r+1,[d](Point u,Point v) { return d?u.y<v.y:u.x<v.x; });
        tr[p].p=a[mid],tr[p].l=build(l,mid-1,d^1),tr[p].r=build(mid+1,r,d^1);
        tr[p].lo[0]=tr[p].hi[0]=a[mid].x,tr[p].lo[1]=tr[p].hi[1]=a[mid].y,tr[p].sz=1;
        for(int v:{tr[p].l,tr[p].r})if(v)
        {
            tr[p].sz+=tr[v].sz;
            for(int k=0;k<2;k++)tr[p].lo[k]=min(tr[p].lo[k],tr[v].lo[k]),tr[p].hi[k]=max(tr[p].hi[k],tr[v].hi[k]);
        }
        return p;
    }
    ll dist(Point u,Point v) { return (u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y); }
    ll bound(int p,Point q)
    {
        if(!p)return LLONG_MAX;
        ll ans=0,b[2]={q.x,q.y};
        for(int k=0;k<2;k++)
        {
            ll d=max({tr[p].lo[k]-b[k],b[k]-tr[p].hi[k],0LL});
            ans+=d*d;
        }
        return ans;
    }
    void nearest(int p,Point q,ll &ans)
    {
        if(!p)return;
        ans=min(ans,dist(q,tr[p].p));
        int l=tr[p].l,r=tr[p].r;
        if(bound(l,q)>bound(r,q))swap(l,r);
        if(bound(l,q)<ans)nearest(l,q,ans);
        if(bound(r,q)<ans)nearest(r,q,ans);
    }
    ll nearest(Point q)
    {
        ll ans=LLONG_MAX;
        nearest(rt,q,ans);
        return ans;
    }
    int rectangle(int p,ll x1,ll y1,ll x2,ll y2)
    {
        if(!p||tr[p].hi[0]<x1||tr[p].lo[0]>x2||tr[p].hi[1]<y1||tr[p].lo[1]>y2)return 0;
        if(x1<=tr[p].lo[0]&&tr[p].hi[0]<=x2&&y1<=tr[p].lo[1]&&tr[p].hi[1]<=y2)return tr[p].sz;
        Point q=tr[p].p;
        int ans=x1<=q.x&&q.x<=x2&&y1<=q.y&&q.y<=y2;
        return ans+rectangle(tr[p].l,x1,y1,x2,y2)+rectangle(tr[p].r,x1,y1,x2,y2);
    }
    int rectangle(ll x1,ll y1,ll x2,ll y2)
    {
        if(x1>x2||y1>y2)return 0;
        return rectangle(rt,x1,y1,x2,y2);
    }
};
