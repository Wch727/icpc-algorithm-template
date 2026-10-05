// 费用流MCMF 的测试与对拍代码
// 模板本体：04-图论/费用流MCMF.cpp
#include "../../04-图论/费用流MCMF.cpp"

int main()
{
    // 自测 1：手造网络 1->2(2,1) 1->3(2,2) 2->4(1,4) 3->4(2,1) 2->3(1,1)
    n=4,s=1,t=4;
    add_edge(1,2,2,1),add_edge(1,3,2,2),add_edge(2,4,1,4),add_edge(3,4,2,1),add_edge(2,3,1,1);
    mcmf();
    printf("手造网络 最大流=%lld 最小费用=%lld（期望 3 11）\n",ans_flow,ans_cost);
    // 自测 2：随机小图，与「枚举每条边的流量」暴力对拍
    for(int T=1;T<=300;T++)
    {
        n=rand()%2+3;// 3 或 4 个点
        s=1,t=n;
        for(int i=1;i<=n;i++)adj[i].clear();
        e.clear();
        int eu[12],ev[12],ec[12],ew[12];
        int pot[12];
        for(int i=1;i<=n;i++)pot[i]=rand()%7;// 用势能差造费用，保证任何环的费用和 >= 0（无负费用环）
        m=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&m<6&&rand()%2)
                {
                    m++;
                    eu[m]=i,ev[m]=j,ec[m]=rand()%3+1;// 容量 1..3
                    ew[m]=pot[j]-pot[i]+rand()%3;// 费用可负，但环上求和非负
                    add_edge(i,j,ec[m],ew[m]);
                }
        mcmf();
        // 暴力：枚举所有边的流量，只保留流量守恒的方案，再取「流量最大、同流量费用最小」
        ll want_flow=0,want_cost=0;
        int tot=1;
        for(int i=1;i<=m;i++)tot*=(ec[i]+1);
        for(int mask=0;mask<tot;mask++)
        {
            int x=mask,bal[12]={0};
            ll c=0;
            for(int i=1;i<=m;i++)
            {
                int f=x%(ec[i]+1);
                x/=(ec[i]+1);
                bal[eu[i]]-=f,bal[ev[i]]+=f;
                c+=1LL*f*ew[i];
            }
            int ok=1;
            for(int i=1;i<=n;i++)
                if(i!=s&&i!=t&&bal[i]!=0)ok=0;
            if(!ok||bal[t]<0)continue;// 中间点不守恒，或流量为负
            if(bal[t]>want_flow||(bal[t]==want_flow&&c<want_cost))want_flow=bal[t],want_cost=c;
        }
        if(ans_flow!=want_flow||ans_cost!=want_cost)
        {
            printf("WA T=%d n=%d got=(%lld,%lld) want=(%lld,%lld)\n",T,n,ans_flow,ans_cost,want_flow,want_cost);
            return 0;
        }
    }
    printf("费用流 与暴力枚举 300 组通过\n");
    return 0;
}
/* SPFA 先建好势能，之后每轮用 Dijkstra 找约化费用最短路：
   h[v] = h[u] + fee(u,v) 时约化费用非负，dis[t] 变化后把 dis 累加回 h
   每次沿最短路把瓶颈流量推满，就是最小费用最大流（SSP）
   注意原图不能有负费用环，否则最小费用没有下界（会死循环）
   只想要最小费用可行流时，可以在 dis[t]>=0 时 break（费用不再下降） */
