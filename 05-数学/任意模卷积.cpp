// 任意正模 mod<=2^31-1 的整数卷积；负系数先归一化，多模 NTT + CRT 精确重构。
// O(L log L)，L 为补齐的 2 次幂且 <=2^24；三模积超过本范围内整数卷积系数上界。
// 返回 mod 下的系数，mod=1 全为 0，空输入返回空；内存为 O(L)。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
ll pow_mod(ll a,ll b,ll p){ll z=1;for(;b;b>>=1,a=a*a%p)if(b&1)z=z*a%p;return z;}
vector<int> convolution_prime(const vector<ll> &a,const vector<ll> &b,int p,int root,int len)
{
    vector<ll> x(len),y(len);for(int i=0;i<(int)a.size();i++)x[i]=a[i]%p;for(int i=0;i<(int)b.size();i++)y[i]=b[i]%p;
    auto ntt=[&](vector<ll> &f,bool inv)
    {
        for(int i=1,j=0;i<len;i++){int bit=len>>1;for(;j&bit;bit>>=1)j^=bit;j^=bit;if(i<j)swap(f[i],f[j]);}
        for(int l=2;l<=len;l*=2){ll z=pow_mod(root,(p-1)/l,p);if(inv)z=pow_mod(z,p-2,p);for(int i=0;i<len;i+=l){ll w=1;for(int j=0;j<l/2;j++,w=w*z%p){ll u=f[i+j],v=f[i+j+l/2]*w%p;f[i+j]=(u+v)%p;f[i+j+l/2]=(u-v+p)%p;}}}
        if(inv){ll z=pow_mod(len,p-2,p);for(ll &v:f)v=v*z%p;}
    };
    ntt(x,false);ntt(y,false);for(int i=0;i<len;i++)x[i]=x[i]*y[i]%p;ntt(x,true);
    return vector<int>(x.begin(),x.begin()+a.size()+b.size()-1);
}
vector<int> convolution_mod(vector<ll> a,vector<ll> b,int mod)
{
    assert(mod>0);if(a.empty()||b.empty())return {};
    int size=a.size()+b.size()-1,len=1;while(len<size)len*=2;assert(len<=(1<<24));
    for(ll &x:a)x=(x%mod+mod)%mod;for(ll &x:b)x=(x%mod+mod)%mod;
    const ll p=167772161,q=469762049,r=754974721;
    auto x=convolution_prime(a,b,p,3,len),y=convolution_prime(a,b,q,3,len),z=convolution_prime(a,b,r,11,len);
    ll ip=pow_mod(p,q-2,q),ipq=pow_mod(p*q%r,r-2,r);vector<int> ans(size);
    for(int i=0;i<size;i++)
    {
        ll t=(y[i]-x[i]+q)%q*ip%q;ll low=x[i]+p*t;
        ll u=(z[i]-low%r+r)%r*ipq%r;
        ans[i]=(low+(__int128)p*q*u)%mod;
    }
    return ans;
}
