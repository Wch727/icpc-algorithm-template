#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// O(log^3 n)，Miller-Rabin 素性判定，确定性基组覆盖 64 位
ll qmul(ll a,ll b,ll mod)
{
    return (ll)((lll)a*b%mod);
}

ll qpow(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    while(n)
    {
        if(n&1)res=qmul(res,a,mod);
        a=qmul(a,a,mod);
        n>>=1;
    }
    return res;
}

bool miller_rabin(ll n)
{
    if(n<2)return false;
    if(n==2||n==3)return true;
    if(n%2==0)return false;
    // 写成 n-1 = d*2^s
    ll d=n-1;
    int s=0;
    while(!(d&1))d>>=1,s++;
    ll base[7]={2,325,9375,28178,450775,9780504,1795265022};
    for(int i=0;i<7;i++)
    {
        ll a=base[i]%n;
        if(a==0)continue;
        ll x=qpow(a,d,n);
        if(x==1||x==n-1)continue;
        bool ok=false;
        for(int j=1;j<s;j++)
        {
            x=qmul(x,x,n);
            if(x==n-1){ok=true;break;}
        }
        if(!ok)return false;// 一定是合数
    }
    return true;
}

// O(n^(1/4))，Pollard-Rho 找 n 的一个非平凡因子，n 必须是合数
ll pollard_rho(ll n)
{
    if(n%2==0)return 2;
    if(n%3==0)return 3;
    while(true)
    {
        ll c=rand()%(n-1)+1;
        ll x=rand()%(n-1)+1,y=x,d=1;
        // 倍增步长 + gcd 批量化
        ll q=1;
        for(ll len=1;d==1;len<<=1)
        {
            ll tx=x;
            for(ll i=1;i<=len;i++)
            {
                x=(qmul(x,x,n)+c)%n;
                q=qmul(q,abs(x-y),n);
                if(i%127==0)
                {
                    d=__gcd(q,n);
                    if(d>1)break;
                }
            }
            if(d==1)d=__gcd(q,n);
            y=x;
            if(d==n){x=tx;break;}// 失败重来
        }
        if(d>1&&d<n)return d;
    }
}

// O(n^(1/4) log n)，递归分解出全部素因子（不排序、含重数）
void factor(ll n,vector<ll> &v)
{
    if(n==1)return;
    if(miller_rabin(n)){v.push_back(n);return;}
    ll d=pollard_rho(n);
    factor(d,v);
    factor(n/d,v);
}

int main()
{
    srand(20050101);
    int bad=0;
    // 1) Miller-Rabin 与试除对拍（1..200000）
    for(ll n=1;n<=200000;n++)
    {
        bool want=true;
        for(ll i=2;i*i<=n;i++)
            if(n%i==0){want=false;break;}
        if(n<2)want=false;
        if(miller_rabin(n)!=want){bad++;break;}
    }
    // 2) 大素数
    ll primes[6]={1000000007LL,1000000009LL,998244353LL,
                  2305843009213693951LL,4611686018427387847LL,1000000000000000003LL};
    for(int i=0;i<6;i++)
        if(!miller_rabin(primes[i]))bad++;
    // 3) 大合数（注意 2^61-1 是素数，不要放进来）
    ll comps[5]={1000000007LL*1000000009LL,999999999999999989LL*3,
                 4503599627370401LL,1LL<<61,123456789012345679LL};
    for(int i=0;i<5;i++)
        if(miller_rabin(comps[i]))bad++;
    // 4) Pollard-Rho：分解结果各项相乘 == 原数，且每项都是素数
    ll tests[8]={1000000007LL*1000000009LL,999999999999999989LL,
                 (1LL<<61)-1,1000000007LL,123456789012345679LL,
                 9007199254740881LL,6364136223846793005ULL>>1,1000003LL*1000033LL};
    for(int t=0;t<8;t++)
    {
        ll n=tests[t];
        vector<ll> v;
        factor(n,v);
        lll prod=1;
        for(int i=0;i<(int)v.size();i++)
        {
            prod*=v[i];
            if(!miller_rabin(v[i]))bad++;// 因子必须是素数
        }
        if(prod!=(lll)n)bad++;
    }
    // 5) 随机合数交叉验证（乘积回代 + 素性）
    mt19937_64 rnd(20250808);
    for(int t=1;t<=200;t++)
    {
        ll a=rnd()%1000000000+2,b=rnd()%1000000000+2;
        ll n=a*b;
        if(n<=1)continue;
        vector<ll> v;
        factor(n,v);
        lll prod=1;
        sort(v.begin(),v.end());
        for(int i=0;i<(int)v.size();i++)prod*=v[i];
        if(prod!=(lll)n)bad++;
        for(int i=0;i<(int)v.size();i++)
            if(!miller_rabin(v[i]))bad++;
    }
    vector<ll> f;
    ll big=1000000007LL*1000000009LL;
    factor(big,f);
    sort(f.begin(),f.end());
    printf("factor(%lld) =",big);
    for(int i=0;i<(int)f.size();i++)printf(" %lld",f[i]);
    printf("\nmiller_rabin(1e18+3) = %d\n",(int)miller_rabin(1000000000000000003LL));
    printf("miller_rabin(2^61-1) = %d (expect 1, 它是梅森素数)\n",(int)miller_rabin((1LL<<61)-1));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1075 输入 21 -> 7（最大质因子）；大数版本须用本模板
// 边界：n<2 不是素数；n=2,3 特判；Pollard-Rho 要求 n 为合数且 n>4

/*
自测记录：
  1) 1..200000 与 O(sqrt n) 试除逐个比对素性；
  2) 6 个大素数 + 5 个大合数判定；
  3) 8 个 1e18 量级数分解，验证乘积等于原数且因子均为素数；
  4) 200 组随机 a*b 合数分解交叉验证。
*/
