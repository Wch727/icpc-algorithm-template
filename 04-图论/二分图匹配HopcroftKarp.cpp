// 左侧 n、右侧 m，均从 0 编号；O(E sqrt(V))，vector 邻接表只存左到右。
// bfs 建最短增广层次，dfs 每轮只找最短路径；返回匹配数，a/b 保存双方匹配，-1 未匹配。
#include<bits/stdc++.h>
using namespace std;
struct HopcroftKarp
{
    int n,m,limit;vector<vector<int>> g;vector<int> a,b,d,it;
    HopcroftKarp(int n,int m):n(n),m(m),g(n),a(n,-1),b(m,-1),d(n),it(n){}
    bool bfs()
    {
        queue<int> q;fill(d.begin(),d.end(),-1);limit=INT_MAX;
        for(int u=0;u<n;u++)if(a[u]<0)d[u]=0,q.push(u);
        while(!q.empty())
        {
            int u=q.front();q.pop();if(d[u]>=limit)continue;
            for(int v:g[u])if(b[v]<0)limit=d[u]+1;else if(d[b[v]]<0)d[b[v]]=d[u]+1,q.push(b[v]);
        }
        return limit<INT_MAX;
    }
    bool dfs(int u)
    {
        for(int &i=it[u];i<(int)g[u].size();i++)
        {
            int v=g[u][i];
            if((b[v]<0&&d[u]+1==limit)||(b[v]>=0&&d[b[v]]==d[u]+1&&dfs(b[v])))
            {a[u]=v;b[v]=u;return true;}
        }
        d[u]=-1;return false;
    }
    int solve(){int ans=count_if(a.begin(),a.end(),[](int x){return x>=0;});while(bfs()){fill(it.begin(),it.end(),0);for(int u=0;u<n;u++)if(a[u]<0&&dfs(u))ans++;}return ans;}
};
