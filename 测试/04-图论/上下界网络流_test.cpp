// 上下界网络流 的测试与对拍代码
// 模板本体：04-图论/上下界网络流.cpp
#include "../../04-图论/上下界网络流.cpp"

int check_flow()// 自测用：检查当前图上的真实流量在上下界内且所有点流量守恒
{
    ll bal[12]={0};
    for(int i=1;i<=m;i++)
    {
        ll f=elow[i]+e[eidx[i]^1].cap;
        if(f<elow[i]||f>eup[i])return 0;
        bal[eu[i]]-=f,bal[ev[i]]+=f;
    }
    for(int i=1;i<=n;i++)if(bal[i])return 0;
    return 1;
}

int main()
{
    // 自测 1：无源汇可行流，三点成环 1->2[1,2] 2->3[1,2] 3->1[1,2]，各流 1 或 2 都行
    n=3,m=3;
    eu[1]=1,ev[1]=2,elow[1]=1,eup[1]=2;
    eu[2]=2,ev[2]=3,elow[2]=1,eup[2]=2;
    eu[3]=3,ev[3]=1,elow[3]=1,eup[3]=2;
    printf("无源汇成环 可行=%d（期望 1）\n",solve_lr(0)>=0);
    printf("实际流量=");
    for(int i=1;i<=m;i++)printf("%lld ",elow[i]+e[eidx[i]^1].cap);// 下界 + 残量反向边 = 真实流量
    printf("校验=%d（期望 1，环内可能多流到 2）\n",check_flow());
    // 自测 2：无源汇无解，1->2[2,2] 与 3->2[1,1]，点 2 只有入边没有出边
    n=3,m=2;
    eu[1]=1,ev[1]=2,elow[1]=2,eup[1]=2;
    eu[2]=3,ev[2]=2,elow[2]=1,eup[2]=1;
    printf("无源汇无解 可行=%d（期望 0）\n",solve_lr(0)>=0);
    // 自测 3：有源汇 s=1,t=4，边 1->2[0,2] 2->4[1,3] 1->3[0,2] 3->4[0,2]
    n=4,m=4,s=1,t=4;
    eu[1]=1,ev[1]=2,elow[1]=0,eup[1]=2;
    eu[2]=2,ev[2]=4,elow[2]=1,eup[2]=3;
    eu[3]=1,ev[3]=3,elow[3]=0,eup[3]=2;
    eu[4]=3,ev[4]=4,elow[4]=0,eup[4]=2;
    printf("有源汇上下界 最大流=%lld 最小流=%lld（期望 4 1）\n",solve_lr(1),solve_lr(2));
    // 自测 4：随机小图，与「枚举每条边流量」暴力对拍
    for(int T=1;T<=300;T++)
    {
        n=rand()%2+3;// 3 或 4 个点
        s=1,t=n;
        m=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&m<5&&rand()%2)
                {
                    m++;
                    eu[m]=i,ev[m]=j;
                    elow[m]=rand()%3;
                    eup[m]=elow[m]+rand()%3;// 上界不小于下界
                }
        // 暴力：枚举所有边的流量，看无源汇/有源汇分别有没有可行解
        int cyc_ok=0,src_ok=0,bmin=INT_MAX,bmax=-1;
        int tot=1;
        for(int i=1;i<=m;i++)tot*=(eup[i]-elow[i]+1);
        for(int mask=0;mask<tot;mask++)
        {
            int x=mask,bal[12]={0};
            for(int i=1;i<=m;i++)
            {
                int f=elow[i]+x%(eup[i]-elow[i]+1);
                x/=(eup[i]-elow[i]+1);
                bal[eu[i]]-=f,bal[ev[i]]+=f;
            }
            int all0=1;
            for(int i=1;i<=n;i++)if(bal[i])all0=0;
            if(all0)cyc_ok=1;// 所有点流量守恒 -> 无源汇可行
            int mid0=1;
            for(int i=1;i<=n;i++)if(i!=s&&i!=t&&bal[i])mid0=0;
            if(mid0)
            {
                bmin=min(bmin,bal[t]);// 最小流对所有可行流取最小，可以是负数
                if(bal[t]>=0)src_ok=1,bmax=max(bmax,bal[t]);// 有解要求存在流量非负的可行流
            }
        }
        ll got_cyc=(solve_lr(0)>=0),got_max=solve_lr(1),got_min=solve_lr(2);
        int bad=0;
        if(got_cyc!=cyc_ok)bad=1;
        if(src_ok)
        {
            if(got_max!=bmax||got_min!=bmin)bad=1;
        }
        else if(got_max!=-1||got_min!=-1)bad=1;
        if(bad)
        {
            printf("WA T=%d n=%d m=%d cyc got=%lld want=%d\n",T,n,m,got_cyc,cyc_ok);
            printf("  max got=%lld want=%d   min got=%lld want=%d\n",got_max,bmax,got_min,bmin);
            return 0;
        }
    }
    printf("上下界网络流 与暴力枚举 300 组通过\n");
    return 0;
}
/* 无源汇可行流：对每条边 [low,up] 建 up-low 的边，并记 d[u]-=low,d[v]+=low，
   补超级源 S 到所有 d>0 的点、所有 d<0 的点到超级汇 T，跑满则可行
   有源汇：先加 t->s 的无穷边转成无源汇，跑完可行流后拆掉超级源汇的边，
   再在残量网络上跑 s->t 的最大流 = 最大流，跑 t->s 的最大流从 base 里减掉 = 最小流
   流量按「流出源点 - 流入源点」定义，退流退过头时最小流可以是负数
   无解判定：转换后的无源汇问题跑不满（等价于不存在流量非负的可行流），和 LOJ117 一致 */
