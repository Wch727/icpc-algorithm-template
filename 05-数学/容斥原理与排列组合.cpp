#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD=1000000007;
const int N=100005;
ll fac[N],inv[N];

ll qpow(ll a,ll n) // n>=0，0<=a<MOD
{
    ll r=1;
    while(n)
    {
        if(n&1)r=r*a%MOD;
        a=a*a%MOD,n>>=1;
    }
    return r;
}

// n<MOD，O(n) 预处理，O(1) 组合数
void init(int n)
{
    assert(n>=0&&n<N);
    fac[0]=1;
    for(int i=1;i<=n;i++)fac[i]=fac[i-1]*i%MOD;
    inv[n]=qpow(fac[n],MOD-2);
    for(int i=n;i>=1;i--)inv[i-1]=inv[i]*i%MOD;
}

ll C(int n,int m) // n 在预处理范围内；m 不合法时返回 0
{
    if(m<0||m>n)return 0;
    return fac[n]*inv[m]%MOD*inv[n-m]%MOD;
}

// O(n)，容斥排除固定点；n=0 时空排列有一种
ll derange(int n)
{
    ll ans=0;
    for(int i=0;i<=n;i++)
    {
        ll x=C(n,i)*fac[n-i]%MOD;
        ans=(ans+(i&1?MOD-x:x))%MOD;
    }
    return (ll)ans;
}

ll multiset_count(const vector<int> &cnt) // cnt 非负，总和在预处理范围内
{
    int n=accumulate(cnt.begin(),cnt.end(),0);
    ll ans=fac[n];
    for(int x:cnt)ans=ans*inv[x]%MOD;
    return (ll)ans;
}

// O(m*2^m)，[1,n] 中不被任一 d 整除的数；d 必须为正
ll avoid(ll n,const vector<ll> &d)
{
    int m=d.size();
    assert(n>=0&&m<25);
    __int128 ans=n;
    for(int s=1;s<(1<<m);s++)
    {
        ll l=1;
        bool over=false;
        for(int i=0;i<m;i++)if(s>>i&1)
        {
            assert(d[i]>0);
            ll x=d[i]/gcd(l,d[i]);
            if(l>n/x){over=true;break;}
            l*=x;
        }
        if(over)continue; // 最小公倍数已超过 n，该交集贡献为 0；不构造 n+1
        ll x=n/l;
        if(__builtin_popcount((unsigned)s)&1)ans-=x;
        else ans+=x;
    }
    return (ll)ans;
}

// 范德蒙德：sum(k) C(a,k)C(b,r-k)=C(a+b,r)，越界项为 0。
// 例 sum(k) C(i-1,k)C(n-i,k)=C(n-1,n-i)，常能消去逐中位数枚举。
// 排序后算子集贡献：第 i 个元素作为子集第 j 个数，系数为 C(i-1,j-1)*2^(n-i)。
// 若该位置权为 F[1]=1、F[j]=2^(j-2)(j>=2)，总系数=(3^(i-1)+1)/2*2^(n-i)。
// “只在最短可行链计数”且两个端点都必须移动：k>=2 时 k!-2(k-1)!+(k-2)!；单点另计。
// Hall：二分图左侧全匹配 iff 每个左点子集 S 都满足 |N(S)|>=|S|；小组数可枚举组子集。
// 带需求量的组展开成单位点或用流判定，不能只检查单组邻居够不够。
