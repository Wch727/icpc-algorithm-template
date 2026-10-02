// Min_25 的计数阶段：一次预处理，得到全部 v=floor(n/i) 处的精确 pi(v)，不取模。
// w 降序列出商值，g[j]=pi(w[j])。不能对任意大 x 查询；x<=sqrt(n) 均在表内。
// 常见时间 O(n^(3/4)/log n)，空间 O(sqrt n)；n>=0，先估算筛规模和运行时间。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
struct PrimeCount
{
    ll n,rt;
    vector<ll> w,g;
    vector<int> small,large;
    PrimeCount(ll n):n(n),rt(sqrtl(n))
    {
        assert(n>=0);
        while(rt+1<=n/(rt+1))++rt;
        while(rt&&rt>n/rt)--rt;
        small.resize(rt+1),large.resize(rt+1);
        for(ll l=1;l<=n;)
        {
            ll x=n/l;int j=w.size();w.push_back(x),g.push_back(x-1);
            if(x<=rt)small[x]=j;else large[n/x]=j;
            l=n/x+1;
        }
        for(ll p=2;p<=rt;++p)
        {
            ll before=g[id(p-1)];if(g[id(p)]==before)continue;
            for(int j=0;j<(int)w.size()&&p<=w[j]/p;++j)g[j]-=g[id(w[j]/p)]-before;
        }
    }
    int id(ll x)const
    {
        assert(1<=x&&x<=n);
        int j=x<=rt?small[x]:large[n/x];assert(w[j]==x);return j;
    }
    ll count(ll x)const{return x==0?0:g[id(x)];}
};
// 例：PrimeCount pc(n); pc.count(n)；整除块 [l,r] 内素数个数为 pi(r)-pi(l-1)。
// 本模板的块端点 r=n/(n/l)，故 r 和前一块端点 l-1 都能查询；一般端点不保证在表内。
