#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// O(n log n)，统计距离为 k 的无序异点对；遍历子树不用递归
struct Centroid
{
    int n,k;
    vector<vector<int> > adj;
    vector<int> fa,sz,vis,cnt;
    Centroid(int n,int k):n(n),k(k),adj(n+1),fa(n+1),sz(n+1),vis(n+1),cnt(n+1){}
    void add_edge(int u,int v)
    {
        adj[u].push_back(v),adj[v].push_back(u);
    }
    int center(int rt)
    {
        vector<int> q(1,rt);
        fa[rt]=0;
        for(int i=0;i<(int)q.size();i++)
            for(int v:adj[q[i]])if(v!=fa[q[i]]&&!vis[v])fa[v]=q[i],q.push_back(v);
        int tot=q.size(),c=rt,best=tot;
        for(int i=tot-1;i>=0;i--)
        {
            int u=q[i],mx=0;
            sz[u]=1;
            for(int v:adj[u])if(fa[v]==u&&!vis[v])sz[u]+=sz[v],mx=max(mx,sz[v]);
            mx=max(mx,tot-sz[u]);
            if(mx<best)best=mx,c=u;
        }
        return c;
    }
    ll solve(int rt)
    {
        int c=center(rt);
        vis[c]=1;
        ll ans=0;
        vector<int> used(1,0);
        cnt[0]=1;
        for(int v:adj[c])if(!vis[v])
        {
            vector<array<int,3> > q(1,{v,c,1});
            vector<int> d;
            for(int i=0;i<(int)q.size();i++)
            {
                int u=q[i][0],p=q[i][1],dep=q[i][2];
                if(dep>k)continue;
                d.push_back(dep),ans+=cnt[k-dep];
                for(int w:adj[u])if(w!=p&&!vis[w])q.push_back({w,u,dep+1});
            }
            for(int dep:d)
            {
                if(!cnt[dep])used.push_back(dep);
                cnt[dep]++;
            }
        }
        for(int d:used)cnt[d]=0;
        for(int v:adj[c])if(!vis[v])ans+=solve(v);
        return ans;
    }
    ll run()
    {
        if(k<=0||k>=n)return 0;
        fill(vis.begin(),vis.end(),0);
        return solve(1);
    }
};
