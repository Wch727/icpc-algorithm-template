#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// O(2^(n/2) n)，含负数；统计和等于目标的子集个数，n<=40
vector<ll> enum_sum(const vector<ll> &a,int l,int r)
{
    vector<ll> s(1,0);
    for(int i=l;i<r;i++)
    {
        int m=s.size();
        for(int j=0;j<m;j++)s.push_back(s[j]+a[i]);
    }
    return s;
}

ll solve(const vector<ll> &a,ll target)
{
    int n=a.size();
    vector<ll> l=enum_sum(a,0,n/2),r=enum_sum(a,n/2,n);
    sort(r.begin(),r.end());
    ll ans=0;
    for(ll x:l)
    {
        pair<vector<ll>::iterator,vector<ll>::iterator> p=equal_range(r.begin(),r.end(),target-x);
        ans+=p.second-p.first;
    }
    return ans;
}
