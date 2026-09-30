// 适用：gcd 计数、互素点对、按约数求和；mu 把 gcd=1 条件转为倍数计数。
// 参数：筛界 1<=n<N；查询须已筛到所用的 mu 前缀上界。
// 下标：mu/sum_mu 从 1 开始，sum_mu[0]=0；g 的有效定义域是 1..gn。
// 关键：单商块末端 n/(n/l)，双商取两个末端的较小值，区间均为闭区间。
// 结论：sum_coprime 数 1<=i<=n、1<=j<=m 的有序点对，复杂度 O(sqrt(n)+sqrt(m)) 上界。
// 易错：多次筛前清 vis；前缀差先转 ll 再乘商，答案及中间乘积仍需满足 ll 范围。
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
// O(n)，线性筛并求 1..n 前缀；含平方因子的 mu 为 0，其余按素因子个数变号。
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
// O(sqrt n)，求 mu[i]*floor(n/i) 的和；n=0 时空和为 0。
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
// O(sqrt n)，g[t] 是商 t 的函数值，gn 是最大合法下标；n>gn 返回 -1。
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
// O(sqrt n)，合并等商区间求 floor(n/i) 总和；原 O(log n) 注释不适用于此循环。
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
// O(sqrt(n)+sqrt(m)) 上界，利用 mu 前缀计互素有序对；筛界至少 min(n,m)。
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
// O(n*m*log(max(n,m))) 上界，逐对 gcd 核对反演公式。
ll sum_coprime_naive(int n,int m)
{
    ll res=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            if(__gcd(i,j)==1)res++;
    return res;
}

// O(n)，暴力枚举倍数求 Σ mu[i]*(n/i)，对拍整除分块用
// O(n)，逐项求和；须预先有 mu[1..n]。
ll sum_mu_div_naive(int n)
{
    ll res=0;
    for(int i=1;i<=n;i++)res+=(ll)mu[i]*(n/i);
    return res;
}

// O(sqrt n)，由 mu 前缀和暴力求 Σ mu[i]，对拍线性筛前缀和用
// O(n)，逐项累加 mu[1..n]；原 O(sqrt n) 注释与实际循环不符。
int mertens_naive(int n)
{
    int res=0;
    for(int i=1;i<=n;i++)res+=mu[i];
    return res;
}
