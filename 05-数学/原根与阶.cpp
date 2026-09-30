#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
const int N=1000006;

int n;
int prime[N],cnt;// 素数表（顺便当最小质因子筛用）
int spf[N];// 最小质因子

// O(n)，线性筛，顺便记最小质因子
void get_prime(int n)
{
    cnt=0;
    for(int i=2;i<=n;i++)
    {
        if(!spf[i])prime[++cnt]=i,spf[i]=i;// i 是素数
        for(int j=1;j<=cnt&&(ll)i*prime[j]<=n;j++)
        {
            spf[i*prime[j]]=prime[j];
            if(i%prime[j]==0)break;// 只被最小质因子筛一次
        }
    }
}

// O(log n)，快速幂（模乘用 __int128 防溢出）
ll qpow(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    while(n)
    {
        if(n&1)res=(ll)((lll)res*a%mod);
        a=(ll)((lll)a*a%mod);
        n>>=1;
    }
    return res;
}

// O(sqrt n)，试除分解质因数（不要求 n 是素数），返回去重后的素因子
vector<ll> factor(ll n)
{
    vector<ll> v;
    for(ll i=2;i*i<=n;i++)
        if(n%i==0)
        {
            v.push_back(i);
            while(n%i==0)n/=i;
        }
    if(n>1)v.push_back(n);
    return v;
}

// O(sqrt(phi)) log 级，求 a 模 p 的阶，要求 gcd(a,p)=1
// 做法：ord | phi(p)，枚举 phi(p) 的素因子逐个除掉
ll get_order(ll a,ll p)
{
    if(__gcd(a,p)!=1)return -1;// 不互素没有阶
    if(p==1)return 1;
    ll ph=p-1;
    vector<ll> f=factor(ph);
    ll ord=ph;
    for(int i=0;i<(int)f.size();i++)
        while(ord%f[i]==0&&qpow(a,ord/f[i],p)==1)ord/=f[i];// 能除就除
    return ord;
}

// O(sqrt(p) + log^2 p)，求模 p 的最小原根（p 为素数，p=2 特判返回 1）
ll get_root(ll p)
{
    if(p==2)return 1;
    vector<ll> f=factor(p-1);
    for(ll g=2;g<p;g++)
    {
        bool ok=true;
        for(int i=0;i<(int)f.size();i++)
            if(qpow(g,(p-1)/f[i],p)==1)// 只要有一个为 1 就不是原根
            {
                ok=false;
                break;
            }
        if(ok)return g;
    }
    return -1;// p 不是素数时可能走到这里
}

// O(sqrt(p) log p)，原根判定：g 是模 p 原根 <=> ord_p(g)=p-1
bool is_root(ll g,ll p)
{
    if(__gcd(g,p)!=1)return false;
    return get_order(g,p)==p-1;
}

// O(p log p)，暴力求阶，对拍用
ll order_naive(ll a,ll p)
{
    if(__gcd(a,p)!=1)return -1;
    ll cur=1;
    for(ll k=1;k<=p;k++)
    {
        cur=(ll)((lll)cur*a%p);
        if(cur==1)return k;
    }
    return -1;
}

// O(p log p)，暴力判定原根：g^1..g^(p-1) 是否两两不同
bool is_root_naive(ll g,ll p)
{
    if(__gcd(g,p)!=1)return false;
    static int seen[N];
    for(int i=1;i<p;i++)seen[i]=0;
    ll cur=1;
    for(int k=1;k<p;k++)
    {
        cur=(ll)((lll)cur*g%p);
        if(seen[cur])return false;
        seen[cur]=1;
    }
    return true;
}

int main()
{
    get_prime(1000000);
    int bad=0;
    // 1) 1..300 范围内所有互素 (a,p) 的阶与暴力对拍
    for(ll p=2;p<=300;p++)
        for(ll a=1;a<p;a++)
        {
            if(__gcd(a,p)!=1)continue;
            if(get_order(a,p)!=order_naive(a,p))bad++;
        }
    // 2) 1..200 的原根判定与暴力对拍，并与最小原根一致
    for(ll p=2;p<=200;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        ll g=get_root(p);
        if(!is_root(g,p))bad++;
        if(!is_root_naive(g,p))bad++;
        for(ll x=1;x<p;x++)
        {
            if(is_root(x,p)!=is_root_naive(x,p))bad++;
            if(is_root(x,p)&&x<g)bad++;// g 必须是最小的那个
        }
    }
    // 3) 大素数：验算 g^d != 1 (d 为 (p-1) 的真因子) 且 g^(p-1)=1
    ll bigp[4]={1000000007LL,998244353LL,1000000009LL,19260817LL};
    for(int t=0;t<4;t++)
    {
        ll p=bigp[t];
        ll g=get_root(p);
        if(qpow(g,p-1,p)!=1)bad++;
        vector<ll> f=factor(p-1);
        for(int i=0;i<(int)f.size();i++)
            if(qpow(g,(p-1)/f[i],p)==1)bad++;
        if(get_order(g,p)!=p-1)bad++;
    }
    // 4) 阶必须整除 phi(p)，且 ord | d <=> a^d=1
    mt19937_64 rnd(20250606);
    for(int t=1;t<=300;t++)
    {
        ll p=rnd()%100000+2;
        if(__gcd((ll)t,p)!=1)continue;
        ll a=rnd()%(p-1)+1;
        if(__gcd(a,p)!=1)continue;
        ll ord=get_order(a,p);
        if((p-1)%ord!=0)bad++;
        if(qpow(a,ord,p)!=1)bad++;
    }
    printf("order(2,7) = %lld (expect 3)\n",get_order(2,7));
    printf("root(2..11) = ");
    for(ll p=2;p<=11;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(isp)printf("%lld ",get_root(p));
    }
    printf("\nroot(1e9+7) = %lld, root(998244353) = %lld\n",get_root(1000000007LL),get_root(998244353LL));
    printf("is_root(3,7) = %d, is_root(2,7) = %d\n",(int)is_root(3,7),(int)is_root(2,7));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P6091 输入 6 -> 原根 5，以及 5^k 的次幂表
// 样例：P3321 用最小原根把乘法转成加法
// 边界：p=2 的原根是 1；gcd(a,p)>1 时阶不存在返回 -1；p 非素数时 get_root 无意义

/*
自测记录：
  1) p<=300 的全部互素 (a,p) 求阶与 O(p) 暴力对拍；
  2) p<=200 的全部素数：原根判定与暴力「幂两两不同」对拍，且 get_root 给的是最小原根；
  3) 4 个大素数验算 g^(p-1)=1 且 g^((p-1)/q)!=1（q 取 p-1 的每个素因子）；
  4) 随机点验证阶整除 p-1 且 a^ord=1。
*/
