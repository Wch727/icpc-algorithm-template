#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

struct Line
{
    ll a,b,c;
    Line(ll a_=0,ll b_=0,ll c_=0):a(a_),b(b_),c(c_){}
};

// 分母为正；交叉乘积必须在 __int128 范围内
bool better(const Line &a,const Line &b,ll x)
{
    if(!a.c)return false;
    if(!b.c)return true;
    return ((lll)a.a*x+a.b)*b.c>((lll)b.a*x+b.b)*a.c;
}

// 动态开点，连续整数域与离散坐标共用；每次操作 O(log V)
struct LiChao
{
    struct Node
    {
        int lc=0,rc=0;
        Line s;
    };
    vector<Node> tr;
    vector<ll> xs;
    ll lo,hi;
    void init(ll l,ll r)
    {
        assert(l<=r);
        lo=l,hi=r,xs.clear(),tr.assign(2,Node());
    }
    void init(vector<ll> v)
    {
        assert(!v.empty());
        sort(v.begin(),v.end());
        v.erase(unique(v.begin(),v.end()),v.end());
        init(0,(ll)v.size()-1);
        xs=move(v);
    }
    ll coord(ll x)
    {
        return xs.empty()?x:xs[x];
    }
    void insert(Line s)
    {
        assert(s.c>0);
        insert(s,lo,hi,1);
    }
    void insert(Line s,ll l,ll r,int p)
    {
        if(!tr[p].s.c)
        {
            tr[p].s=s;
            return;
        }
        ll mid=(ll)((lll)l+((lll)r-l)/2);
        bool bl=better(s,tr[p].s,coord(l)),bm=better(s,tr[p].s,coord(mid));
        if(bm)swap(s,tr[p].s);
        if(l==r)return;
        bool left=bl!=bm;
        int q=left?tr[p].lc:tr[p].rc;
        if(!q)
        {
            q=(int)tr.size();
            tr.push_back(Node());
            if(left)tr[p].lc=q;
            else tr[p].rc=q;
        }
        if(left)insert(s,l,mid,q);
        else insert(s,mid+1,r,q);
    }
    // 返回最优直线，避免把分数截断成整数
    Line query(ll x)
    {
        ll id=x;
        if(!xs.empty())
        {
            id=lower_bound(xs.begin(),xs.end(),x)-xs.begin();
            assert(id<(ll)xs.size()&&xs[id]==x);
        }
        assert(lo<=id&&id<=hi);
        ll l=lo,r=hi;
        int p=1;
        Line ans;
        while(p)
        {
            if(better(tr[p].s,ans,x))ans=tr[p].s;
            if(l==r)break;
            ll mid=(ll)((lll)l+((lll)r-l)/2);
            if(id<=mid)p=tr[p].lc,r=mid;
            else p=tr[p].rc,l=mid+1;
        }
        return ans;
    }
};
