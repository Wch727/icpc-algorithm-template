// @code common
#include<bits/stdc++.h>
using namespace std;
const int N=513;

// @code knapsack
bitset<N> subset_sum(const vector<int> &a)
{
    bitset<N> f;
    f[0]=1;
    for(int x:a)
    {
        assert(x>=0);
        f|=f<<x;
    }
    return f;
}

// @code closure
void closure(vector<bitset<N> > &g)
{
    int n=g.size();
    assert(n<=N);
    for(int i=0;i<n;i++)g[i][i]=1;
    for(int k=0;k<n;k++)
        for(int i=0;i<n;i++)if(g[i][k])g[i]|=g[k];
}

