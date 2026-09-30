#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

// O(log p)，快速幂，模乘走 __int128
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
ll cipolla_naive(ll a,ll p)
{
    a=(a%p+p)%p;
    for(ll x=0;x<p;x++)
        if((lll)x*x%p==a)return x;
    return -1;
}

// O(p)，暴力数二次剩余个数，检查 (p-1)/2 个
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
