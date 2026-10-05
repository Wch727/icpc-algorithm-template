#include<bits/stdc++.h>
using namespace std;

// 必须先求最大匹配（仅“无法再直接配对”的极大匹配不够）。
// adj[1..nl] 是左点的右邻居；match[1..nr] 是右点匹配的左点，0 表示未匹配。
// 从未匹配左点沿交错路：左->右走非匹配边，右->左走匹配边。
// 最小点覆盖 = 未到达左点 + 到达右点；返回左右编号，O(n+m)。
// 最大独立集取该覆盖的补集；本结论仅适用于二分图。
vector<vector<int>> adj;
vector<int> match;
pair<vector<int>,vector<int>> vertex_cover()
{
    int nl=(int)adj.size()-1,nr=(int)match.size()-1;
    vector<int> ml(nl+1),vl(nl+1),vr(nr+1);
    for(int v=1;v<=nr;v++)if(match[v])ml[match[v]]=v;
    queue<int> q;
    for(int u=1;u<=nl;u++)if(!ml[u])vl[u]=1,q.push(u);
    while(!q.empty())
    {
        int u=q.front();q.pop();
        for(int v:adj[u])if(v!=ml[u]&&!vr[v])
        {
            vr[v]=1;
            int w=match[v];
            if(w&&!vl[w])vl[w]=1,q.push(w);
        }
    }
    pair<vector<int>,vector<int>> ans;
    for(int u=1;u<=nl;u++)if(!vl[u])ans.first.push_back(u);
    for(int v=1;v<=nr;v++)if(vr[v])ans.second.push_back(v);
    return ans;
}
