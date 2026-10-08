#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 增广矩阵 a：n 行、m+1 列，最后一列常数；p 为素数，允许负系数。
// 返回 -1 无解、0 唯一解、1 多解；x 返回一组解（自由变量取 0），原 a 不变。
// 不能套到合数模数！消元 O(n*m*min(n,m))，每个主元另需 O(log p) 求逆。
int gauss_mod(vector<vector<ll>> a,int m,ll p,vector<ll> &x)
{
    assert(m>=0&&p>=2);
    int n=a.size(),r=0;
    vector<int> where(m,-1);
    for(auto &row:a)
    {
        assert((int)row.size()==m+1);
        for(ll &v : row)
        {
            v%= p;
            if(v < 0)
                v+= p;
        }
    }
    auto power= [&](ll v, ll e)
    {
        ll z= 1;
        for(; e; e>>= 1, v= (__int128)v * v % p)
            if(e & 1)
                z= (__int128)z * v % p;
        return z;
    };
    for(int c=0;c<m&&r<n;c++)
    {
        int k=r;
        while(k<n&&!a[k][c])++k;
        if(k==n)continue;
        swap(a[k],a[r]),where[c]=r;
        ll inv=power(a[r][c],p-2);
        for(int j=c;j<=m;j++)a[r][j]=(__int128)a[r][j]*inv%p;
        for(int i= 0; i < n; i++)
            if(i != r && a[i][c])
            {
                ll t= a[i][c];
                for(int j= c; j <= m; j++)
                {
                    ll v= (a[i][j] - (__int128)t * a[r][j]) % p;
                    a[i][j]= v < 0 ? v + p : v;
                }
            }
        ++r;
    }
    x.assign(m,0);
    for(int i= r; i < n; i++)
        if(a[i][m])
            return -1;
    for(int c= 0; c < m; c++)
        if(where[c] >= 0)
            x[c]= a[where[c]][m];
    return r==m?0:1;
}

// GF(2)：第 0..m-1 位是系数，第 m 位是常数；m<B。
// 返回约定同上；原矩阵会被修改，rank 给出秩，多解时共有 2^(m-rank) 组。
// bitset 行消元，O(n*m*ceil(B/机器字长))；x 的自由变量取 0。
template<size_t B>
int gauss_xor(vector<bitset<B>> &a,int m,bitset<B> &x,int &rank)
{
    assert(m>=0&&(size_t)m<B);
    int n=a.size();rank=0;x.reset();
    vector<int> where(m,-1);
    for(int c=0;c<m&&rank<n;c++)
    {
        int k=rank;
        while(k<n&&!a[k][c])++k;
        if(k==n)continue;
        swap(a[k],a[rank]),where[c]=rank;
        for(int i= 0; i < n; i++)
            if(i != rank && a[i][c])
                a[i]^= a[rank];
        ++rank;
    }
    for(int i= rank; i < n; i++)
        if(a[i][m])
            return -1;
    for(int c= 0; c < m; c++)
        if(where[c] >= 0)
            x[c]= a[where[c]][m];
    return rank==m?0:1;
}
