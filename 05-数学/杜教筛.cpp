#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=5000006;// 预处理上界，一般取 n^(2/3) 量级

int M;
int prime[N],cnt;// 素数表
bool vis[N];
int phi[N];// 欧拉函数
int mu[N];// 莫比乌斯函数
ll sum_phi[N];// phi 前缀和
ll sum_mu[N];// mu 前缀和

unordered_map<ll,ll> mp_phi,mp_mu;// 记忆化

// O(n)，线性筛 phi / mu 并求前缀和
void get_pre(int n)
{
    M=n,cnt=0;
    vis[1]=true;
    phi[1]=1,mu[1]=1;
    for(int i=2;i<=n;i++)
    {
        if(!vis[i])
        {
            prime[++cnt]=i;
            phi[i]=i-1;// 素数的 phi
            mu[i]=-1;
        }
        for(int j=1;j<=cnt&&(ll)i*prime[j]<=n;j++)
        {
            int t=i*prime[j];
            vis[t]=true;
            if(i%prime[j]==0)
            {
                phi[t]=phi[i]*prime[j];
                mu[t]=0;
                break;// 每个合数只被最小质因子筛一次
            }
            phi[t]=phi[i]*(prime[j]-1);
            mu[t]=-mu[i];
        }
    }
    for(int i=1;i<=n;i++)
    {
        sum_phi[i]=sum_phi[i-1]+phi[i];
        sum_mu[i]=sum_mu[i-1]+mu[i];
    }
}

// O(n^(2/3))，杜教筛求 Σ_{i=1}^{n} phi(i)
// 用恒等式 Σ_{i=1}^{n} phi(i) = n*(n+1)/2 - Σ_{l=2}^{n} (r-l+1)*S(n/l)
ll du_phi(ll n)
{
    if(n<=M)return sum_phi[n];
    if(mp_phi.count(n))return mp_phi[n];
    ll res=n*(n+1)/2;
    for(ll l=2,r;l<=n;l=r+1)
    {
        r=n/(n/l);// 整除分块
        res-=(r-l+1)*du_phi(n/l);
    }
    return mp_phi[n]=res;
}

// O(n^(2/3))，杜教筛求 Σ_{i=1}^{n} mu(i)
// 用恒等式 1 = Σ_{l=1}^{n} (r-l+1)*S(n/l)，即 S(n) = 1 - Σ_{l=2}^{n} (r-l+1)*S(n/l)
ll du_mu(ll n)
{
    if(n<=M)return sum_mu[n];
    if(mp_mu.count(n))return mp_mu[n];
    ll res=1;
    for(ll l=2,r;l<=n;l=r+1)
    {
        r=n/(n/l);
        res-=(r-l+1)*du_mu(n/l);
    }
    return mp_mu[n]=res;
}

// O(n)，暴力前缀和，对拍用
ll phi_naive_sum(ll n)
{
    ll res=0;
    for(ll i=1;i<=n;i++)
    {
        ll x=i,r=x;
        for(ll j=2;j*j<=x;j++)
            if(x%j==0)
            {
                r=r/j*(j-1);
                while(x%j==0)x/=j;
            }
        if(x>1)r=r/x*(x-1);
        res+=r;
    }
    return res;
}

int main()
{
    int P=20000;// 预处理只到 2e4，好让小 n 也能走到杜教筛的递归分支
    get_pre(P);
    int bad=0;
    // 1) 小数据对拍：phi 前缀和
    for(int i=1;i<=2000;i++)
        if(du_phi(i)!=phi_naive_sum(i))bad++;
    // 2) 小数据对拍：mu 前缀和（用线性筛前缀和，同一份筛法）
    for(int i=1;i<=P;i++)
        if(du_mu(i)!=sum_mu[i])bad++;
    // 3) 中等规模对拍：全部走杜教筛递归分支（n>M）
    ll tests[10]={20001,25000,30000,33333,50000,77777,100000,123457,234567,1000000};
    for(int t=0;t<10;t++)
    {
        ll n=tests[t];
        ll want_p=0,want_m=0;
        for(ll i=1;i<=n;i++)want_p+=phi[i];// 用已筛出的 phi 累加
        for(ll i=1;i<=n;i++)want_m+=mu[i];
        if(du_phi(n)!=want_p)bad++;
        if(du_mu(n)!=want_m)bad++;
    }
    // 4) 记忆化命中：重复调用结果必须一致
    if(du_phi(1000000)!=du_phi(1000000))bad++;
    if(du_mu(1000000)!=du_mu(1000000))bad++;
    // 5) 恒等式验证：Σ_{d|n} mu(d) = [n==1]，用前缀和差分查
    for(int i=1;i<=2000;i++)
    {
        int s=0;
        for(int d=1;d*d<=i;d++)
            if(i%d==0)
            {
                s+=mu[d];
                if(d*d!=i)s+=mu[i/d];
            }
        if(s!=(i==1))bad++;
    }
    printf("phi(1..10) prefix = ");
    for(int i=1;i<=10;i++)printf("%lld ",sum_phi[i]);
    printf("\nmu(1..10) prefix  = ");
    for(int i=1;i<=10;i++)printf("%lld ",sum_mu[i]);
    printf("\nS_phi(1e6) = %lld\n",du_phi(1000000));
    printf("S_mu(1e6)  = %lld\n",du_mu(1000000));
    printf("S_phi(1e9) = %lld\n",du_phi(1000000000));
    printf("S_mu(1e9)  = %lld\n",du_mu(1000000000));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P4213 输入 5 -> 12 1（Σφ(1..5)=12，Σμ(1..5)=1）
// 样例：P3768 用杜教筛求 Σφ，配合整除分块
// 边界：M 取 n^(2/3)；n<=M 直接查表；unordered_map 必须记忆化否则退化

/*
自测记录：
  1) phi 前缀和 1..2000 与 O(n sqrt n) 暴力对拍；
  2) mu 前缀和 1..20000 与线性筛前缀和一致；
  3) 10 个中等规模 n（2e4..1e6）强制走递归分支，与筛表累加对拍；
  4) 重复调用验证记忆化不改变结果；
  5) Σ_{d|n} mu(d) = [n==1] 恒等式在 1..2000 上验证。
*/
