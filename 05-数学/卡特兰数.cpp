#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000006;

ll fact[N],inv_fact[N];
ll mod=1000000007;

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

// O(N)，阶乘与阶乘逆元，卡特兰数组合公式要用
void C_init(int n,ll p)
{
    mod=p;
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%p;
    inv_fact[n]=qpow(fact[n],p-2,p);
    for(int i=n;i>=1;i--)inv_fact[i-1]=inv_fact[i]*i%p;
}

// O(1)，要求 0<=n<p
ll C(ll n,ll m)
{
    if(m<0||m>n||n<0)return 0;
    return fact[n]*inv_fact[m]%mod*inv_fact[n-m]%mod;
}

// O(n^2)，出栈序列计数：g[i][j] 表示还有 i 个待入栈、栈内 j 个的方案数
// 每步可以入栈（i>0 -> (i-1,j+1)）或出栈（j>0 -> (i,j-1)），最终停在 g[0][0]
// 两个转移的 j 都变小，所以同一行 j 必须从大到小扫
ll cat_dp(int n)
{
    static ll g[1005][1005];
    for(int i=0;i<=n;i++)
        for(int j=0;j<=n;j++)g[i][j]=0;
    g[n][0]=1;
    for(int i=n;i>=0;i--)
        for(int j=n;j>=0;j--)
        {
            if(!g[i][j])continue;
            if(i>0)g[i-1][j+1]=(g[i-1][j+1]+g[i][j])%mod;// 入栈
            if(j>0)g[i][j-1]=(g[i][j-1]+g[i][j])%mod;// 出栈
        }
    return g[0][0];
}

// O(log n)，Cat[n] 的组合公式：C(2n,n)/(n+1)，即 C(2n,n)*inv(n+1)
ll cat_rec(int n)
{
    if(n==0)return 1;
    return C(2*n,n)*qpow(n+1,mod-2,mod)%mod;
}

// O(n^2)，纯递推 Cat[n] = sum Cat[i]*Cat[n-1-i]，对拍基准
ll cat_naive(int n)
{
    ll c[1005]={0};
    c[0]=1;
    for(int i=1;i<=n;i++)
        for(int j=0;j<i;j++)
            c[i]=(c[i]+c[j]*c[i-1-j])%mod;
    return c[n];
}

int main()
{
    int bad=0;
    int M=2000;
    C_init(2*M+5,1000000007);
    // 1) 组合公式 / 出栈 DP / 朴素递推 三者对拍
    for(int n=0;n<=500;n++)
    {
        ll x=cat_rec(n),y=cat_naive(n),z=cat_dp(n);
        if(x!=y||y!=z)
        {
            bad++;
            if(bad<=5)printf("MISMATCH n=%d rec=%lld naive=%lld dp=%lld\n",n,x,y,z);
        }
    }
    // 2) 小 n 与精确值比对（不取模，n<=18 时卡特兰数在 long long 内）
    ll exact[20],c[20];
    c[0]=1;
    for(int i=1;i<=18;i++)
    {
        c[i]=0;
        for(int j=0;j<i;j++)c[i]+=c[j]*c[i-1-j];
    }
    for(int n=0;n<=18;n++)
    {
        exact[n]=c[n];
        if(cat_rec(n)!=(c[n]%mod))bad++;
        if(cat_naive(n)!=(c[n]%mod))bad++;
        if(cat_dp(n)!=(c[n]%mod))bad++;
    }
    printf("Cat(0..10) = ");
    for(int n=0;n<=10;n++)printf("%lld ",cat_rec(n));
    printf("\nexact  Cat(10) = %lld, Cat(18) = %lld\n",exact[10],exact[18]);
    printf("Cat(1000) = %lld\n",cat_rec(1000));
    printf("stack out-sequence n=3 : %lld (expect 5)\n",cat_dp(3));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1044 输入 3 -> 5，输入 18 -> 477638700
// 样例：P1720 斐波那契数列，与卡特兰数无关但同属递推
// 边界：Cat[0]=1；组合公式要求 n+1 < mod 且 mod 为素数

/*
自测记录：
  1) n=0..500：组合公式与朴素 O(n^2) 递推、出栈序列 DP 三种方法对拍；
  2) n=0..18 与不取模的精确卡特兰数比对。
  关键点：出栈 DP 的一行内 j 必须逆序扫，否则同一行会重复转移。
*/
