#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD=998244353;
ll mod_norm(ll x)
{
    x%= MOD;
    return x < 0 ? x + MOD : x;
}
ll qpow(ll a,ll b)
{
    ll r=1;
    for(; b; b>>= 1, a= a * a % MOD)
        if(b & 1)
            r= r * a % MOD;
    return r;
}

// 一、BM：返回 c，使 f[n]=c[0]f[n-1]+...+c[k-1]f[n-k]，O(s*k)，最坏 O(s²)。
// 输入为连续前 s 项，固定素数模数；全零返回空。仅证明有限前缀的递推关系。
// 若无限序列最短阶数<=K，前 2K 项足以恢复；不能凭任意短前缀保证后续正确。
vector<ll> berlekamp_massey(vector<ll> a)
{
    for(ll &x:a)x=mod_norm(x);
    vector<ll> c{1}, b{1};
    int k=0,m=1;
    ll last=1;
    for(int n=0;n<(int)a.size();n++)
    {
        ll d=a[n];
        for(int i=1;i<=k;i++)d=(d+c[i]*a[n-i])%MOD;
        if(!d)
        {
            ++m;
            continue;
        }
        auto old=c;
        ll coef=d*qpow(last,MOD-2)%MOD;
        c.resize(max(c.size(),b.size()+m));
        for(int i=0;i<(int)b.size();i++)c[i+m]=mod_norm(c[i+m]-coef*b[i]%MOD);
        if(2*k<=n)k=n+1-k,b=move(old),last=d,m=1;
        else ++m;
    }
    c.resize(k+1),c.erase(c.begin());
    for(ll &x:c)x=mod_norm(-x);
    return c;
}

// 二、已知递推求第 n 项：x^n mod (x^k-c[0]x^(k-1)-...-c[k-1])。
// O(k² log n)，空间 O(k)；init 至少含 f[0..k-1]，n 从 0 开始。
// c 为空约定为零序列；输入系数/初值允许负数。只需本区与 mod_norm/MOD 即可使用。
ll linear_nth(vector<ll> init,vector<ll> c,unsigned long long n)
{
    int k=c.size();
    assert(init.size()>=c.size());
    if(!k)return 0;
    for(ll &x:init)x=mod_norm(x);
    for(ll &x:c)x=mod_norm(x);
    if(n<(unsigned)k)return init[n];
    auto mul=[&](const vector<ll> &a,const vector<ll> &b)
    {
        vector<ll> t(2*k-1);
        for(int i= 0; i < k; i++)
            for(int j= 0; j < k; j++)
                t[i + j]= (t[i + j] + a[i] * b[j]) % MOD;
        for(int i=2*k-2;i>=k;i--)
            for(int j=1;j<=k;j++)t[i-j]=(t[i-j]+t[i]*c[j-1])%MOD;
        t.resize(k);return t;
    };
    vector<ll> a(k),b(k);
    a[0]=1;
    if(k==1)b[0]=c[0];else b[1]=1;
    for(; n; n>>= 1, b= mul(b, b))
        if(n & 1)
            a= mul(a, b);
    ll ans=0;
    for(int i=0;i<k;i++)ans=(ans+a[i]*init[i])%MOD;
    return ans;
}
