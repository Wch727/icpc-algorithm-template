#include "../../04-图论/Kruskal重构树.cpp"
#include "../../04-图论/线段树优化建图.cpp"
#include "../../02-数据结构/整体二分(前缀判定).cpp"
const ll INF_TEST=LLONG_MAX/4;
vector<ll> distances(const vector<vector<pair<int,ll>>> &g,int s)
{
    vector<ll>d(g.size(),INF_TEST);d[s]=0;
    priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<pair<ll,int>>>q;q.push({0,s});
    while(!q.empty())
    {
        auto [x,u]=q.top();q.pop();if(x!=d[u])continue;
        for(auto [v,w]:g[u])if(d[v]>x+w)d[v]=x+w,q.push({d[v],v});
    }
    return d;
}
int main()
{
    mt19937 rng(20261002);
    for(int z=0;z<700;z++)
    {
        int n=1+rng()%12,m=rng()%35;
        vector<KruskalTree::Edge>e;
        vector<pair<int,int>>times,query;
        vector<vector<ll>>d(n+1,vector<ll>(n+1,INF_TEST));
        for(int u=1;u<=n;u++)d[u][u]=LLONG_MIN;
        for(int i=0;i<m;i++)
        {
            int u=1+rng()%n,v=1+rng()%n;ll w=(int)(rng()%21)-10;
            e.push_back({u,v,w}),times.push_back({u,v});
            d[u][v]=d[v][u]=min(d[u][v],w);
        }
        for(int k=1;k<=n;k++)for(int u=1;u<=n;u++)for(int v=1;v<=n;v++)d[u][v]=min(d[u][v],max(d[u][k],d[k][v]));
        KruskalTree t(n,e);
        for(int u=1;u<=n;u++)for(int v=1;v<=n;v++)
        {
            int x=t.lca(u,v);
            assert((x==0)==(d[u][v]==INF_TEST));if(x)assert(t.val[x]==d[u][v]);
            query.push_back({u,v});
        }
        for(ll w=-12;w<=12;w++)for(int u=1;u<=n;u++)
        {
            int count=0;
            for(int v=1;v<=n;v++)
            {
                bool same=d[u][v]<=w;
                assert((t.component(u,w)==t.component(v,w))==same);count+=same;
            }
            assert(t.cnt[t.component(u,w)]==count);
        }
        vector<int>expected(query.size(),m+1);
        vector<vector<int>>g(n+1);
        for(int at=0;at<=m;at++)
        {
            if(at){auto [u,v]=times[at-1];g[u].push_back(v),g[v].push_back(u);}
            for(int u=1;u<=n;u++)
            {
                vector<int>seen(n+1);queue<int>q;q.push(u),seen[u]=1;
                while(!q.empty()){int x=q.front();q.pop();for(int y:g[x])if(!seen[y])seen[y]=1,q.push(y);}
                for(int v=1;v<=n;v++)if(seen[v])expected[(u-1)*n+v-1]=min(expected[(u-1)*n+v-1],at);
            }
        }
        assert(first_connected(n,times,query)==expected);
    }
    for(int z=0;z<500;z++)
    {
        int n=1+rng()%10;RangeGraph t(n);vector<vector<pair<int,ll>>>g(n+1);
        for(int i=0;i<30;i++)
        {
            int u=1+rng()%n,v=1+rng()%n,l=1+rng()%n,r=1+rng()%n,L=1+rng()%n,R=1+rng()%n;
            if(l>r)swap(l,r);if(L>R)swap(L,R);ll w=rng()%12;
            switch(rng()%4)
            {
                case 0:t.add_edge(u,v,w);g[u].push_back({v,w});break;
                case 1:t.point_to_range(u,l,r,w);for(int x=l;x<=r;x++)g[u].push_back({x,w});break;
                case 2:t.range_to_point(l,r,u,w);for(int x=l;x<=r;x++)g[x].push_back({u,w});break;
                case 3:t.range_to_range(l,r,L,R,w);for(int x=l;x<=r;x++)for(int y=L;y<=R;y++)g[x].push_back({y,w});break;
            }
        }
        t.point_to_range(1,1,0,5),t.range_to_point(1,0,1,5),t.range_to_range(1,0,1,n,5);
        for(int s=1;s<=n;s++)
        {
            auto a=distances(t.g,s),b=distances(g,s);
            for(int v=1;v<=n;v++)assert(a[v]==b[v]);
        }
    }
    KruskalTree edge_case(3,{{1,2,LLONG_MIN},{2,3,LLONG_MAX}});
    assert(edge_case.cnt[edge_case.component(1,LLONG_MIN)]==2);
    assert(edge_case.val[edge_case.lca(1,3)]==LLONG_MAX);
    cout<<"PASS: reconstruction, interval graph, parallel binary search\n";
}
