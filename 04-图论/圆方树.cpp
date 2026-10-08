#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 无向图点双圆方森林，O(n+m)；忽略自环，支持重边
// Tarjan 使用递归，大链应增大栈或改成显式栈
struct BlockTree
{
    int n,num,tim;
    vector<vector<pair<int,int>>> adj;
    vector<vector<int>> tr;
    vector<int> dfn,low,st;
    BlockTree(int n):n(n),num(n),tim(0),adj(n+1),tr(2*n+1),dfn(n+1),low(n+1){}
    void add_edge(int u,int v,int id)
    {
        if(u==v)return;
        adj[u].push_back({v, id}), adj[v].push_back({u, id});
    }
    void dfs(int u,int pe)
    {
        dfn[u]=low[u]=++tim,st.push_back(u);
        for(pair<int,int> e:adj[u])
        {
            int v=e.first,id=e.second;
            if(id==pe)continue;
            if(!dfn[v])
            {
                dfs(v,id),low[u]=min(low[u],low[v]);
                if(low[v]>=dfn[u])
                {
                    ++num;
                    tr[num].push_back(u),tr[u].push_back(num);
                    int x;
                    do
                    {
                        x=st.back(),st.pop_back();
                        tr[num].push_back(x),tr[x].push_back(num);
                    }while(x!=v);
                }
            }
            else low[u]=min(low[u],dfn[v]);
        }
    }
    void build()
    {
        for(int u= 1; u <= n; u++)
            if(!dfn[u])
                dfs(u, -1), st.pop_back();
    }
    // 圆方树路径上的原点即必经点，包含两个端点；不连通返回 -1
    // 单次 O(n)，多询问可预处理 LCA 与路径前缀和至 O(log n)
    int query(int s,int t)
    {
        vector<int> fa(num+1,-1);
        queue<int> q;
        fa[s]=0,q.push(s);
        while(!q.empty())
        {
            int u=q.front();
            q.pop();
            for(int v : tr[u])
                if(fa[v] < 0)
                    fa[v]= u, q.push(v);
        }
        if(fa[t]<0)return -1;
        int ans=0;
        for(int u=t;u;u=fa[u])ans+=u<=n;
        return ans;
    }
};
