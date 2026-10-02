// 2-SAT 的测试与对拍代码
// 模板本体：04-图论/2-SAT.cpp
#include "../../04-图论/2-SAT.cpp"

int cx[105],cvx[105],cy[105],cvy[105];// 自测用：存下所有条件

int check_clauses()// 把 val[] 代回所有条件验证
{
    for(int i=1;i<=m;i++)
    {
        if(val[cx[i]]==cvx[i]||val[cy[i]]==cvy[i])continue;
        return 0;
    }
    return 1;
}

int main()
{
    // 自测 1：手造可满足式 (x1=1 或 x2=1) 且 (x1=0 或 x2=0) 且 (x2=0 或 x3=1)
    n=3,m=3;
    int ax[5]={0,1,1,2},avx[5]={0,1,0,0},ay[5]={0,2,2,3},avy[5]={0,1,0,1};
    for(int i=1;i<=m;i++)
    {
        cx[i]=ax[i],cvx[i]=avx[i],cy[i]=ay[i],cvy[i]=avy[i];
        add_clause(cx[i],cvx[i],cy[i],cvy[i]);
    }
    printf("手造2-SAT 有解=%d（期望 1） 赋值=",solve());
    for(int i=1;i<=n;i++)printf("%d ",val[i]);
    printf("检查=%d（期望 1）\n",check_clauses());
    // 自测 2：手造矛盾式 (x1=1 或 x1=1) 且 (x1=0 或 x1=0)
    clear_all();
    n=1,m=2;
    cx[1]=1,cvx[1]=1,cy[1]=1,cvy[1]=1;
    cx[2]=1,cvx[2]=0,cy[2]=1,cvy[2]=0;
    for(int i=1;i<=m;i++)add_clause(cx[i],cvx[i],cy[i],cvy[i]);
    printf("矛盾式 有解=%d（期望 0）\n",solve());
    // 自测 3：随机小式，与 2^n 枚举所有赋值暴力对拍
    for(int T=1;T<=500;T++)
    {
        clear_all();
        n=rand()%4+1;
        m=rand()%6+1;
        for(int i=1;i<=m;i++)
        {
            cx[i]=rand()%n+1,cvx[i]=rand()%2;
            cy[i]=rand()%n+1,cvy[i]=rand()%2;
            add_clause(cx[i],cvx[i],cy[i],cvy[i]);
        }
        int got=solve();
        int save[10];
        for(int i=1;i<=n;i++)save[i]=val[i];// 先把模板给的赋值存下来
        // 暴力：枚举 2^n 种赋值，看有没有能满足全部条件的
        int want=0;
        for(int mask=0;mask<(1<<n)&&!want;mask++)
        {
            for(int i=1;i<=n;i++)val[i]=(mask>>(i-1))&1;
            if(check_clauses())want=1;
        }
        for(int i=1;i<=n;i++)val[i]=save[i];// 还原成模板给的赋值再验证
        if(got!=want)
        {
            printf("WA T=%d n=%d got=%d want=%d\n",T,n,got,want);
            return 0;
        }
        if(got&&!check_clauses())// 有解时必须真的满足所有条件
        {
            printf("WA 赋值不合法 T=%d\n",T);
            return 0;
        }
    }
    printf("2-SAT 与 2^n 枚举 500 组通过\n");
    return 0;
}
/* 建图要点：每个变量拆成两个点 i（真）和 i+n（假）
   条件 (x=vx) 或 (y=vy) 拆成两条蕴含边：非 vx -> vy、非 vy -> vx
   同一个变量的真点和假点在同一强连通分量里就无解
   结论：scc 编号小的那个点在拓扑序更靠后，取它为真（Tarjan 出栈序是反拓扑序） */
