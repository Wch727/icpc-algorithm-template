#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int P=998244353;

ll qpow(ll a,ll b)
{
    ll ans=1;
    for(;b;b>>=1,a=a*a%P)if(b&1)ans=ans*a%P;
    return ans;
}

// 已知 y[i]=f(i)，次数不超过 n；n<P，O(n) 求 f(x)
int interpolate(vector<int> y,ll x)
{
    int n=(int)y.size()-1;
    x=(x%P+P)%P;
    if(x<=n)return y[x];
    vector<ll> pre(n+2,1),suf(n+2,1),fac(n+1,1),inv(n+1);
    for(int i=0;i<=n;i++)pre[i+1]=pre[i]*(x-i+P)%P;
    for(int i=n;i>=0;i--)suf[i]=suf[i+1]*(x-i+P)%P;
    for(int i=1;i<=n;i++)fac[i]=fac[i-1]*i%P;
    inv[n]=qpow(fac[n],P-2);
    for(int i=n;i>=1;i--)inv[i-1]=inv[i]*i%P;
    ll ans=0;
    for(int i=0;i<=n;i++)
    {
        ll v=pre[i]*suf[i+1]%P*inv[i]%P*inv[n-i]%P*y[i]%P;
        if((n-i)&1)ans=(ans-v+P)%P;
        else ans=(ans+v)%P;
    }
    return ans;
}

int eval(vector<int> a,ll x)
{
    x=(x%P+P)%P;
    ll ans=0;
    for(int i=(int)a.size()-1;i>=0;i--)ans=(ans*x+a[i])%P;
    return ans;
}
