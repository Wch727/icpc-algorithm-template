// Dijkstra 的测试与对拍代码
// 模板本体：04-图论/Dijkstra.cpp
#include "../../04-图论/Dijkstra.cpp"


int main()
{
    // 自测：随机非负权有向图，朴素 / 堆优化 两者比对
    n=12;
    for(int t=1;t<=300;t++)
    {
        for(int i=1;i<=n;i++)adj[i].clear();
        m=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)// 无向图，每条边只随一次机、只取一个权值
                if(rand()%3==0)
                {
                    int cc=rand()%20+1;
                    m++;
                    Edge e;
                    e.to=j,e.w=cc;
                    adj[i].push_back(e);
                    e.to=i;
                    adj[j].push_back(e);
                }
        for(int st=1;st<=n;st++)
        {
            ll r1[20],r2[20];
            dijkstra_naive(st);
            for(int i=1;i<=n;i++)r1[i]=dis[i];
            dijkstra_heap(st);
            for(int i=1;i<=n;i++)r2[i]=dis[i];
            for(int i=1;i<=n;i++)
                if(r1[i]!=r2[i])
                {
                    printf("WA t=%d st=%d i=%d %lld %lld\n",t,st,i,r1[i],r2[i]);
                    return 0;
                }
        }
    }
    printf("随机对拍 300 组全部通过：朴素=堆优化\n");
    // 手造小图：1->2(7) 1->3(9) 1->6(14) 2->3(10) 2->4(15) 3->4(11) 3->6(2) 4->5(6) 6->5(9)
    n=6,m=9;
    for(int i=1;i<=n;i++)adj[i].clear();
    int a[10]={0,1,1,1,2,2,3,3,4,6},b[10]={0,2,3,6,3,4,4,6,5,5},c[10]={0,7,9,14,10,15,11,2,6,9};
    for(int i=1;i<=m;i++)
    {
        Edge e;
        e.to=b[i],e.w=c[i];
        adj[a[i]].push_back(e);
    }
    dijkstra_heap(1);
    printf("手造图源点 1 的最短路：");
    for(int i=1;i<=n;i++)printf("%lld ",dis[i]);
    printf("\n期望 0 7 9 20 20 11\n");
    return 0;
}
/* 最短路模板，负权边不能用；INF 用 1e18，松弛时不再额外判 INF */
