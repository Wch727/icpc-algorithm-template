#include<bits/stdc++.h>
using namespace std;
const int N=513;

// O(n*S/字长)，非负整数子集和，所有可表示的和必须小于 N
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

// O(n^3/字长)，传递闭包；包含长度为 0 的路径
void closure(vector<bitset<N> > &g)
{
    int n=g.size();
    assert(n<=N);
    for(int i=0;i<n;i++)g[i][i]=1;
    for(int k=0;k<n;k++)
        for(int i=0;i<n;i++)if(g[i][k])g[i]|=g[k];
}

// 小全集子集枚举：一次位运算判断子集是否落在允许集合
int count_subset(unsigned mask,unsigned allow)
{
    int ans=0;
    unsigned s=mask;
    while(true)
    {
        if((s&allow)==s)ans++;
        if(!s)break;
        s=(s-1)&mask;
    }
    return ans;
}
