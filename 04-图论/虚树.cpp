#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct VirtualTree
{
    int n,tim,lg;
    vector<vector<int>> adj,up,tr;
    vector<int> dep,dfn,ed,mark,cnt;
    VirtualTree(int n):n(n),tim(0),lg(1),adj(n+1),tr(n+1),dep(n+1),dfn(n+1),ed(n+1),mark(n+1),cnt(n+1)
    {
        while((1<<lg)<=n)lg++;
        up.assign(lg,vector<int>(n+1));
    }
    void add_edge(int u,int v)
    {
        adj[u].push_back(v),adj[v].push_back(u);
    }
    // 迭代预处理，O(n log n)，避免链上递归爆栈
    void build()
    {
        vector<pair<int,int>> st;
        st.push_back({1,0});
        while(!st.empty())
        {
            int u=st.back().first,&i=st.back().second;
            if(i==0)
            {
                dfn[u]=++tim;
                for(int j=1;j<lg;j++)up[j][u]=up[j-1][up[j-1][u]];
            }
            if(i==(int)adj[u].size())
            {
                ed[u]=tim,st.pop_back();
                continue;
            }
            int v=adj[u][i++];
            if(v==up[0][u])continue;
            up[0][v]=u,dep[v]=dep[u]+1;
            st.push_back({v,0});
        }
    }
    bool anc(int u,int v){return dfn[u]<=dfn[v]&&ed[v]<=ed[u];}
    int lca(int u,int v)
    {
        if(anc(u,v))return u;
        for(int j= lg - 1; j >= 0; j--)
            if(up[j][u] && !anc(up[j][u], v))
                u= up[j][u];
        return up[0][u];
    }
    // 栈构建虚树，DP 求所有关键点对距离和；O(k log k+k log n)
    ll query(vector<int> a)
    {
        if(a.empty())return 0;
        sort(a.begin(), a.end(), [&](int u, int v) { return dfn[u] < dfn[v]; });
        a.erase(unique(a.begin(),a.end()),a.end());
        int k=a.size();
        for(int u:a)mark[u]=1;
        for(int i=1;i<k;i++)a.push_back(lca(a[i-1],a[i]));
        sort(a.begin(), a.end(), [&](int u, int v) { return dfn[u] < dfn[v]; });
        a.erase(unique(a.begin(),a.end()),a.end());
        vector<int> st;
        for(int u:a)tr[u].clear(),cnt[u]=0;
        for(int u:a)
        {
            while(!st.empty()&&!anc(st.back(),u))st.pop_back();
            if(!st.empty())tr[st.back()].push_back(u);
            st.push_back(u);
        }
        ll ans=0;
        for(int i=(int)a.size()-1;i>=0;i--)
        {
            int u=a[i];
            cnt[u]+=mark[u];
            for(int v:tr[u])
            {
                ans+=1LL*(dep[v]-dep[u])*cnt[v]*(k-cnt[v]);
                cnt[u]+=cnt[v];
            }
            mark[u]=0;
        }
        return ans;
    }
};
