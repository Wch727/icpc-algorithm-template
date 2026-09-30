#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=3000006;

ll n,p;
ll inv[N],fact[N],inv_fact[N];

// O(log p)，费马小定理求逆元，要求 p 为素数且 gcd(a,p)=1
ll qpow(ll a,ll n,ll mod)
{
    ll res=1;
    a%=mod;
    while(n)
    {
        if(n&1)(res*=a)%=mod;
        (a*=a)%=mod;
        n>>=1;
    }
    return res;
}

ll inv_fermat(ll a,ll p)
{
    return qpow(a,p-2,p);
}

// O(log p)，exgcd 求逆元，只需 gcd(a,p)=1
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b){x=1,y=0;return a;}
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy,y=xx-(a/b)*yy;
    return g;
}

ll inv_exgcd(ll a,ll p)
{
    ll x,y;
    exgcd(a,p,x,y);
    return (x%p+p)%p;
}

// O(n)，线性递推求 1..n 的全部逆元（p 为素数且 n<p）
// inv[i] = -(p/i) * inv[p%i] (mod p)
void inv_init(int n,ll p)
{
    inv[1]=1;
    for(int i=2;i<=n;i++)
        inv[i]=((p-p/i*inv[p%i])%p+p)%p;
}

// O(n)，阶乘与阶乘逆元预处理，配合组合数用
void fact_init(int n,ll p)
{
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%p;
    inv_fact[n]=qpow(fact[n],p-2,p);
    for(int i=n;i>=1;i--)inv_fact[i-1]=inv_fact[i]*i%p;
}

// O(1)，要求 n<p 且 p 为素数
ll C_small(ll n,ll m,ll p)
{
    if(m<0||m>n||n<0)return 0;
    return fact[n]*inv_fact[m]%p*inv_fact[n-m]%p;
}
