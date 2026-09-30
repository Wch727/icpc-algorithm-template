#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD=1000000007;
const int N=100005;
ll fac[N],inv[N];

ll qpow(ll a,ll n)
{
    ll r=1;
    while(n)
    {
        if(n&1)r=r*a%MOD;
        a=a*a%MOD,n>>=1;
    }
    return r;
}

// n<MOD，O(n) 预处理，O(1) 组合数
void init(int n)
{
    assert(n<N);
    fac[0]=1;
    for(int i=1;i<=n;i++)fac[i]=fac[i-1]*i%MOD;
    inv[n]=qpow(fac[n],MOD-2);
    for(int i=n;i>=1;i--)inv[i-1]=inv[i]*i%MOD;
}

ll C(int n,int m)
{
    if(m<0||m>n)return 0;
    return fac[n]*inv[m]%MOD*inv[n-m]%MOD;
}

// O(n)，容斥排除固定点；n=0 时空排列有一种
ll derange(int n)
{
    ll ans=0;
    for(int i=0;i<=n;i++)
    {
        ll x=C(n,i)*fac[n-i]%MOD;
        ans=(ans+(i&1?MOD-x:x))%MOD;
    }
    return ans;
}

ll multiset_count(const vector<int> &cnt)
{
    int n=accumulate(cnt.begin(),cnt.end(),0);
    ll ans=fac[n];
    for(int x:cnt)ans=ans*inv[x]%MOD;
    return ans;
}

// O(m*2^m)，[1,n] 中不被任一 d 整除的数；d 必须为正
ll avoid(ll n,const vector<ll> &d)
{
    int m=d.size();
    assert(m<25);
    ll ans=n;
    for(int s=1;s<(1<<m);s++)
    {
        ll l=1;
        for(int i=0;i<m;i++)if(s>>i&1)
        {
            assert(d[i]>0);
            ll x=d[i]/gcd(l,d[i]);
            if(l>n/x){l=n+1;break;}
            l*=x;
        }
        ll x=n/l;
        if(__builtin_popcount((unsigned)s)&1)ans-=x;
        else ans+=x;
    }
    return ans;
}
