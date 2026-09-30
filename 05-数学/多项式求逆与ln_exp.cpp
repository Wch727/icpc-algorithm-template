// 适用：生成函数运算；均在模 P 的形式幂级数中计算。
// 参数：a[i] 为 x^i 的系数，0-indexed；n>=1 为保留项数，结果取模 x^n。
// 前提：输入非空且系数在 [0,P)，求逆 a[0]!=0，ln 要 a[0]=1，exp 要 a[0]=0。
// 关键：牛顿迭代每次把正确项数翻倍；低次项截断不影响后续模 x^n 的结果。
// 易错：积分除以次数，需 n<P；完整卷积长度补成 2 的幂且不超过 2^23。
// 复杂度：求逆/ln/exp 按输入截至 O(n) 时 O(n log n)，额外空间 O(n)；超长输入会增加卷积成本。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int P=998244353;

// O(log b)，计算 a^b mod P；b 是非负指数，P 为固定素数。
int qpow(int a,int b)
{
    int ans=1;
    for(;b;b>>=1,a=(ll)a*a%P)if(b&1)ans=(ll)ans*a%P;
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
    // O(L log L)，原地变换 a[0..L-1]；L 是 2 的幂，inv=0 正变换，非零为逆变换。
    ntt(a,0),ntt(b,0);
    for(int i=0;i<n;i++)a[i]=(ll)a[i]*b[i]%P;
    // O(L log L)，原地变换 a[0..L-1]；L 是 2 的幂，inv=0 正变换，非零为逆变换。
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
