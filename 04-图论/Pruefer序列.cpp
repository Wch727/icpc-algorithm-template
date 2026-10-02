// 标号树与 Prüfer 序列的双射：n>=2，编号 0..n-1，序列长度 n-2，O(n log n)。
// 每次删编号最小叶子、输出其邻居；点出现次数=度数-1。Cayley：n^(n-2) 棵标号树。
// 给定度数 d_i>=1 且 Σd_i=2n-2，树数量为 (n-2)!/Π(d_i-1)!。
#include<bits/stdc++.h>
using namespace std;
vector<int> prufer_encode(const vector<vector<int>> &g)
{
    int n=g.size();assert(n>=2);vector<int>d(n),code;priority_queue<int,vector<int>,greater<int>> q;
    for(int i=0;i<n;i++){d[i]=g[i].size();if(d[i]==1)q.push(i);}
    for(int k=0;k<n-2;k++){int u=q.top();q.pop();d[u]=0;int v=-1;for(int x:g[u])if(d[x]){v=x;break;}assert(v>=0);code.push_back(v);if(--d[v]==1)q.push(v);}
    return code;
}
vector<vector<int>> prufer_decode(const vector<int>&code)
{
    int n=code.size()+2;vector<int>d(n,1);for(int x:code){assert(x>=0&&x<n);d[x]++;}
    priority_queue<int,vector<int>,greater<int>> q;for(int i=0;i<n;i++)if(d[i]==1)q.push(i);vector<vector<int>>g(n);
    auto edge=[&](int u,int v){g[u].push_back(v);g[v].push_back(u);};
    for(int v:code){int u=q.top();q.pop();edge(u,v);if(--d[v]==1)q.push(v);}
    int u=q.top();q.pop();edge(u,q.top());return g;
}
