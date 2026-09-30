#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int P=998244353;

int qpow(int a,int b)
{
    int ans=1;
    for(;b;b>>=1,a=(ll)a*a%P)if(b&1)ans=(ll)ans*a%P;
    return ans;
}

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
