#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// f 长度必须为 2^k；O(k*2^k)，原地变换，所有中间和须能放入 ll。
// 子集和：变换后 f[s]=Σ原 f[t]，t⊆s；inverse=true 为 Möbius 逆变换。
// 每一位从不含该位的状态转移；子集求和与子集卷积是不同运算。
void subset_sum(vector<ll> &f,bool inverse=false)
{
    int n=f.size();
    assert(n>0&&(n&(n-1))==0);
    for(int b=1;b<n;b*=2)
        for(int s=0;s<n;s++)if(s&b)
            {if(inverse)f[s]-=f[s^b];else f[s]+=f[s^b];}
}
// 超集和：变换后 f[s]=Σ原 f[t]，s⊆t；另一方向只改变掩码条件。
void superset_sum(vector<ll> &f,bool inverse=false)
{
    int n=f.size();
    assert(n>0&&(n&(n-1))==0);
    for(int b=1;b<n;b*=2)
        for(int s=0;s<n;s++)if(!(s&b))
            {if(inverse)f[s]-=f[s|b];else f[s]+=f[s|b];}
}

// 不相交子集卷积 h[S]=Σ(T⊆S) f[T]*g[S\T] mod p，p>0 不要求素数。
// 输入等长 2^k，负值允许；O(k²2^k) 时间、O(k2^k) 空间，先按 popcount 分层。
vector<ll> subset_convolution(const vector<ll> &a,const vector<ll> &b,ll p)
{
    int n=a.size();assert(n>0&&(n&(n-1))==0&&b.size()==a.size()&&p>0);
    int k=__builtin_ctz((unsigned)n);
    vector<vector<ll>> f(k+1,vector<ll>(n)),g=f,h=f;
    for(int s=0;s<n;s++){int c=__builtin_popcount((unsigned)s);f[c][s]=(a[s]%p+p)%p;g[c][s]=(b[s]%p+p)%p;}
    auto transform=[&](vector<vector<ll>> &v,bool inv)
    {
        for(int c=0;c<=k;c++)for(int bit=1;bit<n;bit*=2)for(int s=0;s<n;s++)if(s&bit)
        {__int128 z=(__int128)v[c][s]+(inv?-(__int128)v[c][s^bit]:v[c][s^bit]);v[c][s]=(z%p+p)%p;}
    };
    transform(f,false);transform(g,false);
    for(int c=0;c<=k;c++)for(int j=0;j<=c;j++)for(int s=0;s<n;s++)h[c][s]=(h[c][s]+(__int128)f[j][s]*g[c-j][s])%p;
    transform(h,true);vector<ll> ans(n);for(int s=0;s<n;s++)ans[s]=h[__builtin_popcount((unsigned)s)][s];return ans;
}
