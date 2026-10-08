// 适用：多个余数条件合并，普通 CRT 要模数两两互素，扩展版无需互素。
// 下标：a/m 的有效范围 1..n，n>=1；所有模数为正，a[i] 为对应余数。
// 结论：可行解按总模数或 lcm 周期重复，返回最小非负代表。
// 关键：扩展合并要求 gcd(m1,m2) 整除 a2-a1，再解出模 m2/g 的增量。
// 易错：总乘积、lcm、a1+m1*x 的存储都必须在 ll 范围内；__int128 中间值不代表最终赋值安全。
// 复杂度：n 次欧几里得合并约 O(n log M)，数组版空间 O(n)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// O(log)，扩展欧几里得
// O(log min(a,b))，x/y 引用返回裴蜀系数；模数使用正数。
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b)
    {
        x= 1, y= 0;
        return a;
    }
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy,y=xx-(a/b)*yy;
    return g;
}

// 模数两两互质版 CRT：x === a[i] (mod m[i])，返回最小非负解
// 用前缀积 p[i]=m[1..i]、后缀积 s[i]=m[i..n] 把 M/m[i] 控制在 1e18 内
// 总模数乘积应 <= 1e18，否则解可能溢出
// O(n log M)，合并 1..n 条互素同余；前后缀需 n+1 槽，输入乘积须可存。
int n;
ll a[N],m[N],p[N],s[N];
ll crt()
{

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
    return (ans%M<0?ans%M+M:ans%M);
}

// 扩展 CRT（模数不必互质）：合并 x === a1 (mod m1) 与 x === a2 (mod m2)
// 返回是否可合并；可合并时 a1 为新余数、m1 为新模数，均取最小非负
// O(log)
// O(log min(m1,m2))，成功时原地更新 a1/m1；失败返回 false，m1 是 lcm 周期。
bool crt_merge(ll &a1,ll &m1,ll a2,ll m2)
{
    a1%=m1;if(a1<0)a1+=m1;
    a2%=m2;if(a2<0)a2+=m2;
    ll x,y;
    ll g=exgcd(m1,m2,x,y);
    ll d=a2-a1;
    if(d%g)return false;// 无解
    ll t=m2/g;
    x=(ll)((__int128)(d/g)%t*x%t);
    x=(x%t<0?x%t+t:x%t);
    __int128 next_mod=(__int128)(m1/g)*m2;
    assert(next_mod<=LLONG_MAX);
    a1=(a1+(__int128)m1*x)%next_mod;
    m1=(ll)next_mod;// lcm
    a1=(a1%m1<0?a1%m1+m1:a1%m1);
    return true;
}

// O(n log)，扩展 CRT 数组版，无解返回 -1（要求 lcm 不超过 9e18）
// O(n log M)，输入模数可不互素，返回最小非负解或 -1；n 必须非零。
ll crt_ex()
{
    ll ra=a[1],rm=m[1];
    for(int i=2;i<=n;i++)
        if(!crt_merge(ra,rm,a[i],m[i]))return -1;
    return (ra%rm<0?ra%rm+rm:ra%rm);
}
