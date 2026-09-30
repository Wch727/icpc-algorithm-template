// 存图(链式前向星) 的测试与对拍代码
// 模板本体：04-图论/存图(链式前向星).cpp
#include "../../04-图论/存图(链式前向星).cpp"

int main()
{
    // 自测：4 个点 5 条边的有向图 + 同名无向图，两种存图结果必须完全一致
    n=4,m=5;
    int eu[6]={0,1,1,2,3,3},ev[6]={0,2,3,4,1,4},ew[6]={0,7,2,5,1,9};
    for(int i=1;i<=m;i++)add_edge(eu[i],ev[i],ew[i]),add_adj(eu[i],ev[i],ew[i]);
    printf("=== 有向图 链式前向星 ===\n");
    show_star();
    printf("=== 有向图 vector 邻接表 ===\n");
    show_adj();
    printf("=== 无向图 链式前向星(只印 u<v) ===\n");
    memset(head,0,sizeof(head)),num=0;
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=1;i<=m;i++)add_undirected(eu[i],ev[i],ew[i]),add_adj(eu[i],ev[i],ew[i]),add_adj(ev[i],eu[i],ew[i]);
    show_ud_star();
    printf("=== 无向图 vector 邻接表(只印 u<v) ===\n");
    show_ud_adj();
    printf("边数 num=%d（无向边正反各加一次，所以是 2*m=%d）\n",num,2*m);
    return 0;
}
/* 样例输入 4 5 / 1 2 7 / 1 3 2 / 2 4 5 / 3 1 1 / 3 4 9
期望：两种存图打印出的边表逐行相同，无向图 num=10 */
