// 基环树 的测试与对拍代码
// 模板本体：04-图论/基环树.cpp
#include "../../04-图论/基环树.cpp"

int check_cycle_edges(int *c,int len)// 校验 c[1..len] 是简单环
{
    if(len<3)return 0;
    int seen[15]={0};
    for(int i=1;i<=len;i++)
    {
        if(seen[c[i]])return 0;
        seen[c[i]]=1;
    }
    for(int i=1;i<=len;i++)
    {
        int u=c[i],v=c[i%len+1],found=0;
        for(int e=head[u];e;e=nxt[e])if(to[e]==v){found=1;break;}
        if(!found)return 0;
    }
    return 1;
}
void dfs_cycle(int s,int u,int dep)// 从 s 出发找回到 s 的简单环，O(n!)（n 很小才用）
{
    bvis[u]=1,bpath[dep]=u;
    for(int e=head[u];e;e=nxt[e])
    {
        int v=to[e];
        if(v==s)
        {
            if(dep>=3)bcyc.insert(dep);// 只记环长
        }
        else if(!bvis[v])dfs_cycle(s,v,dep+1);
    }
    bvis[u]=0;
}
// 暴力最大独立集：枚举所有点集（只在小数据下用）
ll brute_mis(int n_)
{
    ll want=0;
    for(int mask=0;mask<(1<<n_);mask++)
    {
        int flag=1;
        ll s=0;
        for(int i=1;i<=n_;i++)if(mask>>(i-1)&1)s+=val[i];
        for(int u=1;u<=n_&&flag;u++)
        {
            if(!(mask>>(u-1)&1))continue;
            for(int e=head[u];e;e=nxt[e])
                if(to[e]>u&&(mask>>(to[e]-1)&1)){flag=0;break;}
        }
        if(flag)want=max(want,s);
    }
    return want;
}

int main()
{
    srand(20240516);

    // 自测 1：4 元环 1-2-3-4-1，再挂树：5-1、6-2、7-6
    // 点权 1..7 = 3,2,5,1,4,6,7；答案由暴力给出，不手算
    n=7,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0;
    val[1]=3,val[2]=2,val[3]=5,val[4]=1,val[5]=4,val[6]=6,val[7]=7;
    add_edge(1,2),add_edge(2,3),add_edge(3,4),add_edge(4,1),add_edge(1,5),add_edge(2,6),add_edge(6,7);
    int want1=brute_mis(n);
    find_ring_undirected(1);
    int save[15];
    for(int i=1;i<=rn;i++)save[i]=ring[i];
    printf("四元环挂树：找环得到 %d 元环（%s）：",rn,check_cycle_edges(save,rn)?"合法":"非法");
    for(int i=1;i<=rn;i++)printf(" %d",ring[i]);
    printf("\n最大独立集 %lld（暴力 %d）\n",max_independent_set(1),want1);

    // 自测 2：3 元环 1-2-3-1，4 挂在 1 上，权 1,2,3,4
    n=4,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0;
    val[1]=1,val[2]=2,val[3]=3,val[4]=4;
    add_edge(1,2),add_edge(2,3),add_edge(3,1),add_edge(1,4);
    int want2=brute_mis(n);
    find_ring_undirected(1);
    printf("三元环挂点 4：找环得到 %d 元环，最大独立集 %lld（暴力 %d）\n",rn,max_independent_set(1),want2);

    // 自测 3：有向基环树找环：1->2->3->4->2
    n=4,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0;
    // 有向样例只加出边，不能复用无向加边函数
    for(auto [u,v]:vector<pair<int,int> >{{1,2},{2,3},{3,4},{4,2}})
        to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    find_ring_directed(1);
    printf("有向图 1->2->3->4->2：环长 %d（期望 3）环上点",rn);
    for(int i=1;i<=rn;i++)printf(" %d",ring[i]);
    printf("\n");

    // 自测 4：随机基环树对拍：环长 + 最大独立集（暴力枚举子集）
    bool ok=true;
    int round=0;
    for(int T=1;T<=200;T++)
    {
        n=rand()%10+3;
        int base=n;// 先造 n 个点的树，再加一条边 → n 点 n 边
        int eu[15],ev[15],ec=0;
        for(int i=2;i<=base;i++)eu[++ec]=rand()%(i-1)+1,ev[ec]=i;
        int a=rand()%base+1,b=rand()%base+1;
        while(b==a)b=rand()%base+1;
        int dup=0;
        for(int i=1;i<=ec;i++)if((eu[i]==a&&ev[i]==b)||(eu[i]==b&&ev[i]==a))dup=1;
        if(dup){T--;continue;}// 保证图里只有一个环
        eu[++ec]=a,ev[ec]=b;
        m=ec;
        ecnt=0;
        for(int i=1;i<=n;i++)head[i]=0,val[i]=rand()%10+1;
        for(int i=1;i<=m;i++)add_edge(eu[i],ev[i]);
        // 先暴力枚举所有简单环（n 很小），记录环长的多重集
        bpc=0;
        for(int i=1;i<=n;i++)bvis[i]=0;
        bcyc.clear();
        for(int s=1;s<=n;s++)dfs_cycle(s,s,1);
        ll got=max_independent_set(1);// 顺带拿到 find_ring 给出的环（ring[1..rn]）
        // 校验 1：ring 是合法的简单环（相邻有边 + 首尾有边 + 无重复点）
        int rsave[15],rs=rn;
        for(int i=1;i<=rn;i++)rsave[i]=ring[i];
        if(!check_cycle_edges(rsave,rs))
        {
            printf("FAILED 环不是合法环 t=%d rn=%d\n",T,rs);
            ok=false;
            break;
        }
        // 校验 2：这个环长必须出现在暴力枚举的结果里
        if(!bcyc.count(rs))
        {
            printf("FAILED 环长 t=%d rn=%d\n",T,rs);
            ok=false;
            break;
        }
        // 校验 3：环上的点都为 on_ring，且环上点之间的边数正好等于环长
        int edge_on_ring=0,bad=0;
        for(int i=1;i<=rs;i++)if(!on_ring[rsave[i]])bad=1;
        for(int u=1;u<=n;u++)if(on_ring[u])
            for(int i=head[u];i;i=nxt[i])
                if(on_ring[to[i]])edge_on_ring++;
        edge_on_ring>>=1;
        if(bad||edge_on_ring!=rs)
        {
            printf("FAILED 环标记 t=%d rn=%d 环边数=%d\n",T,rs,edge_on_ring);
            ok=false;
            break;
        }
        // 校验 4：最大独立集与暴力枚举子集一致
        ll want=brute_mis(n);
        if(got!=want)
        {
            printf("FAILED 独立集 t=%d n=%d got=%lld want=%lld\n",T,n,got,want);
            printf("ring:");for(int i=1;i<=rn;i++)printf(" %d",ring[i]);printf("\n");
            for(int i=1;i<=m;i++)printf("边 %d-%d\n",eu[i],ev[i]);
            for(int u=1;u<=n;u++)printf("val[%d]=%d dp=(%lld,%lld)\n",u,val[u],dp0[u],dp1[u]);
            ok=false;
            break;
        }
        if(++round%5==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("基环树环长/最大独立集对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 5：n=1e5 大环，迭代不爆栈（环长 1e5，最大独立集 50000）
    n=100000,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,val[i]=1;
    for(int i=1;i<=n;i++)add_edge(i,i%n+1);
    ll big_ans=max_independent_set(1);
    printf("大环 n=100000：环长 %d（期望 100000）最大独立集 %lld（期望 50000）\n",
           rn,big_ans);
    return 0;
}
/* 用法：先 find_ring_undirected(1) 拿到环，给环点打 on_ring 标记，
   再对每个环点 tree_dp()，最后两遍链上 DP 取 max。
   注意：本文件假设图连通且恰好一个环；多环（仙人掌）不适用。 */
