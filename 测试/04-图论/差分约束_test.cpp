// 差分约束 的测试与对拍代码
// 模板本体：04-图论/差分约束.cpp
#include "../../04-图论/差分约束.cpp"

int eu[N],ev[N];// 自测用：记下所有约束，方便代回验证
ll ew[N];

int main()
{
    // 自测 1：手造不等式组 x2-x1<=3，x3-x2<=-2，x3-x1<=1
    n=3,m=3;
    eu[1]=1,ev[1]=2,ew[1]=3;
    eu[2]=2,ev[2]=3,ew[2]=-2;
    eu[3]=1,ev[3]=3,ew[3]=1;
    for(int i=1;i<=m;i++)add_leq(eu[i],ev[i],ew[i]);
    printf("手造组 有解=%d x=",solve());
    for(int i=1;i<=n;i++)printf("%lld ",dis[i]);
    printf("（期望 有解=1 x= 0 0 -2 ）\n");
    // 自测 2：矛盾组 x2-x1<=-1 与 x1-x2<=-1，绕一圈权 -2
    clear_all();
    n=2,m=2;
    eu[1]=1,ev[1]=2,ew[1]=-1;
    eu[2]=2,ev[2]=1,ew[2]=-1;
    for(int i=1;i<=m;i++)add_leq(eu[i],ev[i],ew[i]);
    printf("矛盾组 有解=%d（期望 0）\n",solve());
    // 自测 3：随机不等式组，用 Floyd 判负环对齐「有无解」，有解时代回验证
    for(int T=1;T<=300;T++)
    {
        clear_all();
        n=rand()%3+3;// 3..5 个变量
        m=0;
        ll fl[10][10];
        for(int i=0;i<=n;i++)
            for(int j=0;j<=n;j++)fl[i][j]=(i==j?0:INF);
        for(int j=1;j<=n;j++)fl[0][j]=0;// 超级源点 0 到每个变量权 0
        for(int i=1;i<=8;i++)
        {
            if(rand()%3==0)continue;
            int u=rand()%n+1,v=rand()%n+1;
            ll c=rand()%11-5;
            m++;
            eu[m]=u,ev[m]=v,ew[m]=c;
            add_leq(u,v,c);
            fl[u][v]=min(fl[u][v],c);
        }
        int got=solve();
        // Floyd 判负环作为独立判据
        for(int k=0;k<=n;k++)
            for(int i=0;i<=n;i++)
                for(int j=0;j<=n;j++)
                    if(fl[i][k]+fl[k][j]<fl[i][j])fl[i][j]=fl[i][k]+fl[k][j];
        int neg=0;
        for(int i=0;i<=n;i++)if(fl[i][i]<0)neg=1;
        if(got==neg)
        {
            printf("WA T=%d n=%d 有解=%d 负环=%d\n",T,n,got,neg);
            return 0;
        }
        if(got)// 有解就把 dis 代回所有不等式
        {
            for(int i=1;i<=m;i++)
                if(dis[ev[i]]>dis[eu[i]]+ew[i])
                {
                    printf("WA 解不合法 T=%d 边(%d->%d,%lld)\n",T,eu[i],ev[i],ew[i]);
                    return 0;
                }
        }
    }
    printf("差分约束 与 Floyd 判负环 + 代回验证 300 组通过\n");
    return 0;
}
/* 形式统一成 x_v <= x_u + c，然后跑最短路：
   最短路存在（无负环）时 dis[] 就是一组可行解，有负环就无解
   求最长路版：把 x_v - x_u >= c 看成 v->u 权 -c 的最短路，或者全部取负号跑最长路
   差分约束也可用 spfa 判负环 + 输出 dis；点多时把 spfa 换成势能 Dijkstra（初值先 spfa） */
