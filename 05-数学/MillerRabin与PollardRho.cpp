#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// O(log^3 n)，Miller-Rabin 素性判定，确定性基组覆盖 64 位
ll qmul(ll a,ll b,ll mod)
{
    return (ll)((lll)a*b%mod);
}

ll qpow(ll a,ll n,ll mod)
{
    ll res=1%mod;
    a%=mod;
    while(n)
    {
        if(n&1)res=qmul(res,a,mod);
        a=qmul(a,a,mod);
        n>>=1;
    }
    return res;
}

bool miller_rabin(ll n)
{
    if(n<2)return false;
    if(n==2||n==3)return true;
    if(n%2==0)return false;
    // 写成 n-1 = d*2^s
    ll d=n-1;
    int s=0;
    while(!(d&1))d>>=1,s++;
    ll base[7]={2,325,9375,28178,450775,9780504,1795265022};
    for(int i=0;i<7;i++)
    {
        ll a=base[i]%n;
        if(a==0)continue;
        ll x=qpow(a,d,n);
        if(x==1||x==n-1)continue;
        bool ok=false;
        for(int j=1;j<s;j++)
        {
            x=qmul(x,x,n);
            if(x==n-1){ok=true;break;}
        }
        if(!ok)return false;// 一定是合数
    }
    return true;
}

// O(n^(1/4))，Pollard-Rho 找 n 的一个非平凡因子，n 必须是合数
ll pollard_rho(ll n)
{
    if(n%2==0)return 2;
    if(n%3==0)return 3;
    while(true)
    {
        ll c=rand()%(n-1)+1;
        ll x=rand()%(n-1)+1,y=x,d=1;
        // 倍增步长 + gcd 批量化
        ll q=1;
        for(ll len=1;d==1;len<<=1)
        {
            ll tx=x;
            for(ll i=1;i<=len;i++)
            {
                x=(qmul(x,x,n)+c)%n;
                q=qmul(q,abs(x-y),n);
                if(i%127==0)
                {
                    d=__gcd(q,n);
                    if(d>1)break;
                }
            }
            if(d==1)d=__gcd(q,n);
            y=x;
            if(d==n){x=tx;break;}// 失败重来
        }
        if(d>1&&d<n)return d;
    }
}

// O(n^(1/4) log n)，递归分解出全部素因子（不排序、含重数）
void factor(ll n,vector<ll> &v)
{
    if(n==1)return;
    if(miller_rabin(n)){v.push_back(n);return;}
    ll d=pollard_rho(n);
    factor(d,v);
    factor(n/d,v);
}
