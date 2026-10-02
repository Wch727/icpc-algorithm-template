// 适用：线性丢番图方程、逆元、同余方程；有解条件是 gcd 整除常数项。
// 参数：a/b 为整数系数，x/y 是引用输出；solve_cong 的 m 必须为正。
// 关键：由 b*x1+(a%b)*y1=g 回代成 a*y1+b*(x1-(a/b)*y1)=g。
// 结论：ax=b (mod m) 的解间隔是 m/g；返回一个最小非负解，不是全部解。
// exgcd 返回非负 gcd；系数中间值须在 ll 范围，不能用 (0,0) 的 gcd 作除数。
// 复杂度：欧几里得 O(log min(
// a
// ,
// b
// ))，递归空间同量级；逆元函数不会检查互素。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// O(log min(|a|,|b|))，求 ax+by=gcd(a,b) 的一组解，返回 gcd
// |a|、|b|<=LLONG_MAX；返回非负 gcd，含负系数时仍保持等式。
// O(log min(|a|,|b|))，输出系数 x/y；a=b=0 时返回 0，不能随后拿来做除数。
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b)
    {
        x=a<0?-1:1,y=0;
        return a<0?-a:a;
    }
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy;
    y=xx-(a/b)*yy;
    return g;
}

// 同余方程 ax === b (mod m)，返回最小非负解，无解返回 -1
// 解集：x = x0 + k*(m/g)
// O(log m)，a 是系数、b 是目标余数，m 是正模数；无解返回 -1。
ll solve_cong(ll a,ll b,ll m)
{
    a%=m;if(a<0)a+=m;
    b%=m;if(b<0)b+=m;
    ll x,y;
    ll g=exgcd(a,m,x,y);
    if(b%g)return -1;// 裴蜀定理：g|b 才有解
    ll t=m/g;
    x=(ll)((__int128)x*(b/g)%t);
    x=(x%t<0?x%t+t:x%t);
    return x;
}

// O(log p)，模 p 下的逆元，要求 gcd(a,p)=1，返回最小非负解
// O(log p)，gcd(a,p)=1 且 a>=0、p>1 时返回逆元代表。
ll inv_exgcd(ll a,ll p)
{
    ll x,y;
    exgcd(a,p,x,y);
    return (x%p<0?x%p+p:x%p);
}

// 裴蜀定理：a,b 能线性组合出的最小正整数为 gcd(a,b)
// O(log min(|a|,|b|))，当前直接返回 exgcd 的值；负输入不能保证是最小正整数。
ll bezout_min(ll a,ll b)
{
    ll x,y;
    return exgcd(a,b,x,y);
}
