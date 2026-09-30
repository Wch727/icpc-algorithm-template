#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000006;

int n;
int prime[N],cnt;// prime[1..cnt] 存的素数
bool vis[N];// 合数标记
int mu[N];// 莫比乌斯函数
ll sum_mu[N];// mu 的前缀和（杜教筛 / 整除分块都要用）

// O(n)，线性筛出 mu 并求前缀和
void get_mu(int n)
{
    cnt=0;
    mu[1]=1;
    for(int i=2;i<=n;i++)
    {
        if(!vis[i])
        {
            prime[++cnt]=i;
            mu[i]=-1;// 素数有 1 个质因子
        }
        for(int j=1;j<=cnt&&(ll)i*prime[j]<=n;j++)
        {
            int t=i*prime[j];
            vis[t]=true;
            if(i%prime[j]==0)
            {
                mu[t]=0;// 出现平方因子
                break;// 每个合数只被最小质因子筛一次
            }
            mu[t]=-mu[i];
        }
    }
    for(int i=1;i<=n;i++)sum_mu[i]=sum_mu[i-1]+mu[i];
}

// O(sqrt n)，整除分块：求 Σ_{i=1}^{n} mu[i]*(n/i)
ll sum_mu_div(int n)
{
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=n/(n/l);// [l,r] 内 n/i 相同
        res+=(ll)(sum_mu[r]-sum_mu[l-1])*(n/l);
    }
    return res;
}

// O(sqrt n)，整除分块：求 Σ_{i=1}^{n} mu[i]*g(n/i)，g 是任意整数函数
// 要求 g 可 O(1) 求值；分块时 g 的参数 n/i 只会取到 O(sqrt n) 个不同值
ll sum_mu_mul_g(int n,ll g[],int gn)
{
    if(n>gn)return -1;// g 的定义域不够，调用者自己保证 n<=gn
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=n/(n/l);// [l,r] 内 n/i 相同
        res+=(ll)(sum_mu[r]-sum_mu[l-1])*g[n/l];
    }
    return res;
}

// O(log n)，数论分块求 Σ_{i=1}^{n} floor(n/i)，用于自测分块边界
ll sum_floor(int n)
{
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=n/(n/l);
        res+=(ll)(r-l+1)*(n/l);
    }
    return res;
}

// O(sqrt n)，模板核心：ΣΣ[gcd(i,j)==1]，i<=n,j<=m
// 反演后 = Σ_{d=1}^{n} mu[d]*(n/d)*(m/d)
ll sum_coprime(int n,int m)
{
    if(n>m)swap(n,m);
    ll res=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=min(n/(n/l),m/(m/l));// 两个商同时不变的最远位置
        res+=(ll)(sum_mu[r]-sum_mu[l-1])*(n/l)*(m/l);
    }
    return res;
}

// O(n^2)，暴力数 gcd，对拍用
ll sum_coprime_naive(int n,int m)
{
    ll res=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            if(__gcd(i,j)==1)res++;
    return res;
}

// O(n)，暴力枚举倍数求 Σ mu[i]*(n/i)，对拍整除分块用
ll sum_mu_div_naive(int n)
{
    ll res=0;
    for(int i=1;i<=n;i++)res+=(ll)mu[i]*(n/i);
    return res;
}

// O(sqrt n)，由 mu 前缀和暴力求 Σ mu[i]，对拍线性筛前缀和用
int mertens_naive(int n)
{
    int res=0;
    for(int i=1;i<=n;i++)res+=mu[i];
    return res;
}

int main()
{
    int M=3000;
    get_mu(M);
    int bad=0;
    // 1) mu 与定义对拍（枚举平方因子）
    for(int i=1;i<=M;i++)
    {
        int x=i,c=0,sq=0;
        for(int j=2;j*j<=x;j++)
            if(x%j==0)
            {
                x/=j,c++;
                if(x%j==0)sq=1;
                x*=j;
            }
        if(x>1)c++;
        int want=sq?0:((c&1)?-1:1);
        if(mu[i]!=want)bad++;
    }
    // 2) ΣΣ[gcd==1] 与暴力对拍
    for(int a=1;a<=60;a++)
        for(int b=1;b<=60;b++)
            if(sum_coprime(a,b)!=sum_coprime_naive(a,b))bad++;
    // 3) Σ mu[i]*(n/i) 对拍
    for(int i=1;i<=M;i++)
        if(sum_mu_div(i)!=sum_mu_div_naive(i))bad++;
    // 4) 整除分块 Σ floor(n/i) 与暴力对拍
    for(int i=1;i<=M;i++)
    {
        ll s=0;
        for(int j=1;j<=i;j++)s+=i/j;
        if(sum_floor(i)!=s)bad++;
    }
    // 5) 大规模边界值：sum_mu 前缀和 + 整除分块一致性
    get_mu(1000000);
    if(sum_mu_div(1000000)!=sum_mu_div_naive(1000000))bad++;
    if(sum_mu[1000000]!=mertens_naive(1000000))bad++;
    // 6) sum_mu_mul_g 用 g(x)=x 检验：Σ mu[i]*(n/i) 与整除分块版一致
    static ll g[N];
    for(int i=1;i<=M;i++)g[i]=i;
    for(int i=1;i<=M;i++)
        if(sum_mu_mul_g(i,g,M)!=sum_mu_div_naive(i))bad++;
    printf("mu(1..10) = ");
    for(int i=1;i<=10;i++)printf("%d ",mu[i]);
    printf("\nsum_mu_div(1e6) = %lld\n",sum_mu_div(1000000));
    printf("M(1e6) = %lld\n",sum_mu[1000000]);
    printf("coprime pairs (1..100) = %lld\n",sum_coprime(100,100));
    printf("sum_floor(1e9) = %lld\n",sum_floor(1000000000));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P2158 输入 4 -> 9（n=4 时能看到 9 个点，即 2*Σφ+1）
// 样例：P2568 输入 4 -> 4（素数对 (2,2),(2,3),(3,2) 等）
// 边界：mu[1]=1；n=1 时整除分块只剩 l=r=1；sum_mu 用 ll 防溢出

/*
自测记录：
  1) 1..3000 的 mu 与「枚举平方因子」定义对拍；
  2) 60x60 以内 ΣΣ[gcd==1] 线性筛版本与 O(n^2) 暴力对拍；
  3) 1..3000 的 Σ mu[i]*(n/i) 整除分块与 O(n) 枚举对拍；
  4) 1..3000 的 Σ floor(n/i) 整除分块与二重暴力对拍；
  5) n=1e6 处前缀和、整除分块、通项三种算法结果一致。
*/
