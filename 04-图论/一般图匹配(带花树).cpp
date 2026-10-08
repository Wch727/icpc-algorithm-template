#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// O(n^3)，Edmonds 缩花，顶点从 1 开始；允许奇环
struct Blossom
{
    int n;
    vector<vector<int> > adj;
    vector<int> match,fa,base,vis,flower;
    Blossom(int n):n(n),adj(n+1),match(n+1),fa(n+1),base(n+1),vis(n+1),flower(n+1){}
    void add_edge(int u,int v)
    {
        if(u!=v)adj[u].push_back(v),adj[v].push_back(u);
    }
    int lca(int u,int v)
    {
        vector<int> mark(n+1);
        while(true)
        {
            u=base[u],mark[u]=1;
            if(!match[u])break;
            u=fa[match[u]];
        }
        while(true)
        {
            v=base[v];
            if(mark[v])return v;
            v=fa[match[v]];
        }
    }
    void mark_path(int u,int b,int v)
    {
        while(base[u]!=b)
        {
            flower[base[u]]=flower[base[match[u]]]=1;
            fa[u]=v,v=match[u],u=fa[match[u]];
        }
    }
    int augment(int s)
    {
        fill(vis.begin(),vis.end(),0),fill(fa.begin(),fa.end(),0);
        iota(base.begin(),base.end(),0);
        vector<int> q(1,s);
        vis[s]=1;
        for(int i=0;i<(int)q.size();i++)
        {
            int u=q[i];
            for(int v:adj[u])
            {
                if(base[u]==base[v]||match[u]==v)continue;
                if(v==s||(match[v]&&fa[match[v]]))
                {
                    int b=lca(u,v);
                    fill(flower.begin(),flower.end(),0);
                    mark_path(u,b,v),mark_path(v,b,u);
                    for(int w= 1; w <= n; w++)
                        if(flower[base[w]])
                        {
                            base[w]= b;
                            if(!vis[w])
                                vis[w]= 1, q.push_back(w);
                        }
                }
                else if(!fa[v])
                {
                    fa[v]=u;
                    if(!match[v])
                    {
                        while(v)
                        {
                            int p=fa[v],w=match[p];
                            match[v]=p,match[p]=v,v=w;
                        }
                        return 1;
                    }
                    vis[match[v]]=1,q.push_back(match[v]);
                }
            }
        }
        return 0;
    }
    int run()
    {
        fill(match.begin(),match.end(),0);
        int ans=0;
        for(int i= 1; i <= n; i++)
            if(!match[i])
                ans+= augment(i);
        return ans;
    }
};
