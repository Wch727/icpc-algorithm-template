#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// O(log)，扩展欧几里得
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b){x=1,y=0;return a;}
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy,y=xx-(a/b)*yy;
    return g;
}

// 模数两两互质版 CRT：x === a[i] (mod m[i])，返回最小非负解
// 用前缀积 p[i]=m[1..i]、后缀积 s[i]=m[i..n] 把 M/m[i] 控制在 1e18 内
// 总模数乘积应 <= 1e18，否则解可能溢出
ll crt(int n,ll a[],ll m[])
{
    static ll p[N],s[N];
    p[0]=1;
    for(int i=1;i<=n;i++)p[i]=p[i-1]*m[i];
    s[n+1]=1;
    for(int i=n;i>=1;i--)s[i]=s[i+1]*m[i];
    ll M=p[n],ans=0;
    for(int i=1;i<=n;i++)
    {
        ll Mi=p[i-1]*s[i+1];// M / m[i]
        ll x,y;
        exgcd(Mi%m[i],m[i],x,y);// Mi 在模 m[i] 下的逆元
        x=(x%m[i]+m[i])%m[i];
        ans=(ans+(__int128)a[i]*Mi%M*x)%M;
    }
    return (ans%M+M)%M;
}

// 扩展 CRT（模数不必互质）：合并 x === a1 (mod m1) 与 x === a2 (mod m2)
// 返回是否可合并；可合并时 a1 为新余数、m1 为新模数，均取最小非负
// O(log)
bool crt_merge(ll &a1,ll &m1,ll a2,ll m2)
{
    ll x,y;
    ll g=exgcd(m1,m2,x,y);
    ll d=a2-a1;
    if(d%g)return false;// 无解
    ll t=m2/g;
    x=(ll)((__int128)(d/g)%t*x%t);
    x=(x%t+t)%t;
    a1=a1+(__int128)m1*x;
    m1=m1/g*m2;// lcm
    a1=(a1%m1+m1)%m1;
    return true;
}

// O(n log)，扩展 CRT 数组版，无解返回 -1（要求 lcm 不超过 9e18）
ll crt_ex(int n,ll a[],ll m[])
{
    ll ra=a[1],rm=m[1];
    for(int i=2;i<=n;i++)
        if(!crt_merge(ra,rm,a[i],m[i]))return -1;
    return (ra%rm+rm)%rm;
}
