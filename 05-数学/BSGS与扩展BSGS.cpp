// 适用：求最小 x>=0 使 a^x=b (mod p)，无解返回 -1。
// 参数：a,b 是底数与目标余数，p>=1；p=1 返回 0，约定 a^0=1。
// 关键：指数写成 i*m+j，baby 表保留相同余数的最小 j，按 i 递增取最小解。
// 扩展：逐次消去 gcd(a,p)，记录已消去的指数 cnt；b 不能被公因子整除即无解。
// 易错：模乘用 __int128，但 (a%p+p) 等归一化加法仍须不溢出 ll。
// 复杂度：排序版 O(sqrt(p)*log p) 时间、O(sqrt(p)) 空间；大模数内存是瓶颈。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

// O(log n)，模 p 快速幂；n>=0，乘法先扩为 __int128。
ll qpow(ll a,ll n,ll p)
{
    ll ans=1%p;
    for(a%=p;n;n>>=1,a=(lll)a*a%p)if(n&1)ans=(lll)ans*a%p;
    return ans;
}

// O(log min(a,b))，返回 gcd，引用 x/y 输出 ax+by=gcd 的系数。
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b){x=1,y=0;return a;}
    ll u,v,g=exgcd(b,a%b,u,v);
    x=v,y=u-a/b*v;
    return g;
}

// O(log p)，返回 a 的最小非负逆元；调用者须保证 gcd(a,p)=1。
ll inv_mod(ll a,ll p)
{
    ll x,y;
    exgcd(a,p,x,y);
    return (x%p<0?x%p+p:x%p);
}

// O(sqrt(p) log p)，互素离散对数，返回最小非负指数
// O(sqrt(p)*log p)，仅处理底数与模数互素的离散对数；baby 表排序去重保留最小指数。
ll bsgs(ll a,ll b,ll p)
{
    assert(p>=1);
    if(p==1)return 0;
    a=(a%p<0?a%p+p:a%p),b=(b%p<0?b%p+p:b%p);
    if(b==1)return 0;
    if(gcd(a,p)!=1)return -1;// 非互素请用扩展版本
    ll m=sqrtl(p)+1,cur=1;
    vector<pair<ll,ll> > v;
    for(ll j=0;j<m;j++)v.push_back({cur,j}),cur=(lll)cur*a%p;
    sort(v.begin(),v.end());
    v.erase(unique(v.begin(),v.end(),[](pair<ll,ll> x,pair<ll,ll> y){return x.first==y.first;}),v.end());
    ll step=inv_mod(qpow(a,m,p),p);
    cur=b;
    for(ll i=0;i<=m;i++,cur=(lll)cur*step%p)
    {
        int j=lower_bound(v.begin(),v.end(),make_pair(cur,-1LL))-v.begin();
        if(j<(int)v.size()&&v[j].first==cur)return i*m+v[j].second;
    }
    return -1;
}

// 消去公因子后做 BSGS，p>=1，约定 a^0=1
// 消公因子 O(log p) 次后做 BSGS；mul 保存被消部分，最终指数加 cnt。
ll exbsgs(ll a,ll b,ll p)
{
    assert(p>=1);
    if(p==1)return 0;
    a=(a%p<0?a%p+p:a%p),b=(b%p<0?b%p+p:b%p);
    if(b==1)return 0;
    ll cnt=0,mul=1,g;
    while((g=gcd(a,p))>1)
    {
        if(b%g)return -1;
        b/=g,p/=g,cnt++;
        mul=(lll)mul*(a/g)%p;
        if(mul==b)return cnt;
    }
    ll ans=bsgs(a,(lll)b*inv_mod(mul,p)%p,p);
    return ans<0?-1:ans+cnt;
}
