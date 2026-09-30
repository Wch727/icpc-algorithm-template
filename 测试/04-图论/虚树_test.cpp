// 虚树 的测试与对拍代码
// 模板本体：04-图论/虚树.cpp
#include "../../04-图论/虚树.cpp"

int main()
{
    mt19937 rng(12345);
    for(int T=0;T<=150;T++)
    {
        int n=T?int(rng()%30+1):5;
        VirtualTree tr(n);
        vector<vector<int>> adj(n+1);
        for(int v=2;v<=n;v++)
        {
            int u=T?int(rng()%(v-1)+1):v-1;
            tr.add_edge(u,v),adj[u].push_back(v),adj[v].push_back(u);
        }
        tr.build();
        for(int q=1;q<=20;q++)
        {
            vector<int> a;
            for(int u=1;u<=n;u++)if(rng()%2)a.push_back(u);
            ll want=0;
            for(int i=0;i<(int)a.size();i++)
            {
                vector<int> dis(n+1,-1);
                queue<int> que;
                dis[a[i]]=0,que.push(a[i]);
                while(!que.empty())
                {
                    int u=que.front();
                    que.pop();
                    for(int v:adj[u])if(dis[v]<0)dis[v]=dis[u]+1,que.push(v);
                }
                for(int j=i+1;j<(int)a.size();j++)want+=dis[a[j]];
            }
            if(tr.query(a)!=want){printf("FAILED\n");return 1;}
        }
    }
    printf("虚树 OK\n");
    return 0;
}
