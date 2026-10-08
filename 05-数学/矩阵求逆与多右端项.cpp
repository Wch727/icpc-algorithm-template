// 一次 Gauss–Jordan 解 AX=B（A 为 n*n，B 为 n*r），O(n³+n²r)，原地把 B 变成 X。
// 仅处理 A 可逆的情况；false 表示奇异/接近奇异。求逆令 B=I 即可，不重跑 n 次消元。
// 实数版本 eps 按尺度调整；模版本 p 必须素数，p>=2，允许负输入。编号从 0 开始。
#include<bits/stdc++.h>
using namespace std;
bool solve_matrix(vector<vector<long double>> a,vector<vector<long double>> &b,long double eps=1e-12L)
{
    int n=a.size(),r=n?b[0].size():0;assert((int)b.size()==n);
    for(int c=0;c<n;c++)
    {
        int k= c;
        for(int i= c + 1; i < n; i++)
            if(fabsl(a[i][c]) > fabsl(a[k][c]))
                k= i;
        if(fabsl(a[k][c])<=eps)return false;swap(a[k],a[c]);swap(b[k],b[c]);
        long double z= a[c][c];
        for(auto &x : a[c])
            x/= z;
        for(auto &x : b[c])
            x/= z;
        for(int i= 0; i < n; i++)
            if(i != c)
            {
                z= a[i][c];
                for(int j= c; j < n; j++)
                    a[i][j]-= z * a[c][j];
                for(int j= 0; j < r; j++)
                    b[i][j]-= z * b[c][j];
            }
    }
    return true;
}
bool solve_matrix_mod(vector<vector<long long>> a,vector<vector<long long>> &b,long long p)
{
    using ll=long long;int n=a.size(),r=n?b[0].size():0;assert((int)b.size()==n&&p>=2);
    for(auto &v : a)
        for(ll &x : v)
            x= (x % p + p) % p;
    for(auto &v : b)
        for(ll &x : v)
            x= (x % p + p) % p;
    auto power= [&](ll x, ll e)
    {
        ll z= 1;
        for(; e; e>>= 1, x= (__int128)x * x % p)
            if(e & 1)
                z= (__int128)z * x % p;
        return z;
    };
    for(int c=0;c<n;c++)
    {
        int k= c;
        while(k < n && !a[k][c])
            k++;
        if(k == n)
            return false;
        swap(a[k], a[c]);
        swap(b[k], b[c]);
        ll z= power(a[c][c], p - 2);
        for(ll &x : a[c])
            x= (__int128)x * z % p;
        for(ll &x : b[c])
            x= (__int128)x * z % p;
        for(int i= 0; i < n; i++)
            if(i != c)
            {
                z= a[i][c];
                for(int j= c; j < n; j++)
                    a[i][j]= ((a[i][j] - (__int128)z * a[c][j]) % p + p) % p;
                for(int j= 0; j < r; j++)
                    b[i][j]= ((b[i][j] - (__int128)z * b[c][j]) % p + p) % p;
            }
    }
    return true;
}
