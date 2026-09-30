#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

ll qpow(ll a,ll n,ll p)
{
    ll ans=1%p;
    for(a%=p;n;n>>=1,a=(lll)a*a%p)if(n&1)ans=(lll)ans*a%p;
    return ans;
}

ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b){x=1,y=0;return a;}
    ll u,v,g=exgcd(b,a%b,u,v);
    x=v,y=u-a/b*v;
    return g;
}

ll inv_mod(ll a,ll p)
{
    ll x,y;
    exgcd(a,p,x,y);
    return (x%p+p)%p;
}

// O(sqrt(p) log p)，互素离散对数，返回最小非负指数
ll bsgs(ll a,ll b,ll p)
{
    assert(p>=1);
    if(p==1)return 0;
    a=(a%p+p)%p,b=(b%p+p)%p;
    if(b==1)return 0;
    if(gcd(a,p)!=1)return -1;// 非互素请用扩展版本
    ll m=sqrtl(p)+1,cur=1;
    vector<pair<ll,ll> > v;
    for(ll j=0;j<m;j++)v.push_back({cur,j}),cur=(lll)cur*a%p;
    sort(v.begin(),v.end());
    v.erase(unique(v.begin(),v.end(),[](pair<ll,ll> x,pair<ll,ll> y){return x.first==y.first;}),v.end());
    ll step=inv_mod(qpow(a,m,p),p);
    cur=b;
    for(ll i=0;i<=m;i++,cur=(lll)cur*step%p)
    {
        int j=lower_bound(v.begin(),v.end(),make_pair(cur,-1LL))-v.begin();
        if(j<(int)v.size()&&v[j].first==cur)return i*m+v[j].second;
    }
    return -1;
}

// 消去公因子后做 BSGS，p>=1，约定 a^0=1
ll exbsgs(ll a,ll b,ll p)
{
    assert(p>=1);
    if(p==1)return 0;
    a=(a%p+p)%p,b=(b%p+p)%p;
    if(b==1)return 0;
    ll cnt=0,mul=1,g;
    while((g=gcd(a,p))>1)
    {
        if(b%g)return -1;
        b/=g,p/=g,cnt++;
        mul=(lll)mul*(a/g)%p;
        if(mul==b)return cnt;
    }
    ll ans=bsgs(a,(lll)b*inv_mod(mul,p)%p,p);
    return ans<0?-1:ans+cnt;
}
