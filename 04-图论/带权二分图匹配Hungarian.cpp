// 最小费用匹配：n 行、m 列，n<=m，所有行都要分配不同列；O(n²m)，0-indexed。
// 完整费用矩阵，负费用允许；缺边设大数并在结果中检查是否选中。最大权取负费用。
// 返回 {费用,每行匹配列}；费用及势能/差值须放入 ll，建议 |cost|*n << 1e18。
#include<bits/stdc++.h>
using namespace std;
using ll=long long;
pair<ll,vector<int>> hungarian(const vector<vector<ll>> &c)
{
    int n=c.size(),m=n?c[0].size():0;assert(n<=m);
    vector<ll> u(n+1),v(m+1);vector<int> p(m+1),way(m+1);
    for(int i=1;i<=n;i++)
    {
        p[0]=i;int j0=0;vector<ll> d(m+1,LLONG_MAX/4);vector<bool> used(m+1);
        do
        {
            used[j0]=true;int i0=p[j0],j1=0;ll delta=LLONG_MAX/4;
            for(int j=1;j<=m;j++)if(!used[j])
            {ll x=c[i0-1][j-1]-u[i0]-v[j];if(x<d[j])d[j]=x,way[j]=j0;if(d[j]<delta)delta=d[j],j1=j;}
            for(int j=0;j<=m;j++)if(used[j])u[p[j]]+=delta,v[j]-=delta;else d[j]-=delta;
            j0=j1;
        }while(p[j0]);
        do{int j1=way[j0];p[j0]=p[j1];j0=j1;}while(j0);
    }
    vector<int> match(n);for(int j=1;j<=m;j++)if(p[j])match[p[j]-1]=j-1;
    return {-v[0],match};
}
