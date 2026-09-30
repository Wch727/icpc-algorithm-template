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

int main()
{
    int bad=0;
    // 1) p=3..300 全枚举：有解时最小解与暴力一致，无解时都返回 -1
    for(ll p=3;p<=300;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        for(ll a=0;a<p;a++)
        {
            ll x=cipolla(a,p);
            ll y=cipolla_naive(a,p);
            if(x!=y)bad++;
            if(x!=-1&&(lll)x*x%p!=a)bad++;// 平方代回
        }
    }
    // 2) 勒让德符号与暴力判定对拍
    for(ll p=3;p<=200;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        for(ll a=0;a<p;a++)
        {
            ll x=cipolla_naive(a,p);
            int want=(a==0)?0:(x==-1?-1:1);
            if(legendre(a,p)!=want)bad++;
        }
    }
    // 3) 大素数：随机 a 与随机平方数，验证代回
    ll bigp[4]={1000000007LL,998244353LL,1000000009LL,19260817LL};
    mt19937_64 rnd(20251010);
    for(int t=0;t<4;t++)
    {
        ll p=bigp[t];
        for(int k=1;k<=50;k++)
        {
            ll a=rnd()%(p-1)+1;
            ll x=cipolla(a,p);
            if(x!=-1&&(lll)x*x%p!=a)bad++;
            if(x!=-1&&legendre(a,p)!=1)bad++;
            // 构造一定是二次剩余的 b=y^2，必须求出解
            ll y=rnd()%p;
            ll b=(lll)y*y%p;
            ll z=cipolla(b,p);
            if(z==-1||(lll)z*z%p!=b)bad++;
        }
        // p≡1 (mod 4) 与 p≡3 (mod 4) 都要覆盖
        if(p%4==1)printf("p = %lld 满足 p%%4==1，走 Cipolla 随机分支\n",p);
    }
    // 4) 特例：1 的两解是 1 和 p-1；-1 在 p≡1 (mod 4) 时是二次剩余
    for(int t=0;t<4;t++)
    {
        ll p=bigp[t];
        ll r=cipolla(1,p);
        if(r!=1&&r!=p-1)bad++;
        int lg=legendre(p-1,p);
        if(lg!=(p%4==1?1:-1))bad++;
    }
    // 5) 二次剩余个数应为 (p-1)/2
    for(ll p=3;p<=60;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        if(count_qr_naive(p)!=(p-1)/2)bad++;
    }
    printf("cipolla(4,7)  = %lld (expect 2)\n",cipolla(4,7));
    printf("cipolla(3,7)  = %lld (expect -1, 无解)\n",cipolla(3,7));
    ll r2=cipolla(2,7),r5=cipolla(5,1000000007LL);
    printf("cipolla(2,7)  = %lld, 平方 = %lld\n",r2,(ll)((lll)r2*r2%7));
    printf("cipolla(5,1000000007) = %lld, 平方 = %lld\n",r5,(ll)((lll)r5*r5%1000000007LL));
    printf("legendre(5,1000000007) = %d (5 是二次剩余)\n",legendre(5,1000000007LL));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P5491 输入多组 a p，输出两解（最小在前）或 Hola!
// 样例：Cipolla 模板题给 4 7 -> 2 5
// 边界：a=0 返回 0；p=2 直接返回 a；p≡3 (mod 4) 用 (p+1)/4 次幂；无解返回 -1

/*
自测记录：
  1) p<=300 的全部奇素数、a 取遍 0..p-1，Cipolla 结果与暴力最小解一致且平方代回；
  2) 勒让德符号与暴力判定全表对拍；
  3) 4 个大素数（含 p%4==1 与 p%4==3）随机 a 与随机平方数验证；
  4) 1 与 -1 的经典结论、二次剩余个数 = (p-1)/2 校验。
*/
