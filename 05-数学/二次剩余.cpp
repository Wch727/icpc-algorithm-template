// 适用：求模素数平方根 x^2=a；有非零解时通常是 x 与 p-x 两个根。
// 参数：p 为素数；a 可负，入口归一化；模 2 特判，零的平方根只有零。
// 状态：P/W 是一次 Cipolla 运算使用的全局模数和非剩余，不能并发复用。
// 关键：找 b 使 b^2-a 非剩余，在扩域中求 (b+sqrt(W))^((p+1)/2)。
// 易错：__int128 保护乘法，但双乘积相加和 ll 归一化也要在范围内；返回根不保证最小。
// 复杂度：随机找非剩余期望常数轮，每轮 O(log p)，加一次扩域幂；朴素核对只用于小 p。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

// O(log p)，快速幂，模乘走 __int128
// O(log n)，a 是底数、n>=0 是指数，p>0，模乘先升为 __int128。
ll qpow(ll a,ll n,ll p)
{
    ll res=1%p;
    a%=p;
    while(n)
    {
        if(n&1)res=(ll)((lll)res*a%p);
        a=(ll)((lll)a*a%p);
        n>>=1;
    }
    return res;
}

// O(log p)，勒让德符号：1 是二次剩余，-1 是非剩余，0 表示 a≡0
// 用欧拉判别法 a^((p-1)/2)
// O(log p)，返回 0/1/-1；欧拉判别只对奇素数 p 成立。
int legendre(ll a,ll p)
{
    a=(a%p+p)%p;
    if(a==0)return 0;
    ll t=qpow(a,(p-1)/2,p);
    return t==1?1:-1;
}

// Cipolla 用的扩域 F_p[sqrt(w)] 上的数：real + imag*sqrt(w)
struct Cip{
    ll real,imag;
};

ll P,W;// P 是模数，W 是扩域里的非剩余

// O(1)，扩域乘法 (a+b√w)(c+d√w) = (ac+bdw) + (ad+bc)√w
// O(1)，x/y 是扩域数，以全局 P/W 相乘；虚部平方用 W 代替。
Cip mul(Cip x,Cip y)
{
    Cip r;
    r.real=((lll)x.real*y.real+(lll)x.imag*y.imag%P*W)%P;
    r.imag=((lll)x.real*y.imag+(lll)x.imag*y.real)%P;
    r.real=(r.real%P+P)%P;
    r.imag=(r.imag%P+P)%P;
    return r;
}

// O(log p)，扩域快速幂
// O(log n)，a 是扩域底数，n 是非负指数，须先设置 P/W。
Cip cpw(Cip a,ll n)
{
    Cip res;
    res.real=1%P,res.imag=0;
    while(n)
    {
        if(n&1)res=mul(res,a);
        a=mul(a,a);
        n>>=1;
    }
    return res;
}

// O(log^2 p) 期望，Cipolla 算法：解 x^2 ≡ a (mod p)，p 为奇素数
// 返回一个解（另一个是 p-x），a 为非二次剩余时返回 -1
// 期望 O(log p) 次宽整数操作，返回一个根或 -1；b 的选择随机重试。
ll cipolla(ll a,ll p)
{
    a=(a%p+p)%p;
    if(a==0)return 0;
    if(p==2)return a;// 模 2 时 x=a 就是解
    if(legendre(a,p)!=1)return -1;// 无解判定
    if(p%4==3)return qpow(a,(p+1)/4,p);// p≡3 (mod 4) 有显式公式
    P=p;
    mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
    ll b;
    while(true)
    {
        b=rnd()%p;
        W=((lll)b*b-a)%p;// w = b^2 - a
        W=(W%p+p)%p;
        if(W!=0&&legendre(W,p)==-1)break;// w 必须是非二次剩余
    }
    Cip x;
    x.real=b,x.imag=1;
    Cip r=cpw(x,(p+1)/2);// (b+√w)^((p+1)/2) 的虚部必为 0
    return r.real;
}

// O(p)，暴力找最小非负解，无解返回 -1，对拍用
// O(p)，逐个检查 x=0..p-1；返回最小根，不能直接要求与随机根相等。
ll cipolla_naive(ll a,ll p)
{
    a=(a%p+p)%p;
    for(ll x=0;x<p;x++)
        if((lll)x*x%p==a)return x;
    return -1;
}

// O(p)，暴力数二次剩余个数，检查 (p-1)/2 个
// O(p^2)，逐余数枚举平方根，原 O(p) 注释与两层循环不符。
int count_qr_naive(ll p)
{
    int c=0;
    for(ll x=1;x<p;x++)
    {
        bool ok=false;
        for(ll y=0;y<p;y++)
            if((lll)y*y%p==x%p){ok=true;break;}
        if(ok)c++;
    }
    return c;
}
