#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// O(log min(|a|,|b|))，求 ax+by=gcd(a,b) 的一组解，返回 gcd
// a,b 同负时返回 -gcd(a,b)，等式仍成立；只关心正 gcd 时取 abs
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b)
    {
        x=1,y=0;
        return a;
    }
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy;
    y=xx-(a/b)*yy;
    return g;
}

// 同余方程 ax === b (mod m)，返回最小非负解，无解返回 -1
// 解集：x = x0 + k*(m/g)
ll solve_cong(ll a,ll b,ll m)
{
    a%=m,a=(a+m)%m;
    b%=m,b=(b+m)%m;
    ll x,y;
    ll g=exgcd(a,m,x,y);
    if(b%g)return -1;// 裴蜀定理：g|b 才有解
    ll t=m/g;
    x=(ll)((__int128)x*(b/g)%t);
    x=(x%t+t)%t;
    return x;
}

// O(log p)，模 p 下的逆元，要求 gcd(a,p)=1，返回最小非负解
ll inv_exgcd(ll a,ll p)
{
    ll x,y;
    exgcd(a,p,x,y);
    return (x%p+p)%p;
}

// 裴蜀定理：a,b 能线性组合出的最小正整数为 gcd(a,b)
ll bezout_min(ll a,ll b)
{
    ll x,y;
    return exgcd(a,b,x,y);
}
