// @code common
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
// @code subsets
void subset_sum(vector<ll> &f,bool inverse=false)
{
    int n=f.size();
    assert(n>0&&(n&(n-1))==0);
    for(int b=1;b<n;b*=2)
        for(int s= 0; s < n; s++)
            if(s & b)
            {
                if(inverse)
                    f[s]-= f[s ^ b];
                else
                    f[s]+= f[s ^ b];
            }
}
// @code supersets
void superset_sum(vector<ll> &f,bool inverse=false)
{
    int n=f.size();
    assert(n>0&&(n&(n-1))==0);
    for(int b=1;b<n;b*=2)
        for(int s= 0; s < n; s++)
            if(!(s & b))
            {
                if(inverse)
                    f[s]-= f[s | b];
                else
                    f[s]+= f[s | b];
            }
}
// @code convolution
vector<ll> subset_convolution(const vector<ll> &a,const vector<ll> &b,ll p)
{
    int n=a.size();assert(n>0&&(n&(n-1))==0&&b.size()==a.size()&&p>0);
    int k=__builtin_ctz((unsigned)n);
    vector<vector<ll>> f(k+1,vector<ll>(n)),g=f,h=f;
    for(int s= 0; s < n; s++)
    {
        int c= __builtin_popcount((unsigned)s);
        f[c][s]= (a[s] % p + p) % p;
        g[c][s]= (b[s] % p + p) % p;
    }
    auto transform= [&](vector<vector<ll>> &v, bool inv)
    {
        for(int c= 0; c <= k; c++)
            for(int bit= 1; bit < n; bit*= 2)
                for(int s= 0; s < n; s++)
                    if(s & bit)
                    {
                        __int128 z=
                            (__int128)v[c][s] +
                            (inv ? -(__int128)v[c][s ^ bit] : v[c][s ^ bit]);
                        v[c][s]= (z % p + p) % p;
                    }
    };
    transform(f,false);transform(g,false);
    for(int c= 0; c <= k; c++)
        for(int j= 0; j <= c; j++)
            for(int s= 0; s < n; s++)
                h[c][s]= (h[c][s] + (__int128)f[j][s] * g[c - j][s]) % p;
    transform(h,true);vector<ll> ans(n);for(int s=0;s<n;s++)ans[s]=h[__builtin_popcount((unsigned)s)][s];return ans;
}
