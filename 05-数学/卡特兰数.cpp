// 适用：合法括号序列、出栈顺序、不同二叉树形态、不交叉匹配。
// 参数：n 是成对元素数，Cat[0]=1；阶乘需先预处理到 2*n，且 2*n<mod、<N。
// 关键：Cat[n]=C(2n,n)/(n+1)；首个匹配分割左右子结构得到递推。
// 下标：cat_dp 的 g[i][j] 是剩余未入栈数与栈内数；两者相加不超过初始 n。
// 易错：mod 必须为素数以求逆；DP 与朴素数组要求 n<1005，乘积应在 ll 范围。
// 复杂度：公式查询 O(log mod)，双状态 DP 和卷积递推 O(n^2)，空间分别 O(n^2)/O(n)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000006;

ll fact[N],inv_fact[N];
ll mod=1000000007;

// O(log n)，快速幂
// O(log n)，计算 a 的非负 n 次幂；mod 为正模数。
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
// O(n+log p)，设置全局 mod=p，预处理阶乘 0..n；p 为素数且 n<p。
void C_init(int n,ll p)
{
    mod=p;
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%p;
    inv_fact[n]=qpow(fact[n],p-2,p);
    for(int i=n;i>=1;i--)inv_fact[i-1]=inv_fact[i]*i%p;
}

// O(1)，要求 0<=n<p
// O(1)，求模当前 mod 的 C(n,m)，n 是阶乘下标而非卡特兰参数。
ll C(ll n,ll m)
{
    if(m<0||m>n||n<0)return 0;
    return fact[n]*inv_fact[m]%mod*inv_fact[n-m]%mod;
}

// O(n^2)，出栈序列计数：g[i][j] 表示还有 i 个待入栈、栈内 j 个的方案数
// 每步可以入栈（i>0 -> (i-1,j+1)）或出栈（j>0 -> (i,j-1)），最终停在 g[0][0]
// 两个转移的 j 都变小，所以同一行 j 必须从大到小扫
// O(n^2)，g[n][0] 起步；i 递减接收入栈转移，固定行 j 递减接收出栈转移。
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
// O(log mod)，返回 Cat[n]；n+1 必须可逆且 C 表已覆盖 2*n。
ll cat_rec(int n)
{
    if(n==0)return 1;
    return C(2*n,n)*qpow(n+1,mod-2,mod)%mod;
}

// O(n^2)，纯递推 Cat[n] = sum Cat[i]*Cat[n-1-i]，对拍基准
// O(n^2)，按首对匹配拆分子结构，返回模 mod 的第 n 项。
ll cat_naive(int n)
{
    ll c[1005]={0};
    c[0]=1;
    for(int i=1;i<=n;i++)
        for(int j=0;j<i;j++)
            c[i]=(c[i]+c[j]*c[i-1-j])%mod;
    return c[n];
}
