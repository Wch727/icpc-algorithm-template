// Tarjan强连通分量 的测试与对拍代码
// 模板本体：04-图论/Tarjan强连通分量.cpp
#include "../../04-图论/Tarjan强连通分量.cpp"

int main()
{
    // 自测 1：手造图 1->2 2->3 3->1 3->4 4->5 5->4，应有 2 个 SCC
    n=5,m=6;
    int a[10]={0,1,2,3,3,4},b[10]={0,2,3,1,5,5};
    for(int i=1;i<=m;i++)add_edge(a[i],b[i]);
    for(int i=1;i<=n;i++)if(!dfn[i])tarjan(i);
    printf("手造图 SCC 个数=%d（期望 3）\n",scc_cnt);
    printf("belong: ");
    for(int i=1;i<=n;i++)printf("%d ",belong[i]);
    printf("\n期望 2 2 2 3 1 之类：1,2,3 同块，4、5 各自一块\n");
    // 缩点建 DAG + 最长点权和 DP
    for(int u=1;u<=n;u++)
        for(int v:adj[u])
            if(belong[u]!=belong[v])dag[belong[u]].push_back(belong[v]);
    for(int i=1;i<=scc_cnt;i++)val[i]=sz[i],dp[i]=sz[i];
    // 缩点后跑一遍拓扑排序，保证递推顺序正确（Tarjan 的出块顺序不一定是拓扑序）
    int deg[N],que[N],hh=0,tt=0;
    for(int i=1;i<=scc_cnt;i++)deg[i]=0;
    for(int u=1;u<=scc_cnt;u++)
        for(int i=0;i<(int)dag[u].size();i++)deg[dag[u][i]]++;
    for(int i=1;i<=scc_cnt;i++)if(!deg[i])que[tt++]=i;
    while(hh<tt)
    {
        int u=que[hh++];
        for(int i=0;i<(int)dag[u].size();i++)
        {
            int v=dag[u][i];
            dp[v]=max(dp[v],dp[u]+val[v]);
            if(--deg[v]==0)que[tt++]=v;
        }
    }
    int best=0;
    for(int i=1;i<=scc_cnt;i++)best=max(best,dp[i]);
    printf("最长点权和=%d（期望 4：1,2,3 三块点权和 3 再加 4 这块 1 个点）\n",best);
    // 自测 2：随机图，SCC 划分与「互相可达」(Floyd 传递闭包) 暴力对拍
    for(int t=1;t<=200;t++)
    {
        n=rand()%8+1;
        for(int i=1;i<=n;i++)adj[i].clear(),dag[i].clear();
        timer=0,top=0,scc_cnt=0;
        for(int i=1;i<=n;i++)dfn[i]=low[i]=in_stk[i]=belong[i]=sz[i]=0;
        int re[70][2],mm=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&rand()%3==0)
                {
                    add_edge(i,j);
                    mm++;
                    re[mm][0]=i,re[mm][1]=j;
                }
        for(int i=1;i<=n;i++)if(!dfn[i])tarjan(i);
        // 暴力求可达矩阵
        int r[10][10]={0};
        for(int i=1;i<=mm;i++)r[re[i][0]][re[i][1]]=1;
        for(int i=1;i<=n;i++)r[i][i]=1;
        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(r[i][k]&&r[k][j])r[i][j]=1;
        for(int i=1;i<=n;i++)
        {
            int want=0;
            for(int j=1;j<=n;j++)
                if(r[i][j]&&r[j][i])
                {
                    want=j;
                    break;
                }
            if(belong[i]!=belong[want])
            {
                printf("WA t=%d i=%d bel=%d repr=%d want_same\n",t,i,belong[i],belong[want]);
                return 0;
            }
            for(int j=1;j<=n;j++)
                if((belong[i]==belong[j])!=(r[i][j]&&r[j][i]))
                {
                    printf("WA t=%d i=%d j=%d 同 SCC 判定不一致\n",t,i,j);
                    return 0;
                }
        }
    }
    printf("SCC 划分与传递闭包对拍 200 组通过\n");
    return 0;
}
/* 缩点后按 Tarjan 出栈顺序编号，编号大的在拓扑序前面，所以 DP 正序扫即可
   若图很大（n>1e5）递归 tarjan 可能爆栈，赛场上可把 N 调小或改迭代版 */
