#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD=1000000007;

struct Matrix
{
    int n;
    vector<vector<ll> > a;
    Matrix(int n_,bool unit=false):n(n_),a(n_,vector<ll>(n_,0))
    {
        if(unit)
            for(int i= 0; i < n; i++)
                a[i][i]= 1;
    }
};

Matrix mul(const Matrix &a,const Matrix &b)
{
    Matrix c(a.n);
    for(int i=0;i<a.n;i++)
        for(int k=0;k<a.n;k++)
            for(int j=0;j<a.n;j++)c.a[i][j]=(c.a[i][j]+a.a[i][k]*b.a[k][j])%MOD;
    return c;
}

Matrix qpow(Matrix a,ll n)
{
    Matrix r(a.n,true);
    while(n)
    {
        if(n&1)r=mul(r,a);
        a=mul(a,a),n>>=1;
    }
    return r;
}

// O(k^3 log n)，f(n)=c[0]f(n-1)+...+c[k-1]f(n-k)，初值 f(0..k-1)
vector<ll> c,f;
ll solve(ll n)
{
    int k=c.size();
    assert(k>0&&f.size()==c.size()&&n>=0);
    if(n<k)return (f[n]%MOD+MOD)%MOD;
    Matrix a(k);
    for(int j=0;j<k;j++)a.a[0][j]=(c[j]%MOD+MOD)%MOD;
    for(int i=1;i<k;i++)a.a[i][i-1]=1;
    a=qpow(a,n-k+1);
    ll ans=0;
    for(int i=0;i<k;i++)ans=(ans+a.a[0][i]*((f[k-1-i]%MOD+MOD)%MOD))%MOD;
    return ans;
}
