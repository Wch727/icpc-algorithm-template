// 适用：生成函数运算；均在模 P 的形式幂级数中计算。
// 参数：a[i] 为 x^i 的系数，0-indexed；n>=1 为保留项数，结果取模 x^n。
// 前提：输入非空且系数在 [0,P)，求逆 a[0]!=0，ln 要 a[0]=1，exp 要 a[0]=0。
// 关键：牛顿迭代每次把正确项数翻倍；低次项截断不影响后续模 x^n 的结果。
// 易错：积分除以次数，需 n<P；完整卷积长度补成 2 的幂且不超过 2^23。
// 复杂度：求逆/ln/exp 按输入截至 O(n) 时 O(n log n)，额外空间 O(n)；超长输入会增加卷积成本。
// Bell 数 EGF：sum(n>=0) B[n]*x^n/n! = exp(exp(x)-1)。
// 先构造 exp(x)-1 的系数（常数 0，其余 1/n!），求一次多项式 exp，再乘 n! 取 B[n]。
// 需 n<模数、阶乘可逆；普通生成函数的系数不能直接当 B[n]。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int P=998244353;

// O(log b)，计算 a^b mod P；b 是非负指数，P 为固定素数。
int qpow(int a,int b)
{
    int ans=1;
    for(; b; b>>= 1, a= (ll)a * a % P)
        if(b & 1)
            ans= (ll)ans * a % P;
    return ans;
}

// O(L log L)，原地变换 a[0..L-1]；L 是 2 的幂，inv=0 正变换，非零为逆变换。
void ntt(vector<int> &a,int inv)
{
    int n=a.size();
    for(int i=1,j=0;i<n;i++)
    {
        int bit=n>>1;
        for(;j&bit;bit>>=1)j^=bit;
        j^=bit;
        if(i<j)swap(a[i],a[j]);
    }
    for(int len=2;len<=n;len<<=1)
    {
        int wlen=qpow(3,(P-1)/len);
        if(inv)wlen=qpow(wlen,P-2);
        for(int i=0;i<n;i+=len)
        {
            int w=1;
            for(int j=0;j<len/2;j++,w=(ll)w*wlen%P)
            {
                int u=a[i+j],v=(ll)a[i+j+len/2]*w%P;
                a[i+j]=(u+v)%P,a[i+j+len/2]=(u-v+P)%P;
            }
        }
    }
    if(inv)
    {
        int v=qpow(n,P-2);
        for(int &x:a)x=(ll)x*v%P;
    }
}

// NTT 长度不超过 2^23，输入系数在 [0,P)
// O(L log L)，a/b 为系数副本，k>=0 为结果项数；补零后点乘，再截断或补到 k 项。
vector<int> mul(vector<int> a,vector<int> b,int k)
{
    if(a.empty()||b.empty())return vector<int>(k);
    int sz=a.size()+b.size()-1,n=1;
    while(n<sz)n<<=1;
    assert(n<=(1<<23));
    a.resize(n),b.resize(n);
    ntt(a,0),ntt(b,0);
    for(int i=0;i<n;i++)a[i]=(ll)a[i]*b[i]%P;
    ntt(a,1),a.resize(k);
    return a;
}

// 牛顿迭代 O(n log n)，a[0]!=0
// O(n log n)，返回逆级数；b*(2-a*b) 使误差平方，缺失的输入项视为 0。
vector<int> inverse(const vector<int> &a,int n)
{
    assert(!a.empty()&&a[0]);
    vector<int> b(1,qpow(a[0],P-2));
    for(int len=2;(int)b.size()<n;len<<=1)
    {
        int k=min(len,n);
        vector<int> c(a.begin(),a.begin()+min(k,(int)a.size()));
        c=mul(c,b,k);
        for(int &x:c)x=(P-x)%P;
        c[0]=(c[0]+2)%P;
        b=mul(b,c,k);
    }
    return b;
}

// ln(a)=积分(a'/a)，要求 a[0]=1，O(n log n)
// O(n log n)，导数乘逆再积分；常数项固定为 0，iv[i] 是整数 i 的模逆。
vector<int> logarithm(const vector<int> &a,int n)
{
    assert(a[0]==1);
    vector<int> d(max(0,(int)a.size()-1));
    for(int i=1;i<(int)a.size();i++)d[i-1]=(ll)a[i]*i%P;
    vector<int> c=mul(d,inverse(a,n),n-1),ans(n),iv(n,1);
    for(int i=2;i<n;i++)iv[i]=P-(ll)(P/i)*iv[P%i]%P;
    for(int i=1;i<n;i++)ans[i]=(ll)c[i-1]*iv[i]%P;
    return ans;
}

// 牛顿迭代 b*=1+a-ln(b)，要求 a[0]=0，O(n log n)
// O(n log n)，返回 exp(a)；b*(1+a-ln(b)) 修正高次误差，b[0] 始终为 1。
vector<int> exponential(const vector<int> &a,int n)
{
    assert(a[0]==0);
    vector<int> b(1,1);
    for(int len=2;(int)b.size()<n;len<<=1)
    {
        int k=min(len,n);
        vector<int> c=logarithm(b,k);
        for(int i=0;i<k;i++)c[i]=((i<(int)a.size()?a[i]:0)-c[i]+P)%P;
        c[0]=(c[0]+1)%P;
        b=mul(b,c,k);
    }
    return b;
}

// 以下扩展共用本文件 mul/inverse/ln/exp：固定模 998244353，系数已归一化，n<P。
int power_u64(int a, unsigned long long e)
{
    int z= 1;
    for(; e; e>>= 1, a= (ll)a * a % P)
        if(e & 1)
            z= (ll)z * a % P;
    return z;
}
// a^k mod x^n：分离零前缀、首项常数，再 exp(k*ln(a))；k=0 返回 1，O(n log n)。
vector<int> polynomial_power(const vector<int> &a,unsigned long long k,int n)
{
    if(n <= 0)
        return {};
    vector<int> ans(n);
    if(!k)
    {
        ans[0]= 1;
        return ans;
    }
    int lead=0;while(lead<(int)a.size()&&!a[lead])lead++;
    if(lead==(int)a.size()||(lead&&k>=(unsigned long long)(n+lead-1)/lead))return ans;
    int shift=lead*k,len=n-shift;vector<int> b(len);int inv=qpow(a[lead],P-2);
    for(int i=0;i<len&&lead+i<(int)a.size();i++)b[i]=(ll)a[lead+i]*inv%P;
    auto log=logarithm(b,len);for(int &x:log)x=(ll)x*(k%P)%P;auto c=exponential(log,len);int scale=power_u64(a[lead],k);
    for(int i=0;i<len;i++)ans[shift+i]=(ll)c[i]*scale%P;return ans;
}
// Tonelli–Shanks，固定奇素数 P；非剩余返回 -1，选择两个根中较小者。
int scalar_sqrt(int a)
{
    if(!a)
        return 0;
    if(qpow(a, (P - 1) / 2) != 1)
        return -1;
    int q= P - 1, m= 0;
    while(!(q & 1))
        q>>= 1, m++;
    int z= 2;
    while(qpow(z, (P - 1) / 2) == 1)
        z++;
    int c=qpow(z,q),x=qpow(a,(q+1)/2),t=qpow(a,q);
    while(t != 1)
    {
        int i= 0, u= t;
        while(u != 1)
            u= (ll)u * u % P, i++;
        int b= qpow(c, 1 << (m - i - 1));
        x= (ll)x * b % P;
        c= (ll)b * b % P;
        t= (ll)t * c % P;
        m= i;
    }
    return min(x,P-x);
}
// b²=a mod x^n；非零首项次数必须为偶数且系数为二次剩余。false 无解，O(n log n)。
bool polynomial_sqrt(const vector<int> &a,int n,vector<int> &ans)
{
    ans.assign(n, 0);
    if(!n)
        return true;
    int lead= 0;
    while(lead < min(n, (int)a.size()) && !a[lead])
        lead++;
    if(lead == min(n, (int)a.size()))
        return true;
    if(lead & 1)
        return false;
    int x= scalar_sqrt(a[lead]);
    if(x < 0)
        return false;
    int len= n - lead;
    vector<int> f(len);
    for(int i= 0; i < len && lead + i < (int)a.size(); i++)
        f[i]= a[lead + i];
    vector<int> b= {x};
    while((int)b.size() < len)
    {
        int k= min(len, 2 * (int)b.size());
        vector<int> c(f.begin(), f.begin() + k);
        c= mul(c, inverse(b, k), k);
        b.resize(k);
        for(int i= 0; i < k; i++)
            b[i]= (ll)(b[i] + c[i]) * ((P + 1) / 2) % P;
    }
    for(int i=0;i<len;i++)ans[lead/2+i]=b[i];return true;
}
void trim_poly(vector<int> &a)
{
    while(!a.empty() && !a.back())
        a.pop_back();
}
// 商和余数：a=b*q+r，deg r<deg b；零多项式用空 vector，b 不能为零。O(n log n)。
pair<vector<int>,vector<int>> polynomial_divmod(vector<int> a,vector<int> b)
{
    trim_poly(a);
    trim_poly(b);
    assert(!b.empty());
    if(a.size() < b.size())
        return {{}, a};
    int k=a.size()-b.size()+1;vector<int> ra=a,rb=b;reverse(ra.begin(),ra.end());reverse(rb.begin(),rb.end());ra.resize(k);rb.resize(min(k,(int)rb.size()));
    auto q= mul(ra, inverse(rb, k), k);
    reverse(q.begin(), q.end());
    auto c= mul(b, q, a.size());
    for(int i= 0; i < (int)a.size(); i++)
        a[i]= (a[i] - c[i] + P) % P;
    a.resize(b.size()-1);trim_poly(a);trim_poly(q);return {q,a};
}
// 乘积树：多点求值与快速插值 O((n+m)log²(n+m))；点数 m，所有点在 [0,P)。
// 插值要求点两两不同；各叶节点为 (x-x_i)，用导数求分母，并自底向上合并。
struct PolynomialPoints
{
    int n;vector<int> x;vector<vector<int>> tree;
    PolynomialPoints(vector<int> x) : n(x.size()), x(x), tree(max(1, 4 * n))
    {
        if(n)
            build(1, 0, n);
    }
    void build(int p, int l, int r)
    {
        if(r - l == 1)
        {
            tree[p]= {(P - x[l]) % P, 1};
            return;
        }
        int m= (l + r) / 2;
        build(p * 2, l, m);
        build(p * 2 + 1, m, r);
        tree[p]= mul(tree[p * 2], tree[p * 2 + 1], r - l + 1);
    }
    void evaluate(int p,int l,int r,const vector<int> &a,vector<int> &ans)const
    {
        auto rem= polynomial_divmod(a, tree[p]).second;
        if(r - l == 1)
        {
            ans[l]= rem.empty() ? 0 : rem[0];
            return;
        }
        int m= (l + r) / 2;
        evaluate(p * 2, l, m, rem, ans);
        evaluate(p * 2 + 1, m, r, rem, ans);
    }
    vector<int> evaluate(const vector<int> &a) const
    {
        vector<int> ans(n);
        if(n)
            evaluate(1, 0, n, a, ans);
        return ans;
    }
    vector<int> combine(int p,int l,int r,const vector<int>&w)const
    {
        if(r - l == 1)
            return {w[l]};
        int m= (l + r) / 2;
        auto a= mul(combine(p * 2, l, m, w), tree[p * 2 + 1], r - l),
             b= mul(combine(p * 2 + 1, m, r, w), tree[p * 2], r - l);
        for(int i= 0; i < r - l; i++)
            a[i]= (a[i] + b[i]) % P;
        return a;
    }
    vector<int> interpolate(const vector<int> &y)const
    {
        assert(y.size() == x.size());
        if(!n)
            return {};
        vector<int> d(n);
        for(int i= 1; i <= n; i++)
            d[i - 1]= (ll)tree[1][i] * i % P;
        auto w= evaluate(d);
        for(int i= 0; i < n; i++)
        {
            assert(w[i]);
            w[i]= (ll)y[i] * qpow(w[i], P - 2) % P;
        }
        return combine(1, 0, n, w);
    }
};
// a(b(x)) mod x^n，b[0]=0。分块复合（Brent–Kung 的朴素线性组合版）。
// O(n² + sqrt(n)*M(n))，M 为卷积成本；比 Horner 的 n 次 NTT 少，但不是最新准线性算法。
vector<int> polynomial_compose(vector<int> a,vector<int> b,int n)
{
    if(!n)
        return {};
    assert(b.empty() || b[0] == 0);
    a.resize(n);
    b.resize(n);
    int block= max(1, (int)sqrt(n));
    vector<vector<int>> baby(block+1,vector<int>(n));baby[0][0]=1;for(int j=1;j<=block;j++)baby[j]=mul(baby[j-1],b,n);
    vector<int> ans(n);
    for(int l= (n - 1) / block * block; l >= 0; l-= block)
    {
        ans= mul(ans, baby[block], n);
        for(int j= 0; j < block && l + j < n; j++)
            if(a[l + j])
                for(int i= 0; i < n; i++)
                    ans[i]= (ans[i] + (ll)a[l + j] * baby[j][i]) % P;
    }
    return ans;
}
