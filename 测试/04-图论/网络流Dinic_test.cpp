// 网络流Dinic 的测试与对拍代码
// 模板本体：04-图论/网络流Dinic.cpp
#include "../../04-图论/网络流Dinic.cpp"

int main()
{
    // 第二条增广路需要撤销第一次匹配；另覆盖自环、零容量和重复调用。
    n=6,s=1,t=6;
    add_edge(1,2,1); add_edge(1,3,1);
    add_edge(2,4,1); add_edge(2,5,1); add_edge(3,4,1);
    add_edge(4,6,1); add_edge(5,6,1);
    add_edge(2,2,7); add_edge(1,6,0);
    assert(dinic()==2&&dinic()==0);
    for(int u=1;u<=n;u++)adj[u].clear();
    e.clear();
    // 自测 1：手造网络 1->2(3) 1->3(2) 2->3(1) 2->4(2) 3->4(3)，最大流 = 5
    n=4,s=1,t=4;
    add_edge(1,2,3),add_edge(1,3,2),add_edge(2,3,1),add_edge(2,4,2),add_edge(3,4,3);
    printf("手造网络 最大流=%lld（期望 5）\n",dinic());
    // 自测 2：随机小网络，最大流 = 最小割（枚举源点侧点集暴力求割）
    for(int T=1;T<=200;T++)
    {
        n=rand()%5+3;
        for(int i=1;i<=n;i++)adj[i].clear();
        e.clear();
        s=1,t=n;
        int eu[60],ev[60];
        ll ew[60];
        m=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&rand()%2)
                {
                    m++;
                    eu[m]=i,ev[m]=j,ew[m]=rand()%8+1;
                    add_edge(i,j,ew[m]);
                }
        ll got=dinic();
        // 暴力最小割：枚举所有包含 s 不含 t 的点集 S，割 = 从 S 出去的边权和
        ll best=INF;
        for(int mask=0;mask<(1<<n);mask++)
        {
            if(!(mask&1))continue;// 必须含 s
            if(mask>>(n-1)&1)continue;// 不能含 t
            ll sum=0;
            for(int i=1;i<=m;i++)
                if((mask>>(eu[i]-1)&1)&&!(mask>>(ev[i]-1)&1))sum+=ew[i];
            best=min(best,sum);
        }
        if(got!=best)
        {
            printf("WA T=%d got=%lld mincut=%lld\n",T,got,best);
            return 0;
        }
    }
    printf("最大流=最小割 对拍 200 组通过\n");
    // 自测 3：二分图匹配型网络，与匈牙利结果对拍
    for(int T=1;T<=200;T++)
    {
        int nl=rand()%4+1,nr=rand()%4+1;
        n=nl+nr+2;
        for(int i=1;i<=n;i++)adj[i].clear();
        e.clear();
        s=n-1,t=n;
        for(int i=1;i<=nl;i++)add_edge(s,i,1);
        for(int j=1;j<=nr;j++)add_edge(nl+j,t,1);
        int mm=0,ea[30],eb[30];
        for(int i=1;i<=nl;i++)
            for(int j=1;j<=nr;j++)
                if(rand()%2)
                {
                    add_edge(i,nl+j,1);
                    mm++;
                    ea[mm]=i,eb[mm]=j;
                }
        ll got=dinic();
        // 暴力枚举所有匹配，求最大匹配
        int want=0;
        for(int mask=0;mask<(1<<mm);mask++)
        {
            int lu[8]={0},rv[8]={0},c=0,ok=1;
            for(int i=1;i<=mm&&ok;i++)
                if(mask>>(i-1)&1)
                {
                    if(lu[ea[i]]||rv[eb[i]])ok=0;
                    else lu[ea[i]]=1,rv[eb[i]]=1,c++;
                }
            if(ok)want=max(want,c);
        }
        if(got!=want)
        {
            printf("WA 二分图 T=%d got=%lld want=%d\n",T,got,want);
            return 0;
        }
    }
    printf("Dinic 求二分图最大匹配 对拍 200 组通过\n");
    return 0;
}
/* 最小割含义：跑完最大流后，从源点只沿 cap>0 的边走能到达的点集就是 S 侧，
   S 到 T 的边全部满流，这些边的容量和 = 最大流 = 最小割
   必须多路增广（while dfs）而不是一次 bfs 只推一条路，否则会退化成 EK */
