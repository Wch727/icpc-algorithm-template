// Bellman-Ford 的测试与对拍代码
// 模板本体：04-图论/Bellman-Ford.cpp
#include "../../04-图论/Bellman-Ford.cpp"

int main()
{
    // 自测 1：手造有向图（含负权边，无负环）
    // 1->2(6) 1->3(7) 2->3(8) 2->4(5) 2->5(-4) 3->4(-3) 3->5(9) 4->2(-2) 5->4(7)
    n=5,m=9;
    int a[12]={0,1,1,2,2,2,3,3,4,5},b[12]={0,2,3,3,4,5,4,5,2,4},c[12]={0,6,7,8,5,-4,-3,9,-2,7};
    for(int i=1;i<=m;i++)e[i].u=a[i],e[i].v=b[i],e[i].w=c[i];
    int bad=bellman_ford(1);
    printf("手造图 负环=%d dis=",bad);
    for(int i=1;i<=n;i++)printf("%lld ",dis[i]);
    printf("\n期望 负环=0 dis= 0 2 7 4 -2\n");
    // 自测 2：负环图 1->2(1) 2->3(-1) 3->2(-1)
    n=3,m=3;
    int a2[5]={0,1,2,3},b2[5]={0,2,3,2},c2[5]={0,1,-1,-1};
    for(int i=1;i<=m;i++)e[i].u=a2[i],e[i].v=b2[i],e[i].w=c2[i];
    printf("负环图 负环=%d（期望 1）\n",bellman_ford(1));
    // 自测 3：随机构造含负权的无负环图，与 Floyd 对拍
    n=9;
    int ok=1;
    for(int t=1;t<=200&&ok;t++)
    {
        int w[12][12];
        memset(w,0x3f,sizeof(w));
        m=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&rand()%40==0)
                {
                    w[i][j]=rand()%30-10;
                    m++;
                    e[m].u=i,e[m].v=j,e[m].w=w[i][j];
                }
        int fl[12][12];
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)fl[i][j]=(i==j?0:w[i][j]);
        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(fl[i][k]+fl[k][j]<fl[i][j])fl[i][j]=fl[i][k]+fl[k][j];
        for(int st=1;st<=n;st++)
        {
            if(bellman_ford(st))
            {
                ok=0;
                break;
            }
            for(int i=1;i<=n;i++)
            {
                ll want=(fl[st][i]>=(int)1e8?INF:fl[st][i]);
                if(dis[i]!=want)
                {
                    printf("WA t=%d st=%d i=%d got=%lld want=%lld\n",t,st,i,dis[i],want);
                    return 0;
                }
            }
        }
    }
    printf("与 Floyd 对拍 200 组通过\n");
    return 0;
}
/* 松弛时先判 dis[e[j].u]<INF，避免 INF+负数 溢出 */
