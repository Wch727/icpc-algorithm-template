#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;

const ll mod=1000000007;
ll fact[N],inv_fact[N];

// O(log n)，快速幂
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

// O(N)，阶乘与阶乘逆元预处理
// 注意：必须保证 N < p，否则 fact[N] === 0 (mod p) 会让逆元链整体失效
void C_init(int n,ll p)
{
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%p;
    inv_fact[n]=qpow(fact[n],p-2,p);
    for(int i=n;i>=1;i--)inv_fact[i-1]=inv_fact[i]*i%p;
}

// O(1)，C(n,m) mod p，要求 0<=n<p 且 p 为素数
ll C(ll n,ll m,ll p)
{
    if(m<0||m>n||n<0)return 0;
    return fact[n]*inv_fact[m]%p*inv_fact[n-m]%p;
}

const int C_MAX=200000;// 阶乘预处理的数组上限；调用时传的 n 要同时满足 n < p

// O(log_p n)，Lucas 定理：C(n,m) mod p = C(n%p,m%p)*C(n/p,m/p)，p 为素数
// 要求已 C_init(p-1,p)（即预处理到 p-1）；p 很大且 n<p 时直接算 C(n,m)
ll Lucas(ll n,ll m,ll p)
{
    if(m<0||m>n)return 0;
    if(m==0)return 1;
    return C(n%p,m%p,p)*Lucas(n/p,m/p,p)%p;
}

// O(log_p n)，小素数下算 C(n,m) mod p 的对拍基准
// 思路：把 n! 里的 p 因子全部抽走，剩下的部分 mod p 可逆
// k(n) = n/p + k(n/p) 为 v_p(n!)，num/den 为去掉 p 因子后的乘积
ll kfac(ll n,ll p)
{
    ll r=0;
    while(n)n/=p,r+=n;
    return r;
}

ll pf(ll n,ll p)
{
    ll r=1;
    while(n)
    {
        ll t=n/p;
        // t! 的 (n/p) 段的完整阶乘 mod p
        for(ll i=1;i<=n%p;i++)r=r*i%p;
        if(t&1)r=(p-r)%p;// (p-1)! === -1 (mod p)
        n=t;
    }
    return r;
}

ll C_naive(ll n,ll m,ll p)
{
    if(m<0||m>n)return 0;
    ll e=kfac(n,p)-kfac(m,p)-kfac(n-m,p);
    if(e>0)return 0;// p 整除组合数
    ll num=pf(n,p);
    ll den=pf(m,p)*pf(n-m,p)%p;
    return num*qpow(den,p-2,p)%p;
}

int main()
{
    int bad=0;
    const ll P=1000000007;
    C_init(100005,P);
    // 1) C 与杨辉三角对拍
    static ll c[505][505];
    for(int i=0;i<=500;i++)
    {
        c[i][0]=1;
        for(int j=1;j<=i;j++)
            c[i][j]=(c[i-1][j-1]+(j<=i-1?c[i-1][j]:0))%P;
    }
    for(int i=0;i<=500;i++)
        for(int j=0;j<=i;j++)
            if(C(i,j,P)!=c[i][j])bad++;
    // 2) Lucas 小素数下与 C_naive 对拍（n,m 到 2e9）
    ll ps[6]={2,3,5,7,11,13};
    mt19937_64 rnd(20250303);
    for(int t=0;t<6;t++)
    {
        ll p=ps[t];
        C_init(p-1,p);// 每个小素数按其模数重新预处理，边界是 p-1
        for(int i=1;i<=2000;i++)
        {
            ll n=rnd()%2000000000LL,m=rnd()%2000000000LL;
            if(m>n)swap(n,m);
            if(Lucas(n,m,p)!=C_naive(n,m,p))
            {
                bad++;
                if(bad<=5)printf("MISMATCH p=%lld n=%lld m=%lld\n",p,n,m);
            }
        }
        // 小范围再与杨辉三角（mod p）对拍
        static ll s[105][105];
        for(int i=0;i<=100;i++)
        {
            s[i][0]=1;
            for(int j=1;j<=i;j++)s[i][j]=(s[i-1][j-1]+(j<=i-1?s[i-1][j]:0))%p;
        }
        for(int i=0;i<=100;i++)
            for(int j=0;j<=i;j++)
                if(Lucas(i,j,p)!=s[i][j])bad++;
    }
    // 3) 边界
    if(C(5,6,P)!=0||C(5,-1,P)!=0||C(-1,3,P)!=0)bad++;
    if(Lucas(5,6,3)!=0||Lucas(5,-1,3)!=0)bad++;
    C_init(100005,P);
    printf("C(10,3) = %lld (expect 120)\n",C(10,3,P));
    printf("C(100,50) = %lld\n",C(100,50,P));
    printf("C(100000,50000) = %lld\n",C(100000,50000,P));
    C_init(6,7);
    printf("Lucas(10,3,7) = %lld (expect 1)\n",Lucas(10,3,7));
    C_init(2,3);
    printf("Lucas(1e18,1e9,3) = %lld\n",Lucas(1000000000000000000LL,1000000000LL,3));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P3807 输入 (1 2 5) -> 3, (2 1 5) -> 3（n,m 到 1e18，p<=1e5）
// 样例：P2822 求 C(i,j) 是 k 的倍数个数，用杨辉三角离线前缀和
// 边界：m<0 或 m>n 返回 0；Lucas 要求 p 为素数且 p 不大（C 预处理到 p-1）

/*
自测记录：
  1) C(n,m) 与杨辉三角 0..500 全表对拍；
  2) p=2,3,5,7,11,13 时 Lucas 与按 p 因子抽离的暴力 C_naive 各 2000 组对拍（n,m 到 2e9），
     另与杨辉三角 0..100 对拍；
  3) 越界参数返回 0。
*/
