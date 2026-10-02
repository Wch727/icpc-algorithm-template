// 区间 chmin/chmax/add/sum，0-indexed 闭区间；同时维护严格次大/次小、极值个数。
// chmin 只有 max2<x<max1 才能整段更新；否则递归。chmax 对称；加法移动全部极值。
// 含 add 的通用 Beats 标准摊还界 O((n+q)log²n)，空间 O(n)。值/和远离 ±INF。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
struct Beats
{
    static constexpr ll INF=LLONG_MAX/4;
    struct Node{ll sum=0,mx=-INF,mx2=-INF,mn=INF,mn2=INF,add=0;int cmx=0,cmn=0;};
    int n;vector<Node> t;
    Beats(const vector<ll> &a):n(a.size()),t(max(1,4*n)){if(n)build(1,0,n-1,a);}
    void pull(int p)
    {
        auto &a=t[p*2],&b=t[p*2+1],&c=t[p];c.sum=a.sum+b.sum;
        c.mx=max(a.mx,b.mx);c.cmx=(a.mx==c.mx?a.cmx:0)+(b.mx==c.mx?b.cmx:0);
        c.mx2=max(a.mx==c.mx?a.mx2:a.mx,b.mx==c.mx?b.mx2:b.mx);
        c.mn=min(a.mn,b.mn);c.cmn=(a.mn==c.mn?a.cmn:0)+(b.mn==c.mn?b.cmn:0);
        c.mn2=min(a.mn==c.mn?a.mn2:a.mn,b.mn==c.mn?b.mn2:b.mn);
    }
    void build(int p,int l,int r,const vector<ll> &a){if(l==r){t[p].sum=t[p].mn=t[p].mx=a[l];t[p].cmn=t[p].cmx=1;return;}int m=(l+r)/2;build(p*2,l,m,a);build(p*2+1,m+1,r,a);pull(p);}
    void plus(int p,int len,ll x){auto &a=t[p];a.sum+=len*x;a.mx+=x;a.mn+=x;if(a.mx2!=-INF)a.mx2+=x;if(a.mn2!=INF)a.mn2+=x;a.add+=x;}
    void lower(int p,ll x){auto &a=t[p];if(a.mx<=x)return;a.sum+=(x-a.mx)*a.cmx;if(a.mn==a.mx)a.mn=x;else if(a.mn2==a.mx)a.mn2=x;a.mx=x;}
    void upper(int p,ll x){auto &a=t[p];if(a.mn>=x)return;a.sum+=(x-a.mn)*a.cmn;if(a.mx==a.mn)a.mx=x;else if(a.mx2==a.mn)a.mx2=x;a.mn=x;}
    void push(int p,int l,int r)
    {
        int m=(l+r)/2;if(t[p].add){plus(p*2,m-l+1,t[p].add);plus(p*2+1,r-m,t[p].add);t[p].add=0;}
        for(int c:{p*2,p*2+1}){lower(c,t[p].mx);upper(c,t[p].mn);}
    }
    // op=0 加 x，op=1 chmin(x)，op=2 chmax(x)。调用区间必须在 [0,n)。
    void update(int p,int l,int r,int L,int R,ll x,int op)
    {
        if(R<l||r<L||(op==1&&t[p].mx<=x)||(op==2&&t[p].mn>=x))return;
        if(L<=l&&r<=R)
        {
            if(op==0){plus(p,r-l+1,x);return;}
            if(op==1&&t[p].mx2<x){lower(p,x);return;}
            if(op==2&&t[p].mn2>x){upper(p,x);return;}
        }
        push(p,l,r);int m=(l+r)/2;update(p*2,l,m,L,R,x,op);update(p*2+1,m+1,r,L,R,x,op);pull(p);
    }
    ll query(int p,int l,int r,int L,int R){if(R<l||r<L)return 0;if(L<=l&&r<=R)return t[p].sum;push(p,l,r);int m=(l+r)/2;return query(p*2,l,m,L,R)+query(p*2+1,m+1,r,L,R);}
    void update(int l,int r,ll x,int op){assert(0<=l&&l<=r&&r<n);update(1,0,n-1,l,r,x,op);}
    ll sum(int l,int r){assert(0<=l&&l<=r&&r<n);return query(1,0,n-1,l,r);}
};
