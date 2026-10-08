// 适用：已知连续整数点值，求低次数多项式在新点的值，如幂和求和。
// 下标：y[0..n]=f(0..n)，非空，次数<=n<P；eval 的 a[i] 是 x^i 系数。
// 参数：x 是求值点，入口按模 P 归一，y 和 a 建议预先归一到 [0,P)。
// 关键：前后缀积算去掉 i 后的所有 (x-j)，分母为 i!*(n-i)!*(-1)^(n-i)。
// 易错：x 对应已知节点时直接返回 y[x]，不做除以零；节点必须不同模 P。
// 复杂度：插值 O(n+log P)，空间 O(n)，Horner 系数求值 O(次数)。
// 参数计数先证明次数：DAG 上每条路径至多 n-1 条边，每边至多乘一次参数，则次数<=n-1。
// 取 n 个不同节点值即可；含参数的分母、任意函数或取模分支不能直接宣称低次多项式。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int P=998244353;

// O(log b)，固定模 P 快速幂；a 为归一底数，b>=0。
ll qpow(ll a,ll b)
{
    ll ans=1;
    for(; b; b>>= 1, a= a * a % P)
        if(b & 1)
            ans= ans * a % P;
    return ans;
}

// 已知 y[i]=f(i)，次数不超过 n；n<P，O(n) 求 f(x)
// O(n+log P)，y 是连续节点值的副本、x 是查询点；预处理逆阶乘只求一次幂。
int interpolate(vector<int> y,ll x)
{
    int n=(int)y.size()-1;
    x=(x%P+P)%P;
    if(x<=n)return y[x];
    vector<ll> pre(n+2,1),suf(n+2,1),fac(n+1,1),inv(n+1);
    for(int i=0;i<=n;i++)pre[i+1]=pre[i]*(x-i+P)%P;
    for(int i=n;i>=0;i--)suf[i]=suf[i+1]*(x-i+P)%P;
    for(int i=1;i<=n;i++)fac[i]=fac[i-1]*i%P;
    inv[n]=qpow(fac[n],P-2);
    for(int i=n;i>=1;i--)inv[i-1]=inv[i]*i%P;
    ll ans=0;
    for(int i=0;i<=n;i++)
    {
        ll v=pre[i]*suf[i+1]%P*inv[i]%P*inv[n-i]%P*y[i]%P;
        if((n-i)&1)ans=(ans-v+P)%P;
        else ans=(ans+v)%P;
    }
    return ans;
}

// O(a.size())，按系数向量求 f(x)，倒序 Horner 避免逐项求幂；空向量返回 0。
int eval(vector<int> a,ll x)
{
    x=(x%P+P)%P;
    ll ans=0;
    for(int i=(int)a.size()-1;i>=0;i--)ans=(ans*x+a[i])%P;
    return ans;
}
