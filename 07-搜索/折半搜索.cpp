#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
vector<ll> a,sums[2];

// O(2^(n/2) n)，含负数；统计和等于目标的子集个数，n<=40
void enum_sum(int l,int r,int id)
{
    auto &s=sums[id];s.assign(1,0);
    for(int i=l;i<r;i++)
    {
        int m=s.size();
        for(int j=0;j<m;j++)s.push_back(s[j]+a[i]);
    }
}

ll solve(ll target)
{
    int n=a.size();
    enum_sum(0,n/2,0);enum_sum(n/2,n,1);
    auto &l=sums[0],&r=sums[1];
    sort(r.begin(),r.end());
    ll ans=0;
    for(ll x:l)
    {
        __int128 want=(__int128)target-x;
        if(want<LLONG_MIN||want>LLONG_MAX)continue;
        auto p=equal_range(r.begin(),r.end(),(ll)want);
        ans+=p.second-p.first;
    }
    return ans;
}
