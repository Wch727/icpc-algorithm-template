// C(n,k) mod m，n,k 非负 64 位，1<=m<=2^31-1；分解素数幂 + 去 p 因子阶乘 + CRT。
// 每个素数幂 pk 需 O(pk) 预处理，故要求 pk<=2e6（可按内存调整）；查询 O(log_p n * log pk)。
// n 极大、pk 极大时不能直接用本版；普通素数且小 n 优先原组合数模板。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;using U=unsigned long long;
ll exgcd(ll a,ll b,ll &x,ll &y){if(!b){x=1;y=0;return a;}ll z=exgcd(b,a%b,y,x);y-=a/b*x;return z;}
ll inverse(ll a,ll m){ll x,y;assert(exgcd(a,m,x,y)==1);return (x%m+m)%m;}
ll power(ll a,U b,ll m){ll z=1;for(;b;b>>=1,a=a*a%m)if(b&1)z=z*a%m;return z;}
ll binomial_prime_power(U n,U k,int p,int pk)
{
    vector<ll> f(pk+1,1);for(int i=1;i<=pk;i++)f[i]=f[i-1]*(i%p?i:1)%pk;
    auto fact=[&](U x){ll ans=1;while(x){ans=ans*power(f[pk],x/pk,pk)%pk*f[x%pk]%pk;x/=p;}return ans;};
    auto exponent=[&](U x){U z=0;while(x)z+=x/=p;return z;};
    U e=exponent(n)-exponent(k)-exponent(n-k);int q=0;for(int x=pk;x>1;x/=p)q++;
    if(e>=(U)q)return 0;
    return fact(n)*inverse(fact(k),pk)%pk*inverse(fact(n-k),pk)%pk*power(p,e,pk)%pk;
}
ll exlucas(U n,U k,int m)
{
    assert(m>0);if(k>n||m==1)return 0;int rest=m;ll ans=0;
    for(int p=2;(ll)p*p<=rest;p++)if(rest%p==0)
    {int pk=1;while(rest%p==0)rest/=p,pk*=p;assert(pk<=2000000);ll v=binomial_prime_power(n,k,p,pk),M=m/pk;ans=(ans+(__int128)v*M*inverse(M%pk,pk))%m;}
    if(rest>1){assert(rest<=2000000);ll v=binomial_prime_power(n,k,rest,rest),M=m/rest;ans=(ans+(__int128)v*M*inverse(M%rest,rest))%m;}
    return ans;
}
