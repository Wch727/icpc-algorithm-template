// 适用：模意义下除法、组合数；除以 a 等价于乘逆元，但逆元存在需 gcd(a,p)=1。
// 参数：模数 p>1；费马及线性递推版要求素数 p，指数非负，输入余数先归一。
// 下标：inv 为 1..n，fact/inv_fact 为 0..n，0<=n<N 且 n<p。
// 关键：阶乘逆元从最高下标逆推，只需一次快速幂，查询 O(1)。
// 易错：inv_exgcd 未检查 gcd；模数变化后阶乘表必须重建；ll 乘积需保证不溢出。
// 复杂度：单个逆元 O(log p)，批量 O(n)，阶乘预处理 O(n+log p)，空间 O(n)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=3000006;

ll n,p;
ll inv[N],fact[N],inv_fact[N];

// O(log p)，费马小定理求逆元，要求 p 为素数且 gcd(a,p)=1
// O(log n)，计算 a^n mod mod；函数本身是幂，求逆需指数 p-2。
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

// O(log p)，返回 a 的逆元；p 为素数且 a 不是 p 的倍数。
ll inv_fermat(ll a,ll p)
{
    return qpow(a,p-2,p);
}

// O(log p)，exgcd 求逆元，只需 gcd(a,p)=1
// O(log min(a,b))，返回 gcd，引用 x/y 输出 ax+by=gcd。
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b){x=1,y=0;return a;}
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy,y=xx-(a/b)*yy;
    return g;
}

// O(log p)，返回最小非负逆元；a 建议传非负余数，互素前提由调用者保证。
ll inv_exgcd(ll a,ll p)
{
    ll x,y;
    // O(log min(a,b))，返回 gcd，引用 x/y 输出 ax+by=gcd。
    exgcd(a,p,x,y);
    return (x%p+p)%p;
}

// O(n)，线性递推求 1..n 的全部逆元（p 为素数且 n<p）
// inv[i] = -(p/i) * inv[p%i] (mod p)
// O(n)，求 inv[1..n]；至少 n>=1，递推访问 p%i<i 的已求逆元。
void inv_init(int n,ll p)
{
    inv[1]=1;
    for(int i=2;i<=n;i++)
        inv[i]=((p-p/i*inv[p%i])%p+p)%p;
}

// O(n)，阶乘与阶乘逆元预处理，配合组合数用
// O(n+log p)，n 是预处理上界，p 为素数；fact[n] 不得为 0。
void fact_init(int n,ll p)
{
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%p;
    inv_fact[n]=qpow(fact[n],p-2,p);
    for(int i=n;i>=1;i--)inv_fact[i-1]=inv_fact[i]*i%p;
}

// O(1)，要求 n<p 且 p 为素数
// O(1)，返回 C(n,m) mod p；n 须在同模数预处理范围内，非法 m 返回 0。
ll C_small(ll n,ll m,ll p)
{
    if(m<0||m>n||n<0)return 0;
    return fact[n]*inv_fact[m]%p*inv_fact[n-m]%p;
}
