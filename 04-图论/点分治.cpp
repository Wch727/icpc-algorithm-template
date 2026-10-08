// 适用：无权树精确距离点对计数；统计无序且两端不同的点对。
// 参数：顶点 1..n，n>=1，k 是边数距离；输入必须为连通树。
// 状态：vis 表示已删除重心，cnt[d] 是此前子树到当前重心距离为 d 的点数。
// 关键：每棵子树先查询、后入桶，避免把同一子树内部点对错误算作过重心路径。
// 易错：used 仅清本轮触及的桶，递归前必须清净；答案最多 n*(n-1)/2，用 ll。
// 复杂度：总 O(n log n)、空间 O(n)；重心递归深度 O(log n)，子树遍历用显式队列。
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
    // 摊还 O(1)，加无向树边 u-v；不要加入自环或额外环。
    void add_edge(int u,int v)
    {
        adj[u].push_back(v),adj[v].push_back(u);
    }
    int center(int rt)
    {
        vector<int> q(1,rt);
        fa[rt]=0;
        for(int i=0;i<(int)q.size();i++)
            for(int v : adj[q[i]])
                if(v != fa[q[i]] && !vis[v])
                    fa[v]= q[i], q.push_back(v);
        int tot=q.size(),c=rt,best=tot;
        for(int i=tot-1;i>=0;i--)
        {
            int u=q[i],mx=0;
            sz[u]=1;
            for(int v : adj[u])
                if(fa[v] == u && !vis[v])
                    sz[u]+= sz[v], mx= max(mx, sz[v]);
            mx=max(mx,tot-sz[u]);
            if(mx<best)best=mx,c=u;
        }
        return c;
    }
    // 本块 O(块大小)，连同递归 O(块大小*log(块大小))；rt 是当前块入口，返回其点对数。
    ll solve(int rt)
    {
        int c=center(rt);
        vis[c]=1;
        ll ans=0;
        vector<int> used(1,0);
        cnt[0]=1;
        for(int v : adj[c])
            if(!vis[v])
            {
                vector<array<int, 3>> q(1, {v, c, 1});
                vector<int> d;
                for(int i= 0; i < (int)q.size(); i++)
                {
                    int u= q[i][0], p= q[i][1], dep= q[i][2];
                    if(dep > k)
                        continue;
                    d.push_back(dep), ans+= cnt[k - dep];
                    for(int w : adj[u])
                        if(w != p && !vis[w])
                            q.push_back({w, u, dep + 1});
                }
                for(int dep : d)
                {
                    if(!cnt[dep])
                        used.push_back(dep);
                    cnt[dep]++;
                }
            }
        for(int d:used)cnt[d]=0;
        for(int v : adj[c])
            if(!vis[v])
                ans+= solve(v);
        return ans;
    }
    ll run()
    {
        if(k<=0||k>=n)return 0;
        fill(vis.begin(),vis.end(),0);
        return solve(1);
    }
};
