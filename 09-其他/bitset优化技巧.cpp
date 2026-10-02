#include<bits/stdc++.h>
using namespace std;
const int N=513;

// O(n*N/字长)，非负整数子集和，保留 0..N-1；只需 N 大于查询上限。
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

// O(n²*N/字长)，传递闭包；包含长度为 0 的路径，高于 n-1 的输入位应清零。
void closure(vector<bitset<N> > &g)
{
    int n=g.size();
    assert(n<=N);
    for(int i=0;i<n;i++)g[i][i]=1;
    for(int k=0;k<n;k++)
        for(int i=0;i<n;i++)if(g[i][k])g[i]|=g[k];
}

// mask 的子集中同时属于 allow 的个数（含空集）：交集中的每一位自由选，O(1)。
unsigned long long count_subset(unsigned mask,unsigned allow){return 1ULL<<__builtin_popcount(mask&allow);}
