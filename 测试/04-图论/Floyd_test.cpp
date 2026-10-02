// Floyd 的测试与对拍代码
// 模板本体：04-图论/Floyd.cpp
#include "../../04-图论/Floyd.cpp"

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        n=3; for(int i=1;i<=n;i++)for(int j=1;j<=n;j++)d[i][j]=i==j?0:INF;
        d[2][3]=-5; floyd(); assert(d[1][3]==INF&&d[2][3]==-5);
    }

    // 自测 1：手造有向图 1->2(3) 1->4(7) 2->3(2) 3->1(1) 3->4(1) 4->3(2)
    n=4,m=6;
    int a[8]={0,1,1,2,3,3,4},b[8]={0,2,4,3,1,4,3},c[8]={0,3,7,2,1,1,2};
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)d[i][j]=(i==j?0:INF);
    for(int i=1;i<=m;i++)d[a[i]][b[i]]=min(d[a[i]][b[i]],(ll)c[i]);
    floyd();
    printf("手造图全源最短路：\n");
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
            if(d[i][j]>=INF/2)printf("INF ");
            else printf("%lld ",d[i][j]);
        printf("\n");
    }
    printf("期望 0 3 5 6 / INF 0 2 3 / 1 4 0 1 / 3 6 2 0\n");
    // 自测 2：传递闭包，不能到自己的点（无环）应为 0
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)reach[i][j]=(i!=j&&d[i][j]<INF/2);
    closure();
    printf("传递闭包：\n");
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)printf("%d",reach[i][j]?1:0);
        printf("\n");
    }
    printf("期望 0111 / 1111 / 1111 / 1111（每行都能到自己）\n");
    // 自测 3：随机图与 Dijkstra 对拍
    n=30;
    for(int t=1;t<=100;t++)
    {
        ll g[35][35];
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)g[i][j]=(i==j?0:INF);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&rand()%8==0)g[i][j]=rand()%100+1;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)d[i][j]=g[i][j];
        floyd();
        for(int st=1;st<=n;st++)
        {
            ll dj[35];
            bool vis[35];
            for(int i=1;i<=n;i++)dj[i]=INF,vis[i]=0;
            dj[st]=0;
            for(int i=1;i<=n;i++)
            {
                int u=0;
                for(int j=1;j<=n;j++)
                    if(!vis[j]&&(u==0||dj[j]<dj[u]))u=j;
                if(u==0||dj[u]>=INF)break;
                vis[u]=1;
                for(int v=1;v<=n;v++)
                    if(dj[u]+g[u][v]<dj[v])dj[v]=dj[u]+g[u][v];
            }
            for(int i=1;i<=n;i++)
                if(dj[i]!=d[st][i])
                {
                    printf("WA t=%d st=%d i=%d floyd=%lld dij=%lld\n",t,st,i,d[st][i],dj[i]);
                    return 0;
                }
        }
    }
    printf("与 Dijkstra 对拍 100 组通过\n");
    return 0;
}
/* 三重循环 k 必须在最外层；d[i][i] 初始 0，负环时会出现负的自环值 */
