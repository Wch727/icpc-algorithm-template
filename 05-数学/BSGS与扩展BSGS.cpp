#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
const ll BAS=100000;// 小步的枚举上界，普通 BSGS 取 ceil(sqrt(p))
const ll H=100000;// 暴力搜索的上界，对拍用

// O(log n)，扩展欧几里得，返回 gcd 并解出 ax+by=g
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b){x=1,y=0;return a;}
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy,y=xx-(a/b)*yy;
    return g;
}

// O(log p)，a 在模 p 下的逆元（要求 gcd(a,p)=1）
ll inv_mod(ll a,ll p)
{
    ll x,y;
    exgcd(a%p,p,x,y);
    return (x%p+p)%p;
}

// O(log p)，快速幂，模乘全部走 __int128
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

// O(log p)，一种一定合法的特解：a*x≡b (mod p)，返回最小非负解，无解返回 -1
ll linear_cong(ll a,ll b,ll p)
{
    ll x,y;
    ll g=exgcd(a,p,x,y);
    if(b%g)return -1;
    ll mod=p/g;
    ll t=(ll)((lll)(b/g)%mod*((x%mod+mod)%mod)%mod);
    return t;
}

// O(sqrt p)，普通 BSGS：a^x ≡ b (mod p)，要求 gcd(a,p)=1且 p>1
// 返回最小的非负 x，无解返回 -1
ll bsgs(ll a,ll b,ll p)
{
    a%=p,b%=p;
    if(p==1)return 0;// 模 1 时一切为 0
    if(b==1)return 0;// 指数为 0
    if(a==0)return b==0?1:-1;// 0^x：x=0 得 1，x>=1 得 0
    ll m=ceil(sqrt((double)p));
    unordered_map<ll,ll> mp;// 存 a^j -> j，j 取最小的
    ll cur=1;
    for(ll j=0;j<m;j++)
    {
        if(!mp.count(cur))mp[cur]=j;
        cur=(ll)((lll)cur*a%p);
    }
    ll am_inv=inv_mod(qpow(a,m,p),p);// a^(-m)
    cur=b;
    for(ll i=0;i<=m;i++)
    {
        if(mp.count(cur))
        {
            ll x=i*m+mp[cur];
            if(x>=0)return x;
        }
        cur=(ll)((lll)cur*am_inv%p);
    }
    return -1;
}

// O(sqrt p + log p)，扩展 BSGS：a^x ≡ b (mod p)，p 不要求是素数
// 返回最小的非负 x，无解返回 -1
ll exbsgs(ll a,ll b,ll p)
{
    a%=p,b%=p;
    if(p==1)return 0;
    if(b==1||b%p==0)return 0;// x=0 时 a^0=1；b≡0 时 x=0 已满足（约定 0^0=1）
    ll g,cnt=0,mul=1;
    while((g=__gcd(a,p))>1)
    {
        if(b%g)return -1;// 每一步都要 g | b，否则无解
        cnt++;
        b/=g,p/=g;
        mul=(ll)((lll)mul*(a/g)%p);// 左边多出来的 a/g 因子
        if(mul==b)return cnt;// 恰好凑上，答案就是当前指数
        if(p==1)return cnt;
    }
    b=(ll)((lll)b*inv_mod(mul,p)%p);// 两边同乘 mul 的逆元
    ll t=bsgs(a,b,p);
    if(t==-1)return -1;
    return t+cnt;
}

// O(H)，暴力枚举指数，对拍用
ll bsgs_naive(ll a,ll b,ll p)
{
    a%=p,b%=p;
    if(p==1)return 0;
    ll cur=1%p;
    for(ll x=0;x<=H;x++)
    {
        if(cur==b)return x;
        cur=(ll)((lll)cur*a%p);
    }
    return -1;
}

// O(p)，暴力求所有解里最小的那个，对小模数足够（p<=50）
ll solve_min(ll a,ll b,ll p)
{
    a%=p,b%=p;
    if(p==1)return 0;
    ll cur=1%p;
    for(ll x=0;x<=4*p+100;x++)
    {
        if(cur==b)return x;
        cur=(ll)((lll)cur*a%p);
    }
    return -1;
}

int main()
{
    int bad=0;
    // 1) 素数模下的经典例子
    if(bsgs(2,8,13)!=3)bad++;// 2^3=8
    if(bsgs(3,13,17)!=4)bad++;// 3^4=81=13+4*17
    if(bsgs(2,1,1000000007)!=0)bad++;
    if(bsgs(2,0,1000000007)!=-1)bad++;// a 与 p 互素时 a^x 不会是 0
    // 2) 小模数全枚举：a,p<=40，b 取遍 0..p-1
    for(ll p=2;p<=40;p++)
        for(ll a=1;a<=40;a++)
            for(ll b=0;b<p;b++)
            {
                ll got=bsgs(a,b,p);
                ll want=solve_min(a,b,p);
                if(got!=want)bad++;
                if(got!=-1&&qpow(a,got,p)!=b%p)bad++;// 结果代回验证
            }
    // 3) 扩展 BSGS：p 非素数（含 2^k、合数）全枚举
    for(ll p=2;p<=40;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(isp&&p>2)continue;// 素数留给第 2 步
        for(ll a=1;a<=40;a++)
            for(ll b=0;b<p;b++)
            {
                ll got=exbsgs(a,b,p);
                ll want=solve_min(a,b,p);
                if(got!=want)bad++;
                if(got!=-1&&qpow(a,got,p)!=b%p)bad++;
            }
    }
    // 4) 大模数下用普通 BSGS 验证代回
    ll primes[4]={1000000007LL,998244353LL,1000000009LL,19260817LL};
    mt19937_64 rnd(20250909);
    for(int t=0;t<4;t++)
    {
        ll p=primes[t];
        for(int k=1;k<=20;k++)
        {
            ll x=rnd()%1000000+1;// 先造一个已知答案
            ll a=rnd()%(p-1)+1;
            ll b=qpow(a,x,p);
            ll got=bsgs(a,b,p);
            if(got==-1||got>x)bad++;// 找到的必须不超过已知解
            if(qpow(a,got,p)!=b)bad++;
        }
    }
    // 5) 扩展 BSGS 大模数：2^k 与合数模
    for(int t=0;t<40;t++)
    {
        ll p=(rnd()%12==0)?(1LL<<(rnd()%20+10)):(rnd()%100000+2);
        if(p<2)continue;
        ll a=rnd()%p+1;
        ll x=rnd()%1000+1;
        ll b=qpow(a,x,p);
        ll got=exbsgs(a,b,p);
        if(got==-1)bad++;
        else if(qpow(a,got,p)!=b)bad++;
    }
    printf("bsgs(2,8,13)  = %lld (expect 3)\n",bsgs(2,8,13));
    printf("bsgs(3,13,17) = %lld (expect 4)\n",bsgs(3,13,17));
    printf("bsgs(2,5,1000000007) = %lld, check = %lld\n",bsgs(2,5,1000000007LL),qpow(2,bsgs(2,5,1000000007LL),1000000007LL));
    printf("exbsgs(2,3,8) = %lld (expect -1)\n",exbsgs(2,3,8));
    printf("exbsgs(6,3,25) = %lld, check = %lld\n",exbsgs(6,3,25),qpow(6,exbsgs(6,3,25),25));
    printf("exbsgs(2,8,48) = %lld, check = %lld\n",exbsgs(2,8,48),qpow(2,exbsgs(2,8,48),48));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P4195 输入多组 a p b，输出最小 x，无解输出 No Solution
// 样例：P3846 输入 p b n -> 3 5 8 得 3（3^3=27=2+5*5 型离散对数）
// 边界：p=1 返回 0；b%p==0 返回 0；普通 BSGS 要求 gcd(a,p)=1

/*
自测记录：
  1) 三个经典手算例子 + b=0 / b=1 特判；
  2) a,p<=40 的全部组合（b 取遍 0..p-1）与小模数暴力枚举最小解对拍，且结果代回 a^x=b；
  3) 非素数模数（2 的幂、一般合数）下 exbsgs 同样全枚举对拍；
  4) 4 个大素数上用已知指数 x 造 b，验证 bsgs 求出的 x 不超过已知解且代回成立；
  5) 随机大模数（含 2^k）用已知指数造数据验 exbsgs 代回。
*/
