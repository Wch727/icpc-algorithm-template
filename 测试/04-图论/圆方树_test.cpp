// 圆方树 的测试与对拍代码
// 模板本体：04-图论/圆方树.cpp
#include "../../04-图论/圆方树.cpp"

bool connected(const vector<vector<int>> &adj,int s,int t,int ban)
{
    if(s==ban||t==ban)return false;
    vector<int> vis(adj.size());
    queue<int> q;
    vis[s]=1,q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int v:adj[u])if(v!=ban&&!vis[v])vis[v]=1,q.push(v);
    }
    return vis[t];
}

int main()
{
    int bad=0;
    BlockTree s(4);
    s.add_edge(1,2,1),s.add_edge(2,3,2),s.add_edge(3,1,3),s.add_edge(3,4,4);
    s.build();
    bad+=s.query(1,4)!=3;
    mt19937 rng(9281);
    for(int T=1;T<=200;T++)
    {
        int n=rng()%12+1,id=0;
        BlockTree tr(n);
        vector<vector<int>> adj(n+1);
        for(int u=1;u<=n;u++)for(int v=u+1;v<=n;v++)if(rng()%3==0)
        {
            tr.add_edge(u,v,++id),adj[u].push_back(v),adj[v].push_back(u);
            if(rng()%4==0)tr.add_edge(u,v,++id);
        }
        tr.build();
        for(int u=1;u<=n;u++)for(int v=u;v<=n;v++)
        {
            int want=-1;
            if(connected(adj,u,v,0))
            {
                want=0;
                for(int x=1;x<=n;x++)want+=!connected(adj,u,v,x);
            }
            bad+=tr.query(u,v)!=want;
        }
    }
    printf("圆方树：%s\n",bad?"FAILED":"OK");
    if(bad)return 1;
    return 0;
}
