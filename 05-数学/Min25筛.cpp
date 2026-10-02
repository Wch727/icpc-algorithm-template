// Min_25：积性 f，f(1)=1，素数处 f(p)=c0+c1*p+c2*p² mod 1e9+7。
// prime_power(p,e,p^e) 由题目提供，不是随意对普通整数套同一多项式；先筛素数贡献，再枚举最小素因子。
// 常见复杂度 O(n^(3/4)/log n)，空间 O(sqrt n)；n>=0，sqrt(n) 须能分配，建议先估算性能。
// 例 f(p^e)=p^e*(p^e-1)，coeff={0,-1,1}。phi/mu 的单纯前缀仍优先杜教筛。
// 需要精确 pi(floor(n/i)) 时用“整除商上的质数计数.cpp”；本接口返回模意义积性函数和。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
template<class PrimePower>
ll min25(ll n,array<ll,3> coeff,PrimePower prime_power)
{
    const ll mod=1000000007,iv2=500000004,iv6=166666668;if(n<1)return 0;
    ll rt=sqrtl(n);while((rt+1)<=n/(rt+1))rt++;while(rt>n/rt)rt--;
    assert(rt<=INT_MAX-1);int r=rt;vector<int> prime={0};vector<bool> composite(r+1);
    for(int i=2;i<=r;i++){if(!composite[i])prime.push_back(i);for(int j=1;j<(int)prime.size()&&(ll)i*prime[j]<=r;j++){composite[i*prime[j]]=true;if(i%prime[j]==0)break;}}
    vector<array<ll,3>> pref(prime.size());for(int i=1;i<(int)prime.size();i++){ll p=prime[i];pref[i]={ (pref[i-1][0]+1)%mod,(pref[i-1][1]+p)%mod,(pref[i-1][2]+p*p)%mod};}
    vector<ll>w;vector<array<ll,3>>g;vector<int> small(r+1),large(r+1);
    for(ll l=1;l<=n;){ll x=n/l;int id=w.size();w.push_back(x);ll z=x%mod;g.push_back({(z-1+mod)%mod,(z*(z+1)%mod*iv2-1+mod)%mod,(z*(z+1)%mod*(2*z+1)%mod*iv6-1+mod)%mod});if(x<=r)small[x]=id;else large[n/x]=id;l=n/x+1;}
    auto id=[&](ll x){return x<=r?small[x]:large[n/x];};
    for(int i=1;i<(int)prime.size();i++){ll p=prime[i];for(int j=0;j<(int)w.size()&&p<=w[j]/p;j++){int k=id(w[j]/p);ll power=1;for(int d=0;d<3;d++){g[j][d]=(g[j][d]-power*(g[k][d]-pref[i-1][d])%mod+mod)%mod;power=power*p%mod;}}}
    for(ll &c:coeff)c=(c%mod+mod)%mod;
    auto sum=[&](auto&&sum,ll x,int j)->ll
    {
        if(x<=1||(j>0&&prime[j]>=x))return 0;ll ans=0;int k=id(x);
        for(int d=0;d<3;d++)ans=(ans+coeff[d]*(g[k][d]-pref[j][d]+mod))%mod;
        for(int i=j+1;i<(int)prime.size()&&prime[i]<=x/prime[i];i++)
        {ll p=prime[i],pw=p;for(int e=1;;e++){ll f=(prime_power(p,e,pw)%mod+mod)%mod;ans=(ans+f*(sum(sum,x/pw,i)+(e!=1)))%mod;if(pw>x/p)break;pw*=p;}}
        return ans;
    };
    return (sum(sum,n,0)+1)%mod;
}
