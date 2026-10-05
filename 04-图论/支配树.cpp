// Lengauer–Tarjan 支配树：固定入口 root 到 v 的所有路径都经过 idom[v]。
// 有向图 0-indexed，返回直接支配点；入口和不可达点为 -1。O((n+m)log n) 标准界。
// DFS 与并查集压缩均迭代，避免长链爆栈；割点算法不能替代入口相关的有向支配。
#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> g;
vector<int> dominator_tree(int root)
{
    int n=g.size();assert(0<=root&&root<n);vector<int> dfn(n),ord={-1},parent(n+1),idx(n),stack={root};
    dfn[root]=1;ord.push_back(root);
    while(!stack.empty()){int u=stack.back();if(idx[u]==(int)g[u].size()){stack.pop_back();continue;}int v=g[u][idx[u]++];if(!dfn[v]){dfn[v]=ord.size();parent[dfn[v]]=dfn[u];ord.push_back(v);stack.push_back(v);}}
    int count=ord.size()-1;vector<vector<int>> pred(count+1),bucket(count+1);
    for(int u=0;u<n;u++)if(dfn[u])for(int v:g[u])if(dfn[v])pred[dfn[v]].push_back(dfn[u]);
    vector<int> semi(count+1),label(count+1),ancestor(count+1),dom(count+1);iota(semi.begin(),semi.end(),0);label=semi;
    auto representative=[&](int v){vector<int> path;int x=v;while(ancestor[x]&&ancestor[ancestor[x]])path.push_back(x),x=ancestor[x];for(auto it=path.rbegin();it!=path.rend();it++){int u=*it,a=ancestor[u];if(semi[label[a]]<semi[label[u]])label[u]=label[a];ancestor[u]=ancestor[a];}return label[v];};
    for(int i=count;i>1;i--)
    {
        for(int v:pred[i])semi[i]=min(semi[i],semi[representative(v)]);
        bucket[semi[i]].push_back(i);ancestor[i]=parent[i];
        for(int v:bucket[parent[i]]){int u=representative(v);dom[v]=semi[u]<semi[v]?u:parent[i];}bucket[parent[i]].clear();
    }
    vector<int> answer(n,-1);
    for(int i=2;i<=count;i++){if(dom[i]!=semi[i])dom[i]=dom[dom[i]];answer[ord[i]]=ord[dom[i]];}
    return answer;
}
