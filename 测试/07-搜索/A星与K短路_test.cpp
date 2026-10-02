// A星与K短路 的测试与对拍代码
// 模板本体：07-搜索/A星与K短路.cpp
#include "../../07-搜索/A星与K短路.cpp"

// ================= 二、暴力枚举所有简单路径，用于对拍 =================
// 只在 n<=8 时用，O(路径数)；把 st->en 的所有简单路长度收集起来排序

vector<int> bf_len;

void bf_dfs(int u,int d)
{
    if(u==en)
    {
        bf_len.push_back(d);
        return;
    }
    for(Edge e:g.adj[u])
    {
        int v=e.to,w=e.w;
        if(vis[v])continue;
        vis[v]=1;
        bf_dfs(v,d+w);
        vis[v]=0;
    }
}

// 返回第 k 短简单路的长度，不足返回 -1（同长度不同路径分别计数）

int brute_kth(int s,int t,int kk)
{
    bf_len.clear();
    memset(vis,0,sizeof(vis));
    vis[s]=1;
    bf_dfs(s,0);
    sort(bf_len.begin(),bf_len.end());
    if((int)bf_len.size()<kk)return -1;
    return bf_len[kk-1];
}

// ================= 四、自测 =================

void test_puzzle()
{
    int a[3][3]={{1,2,3},{4,5,6},{7,0,8}};
    printf("[A*八数码] 123456708 -> %d 步 (期望 1)\n",astar_puzzle(a));
    int b[3][3]={{1,2,3},{4,0,6},{7,5,8}};
    printf("[A*八数码] 123406758 -> %d 步 (期望 2)\n",astar_puzzle(b));
    int c[3][3]={{1,2,3},{4,5,6},{7,8,0}};
    printf("[A*八数码] 目标态 -> %d 步 (期望 0)\n",astar_puzzle(c));
}

void test_kth()
{
    mt19937 rnd(1234);
    int ok=1,fail=0,tested=0;
    for(int t=1;t<=400&&tested<30;t++)
    {
        n=rnd()%5+3;//3..7 个点
        g.init(n),rg.init(n);
        // 随机树：保证连通，且第 k 短路一定是简单路，可以和暴力完全对拍
        for(int i=2;i<=n;i++)
        {
            int u=rnd()%(i-1)+1,w=rnd()%5+1;
            g.add_edge(u,i,w),g.add_edge(i,u,w);
            rg.add_edge(i,u,w),rg.add_edge(u,i,w);
        }
        st=1,en=n;
        for(int kk=1;kk<=3;kk++)
        {
            ll got=kth_shortest(st,en,kk);
            int bf=brute_kth(st,en,kk);
            if(bf==-1)continue;//树上不足 k 条简单路时跳过（A* 会给出重复走点的走法）
            tested++;
            if(got!=bf)
            {
                ok=0,fail++;
                if(fail<=3)printf("  第 %d 组 k=%d: A*=%lld 暴力=%d\n",t,kk,got,bf);
            }
        }
    }
    printf("[k短路] %d 组随机树 与暴力枚举简单路 完全对拍 %s\n",tested,(assert(ok),ok?"全部通过":"失败"));

    // 含环的图上，A* 允许重复走点，答案不会长于「第 k 短简单路」
    ok=1,fail=0,tested=0;
    for(int t=1;t<=400&&tested<30;t++)
    {
        n=rnd()%5+3;
        g.init(n),rg.init(n);
        for(int i=2;i<=n;i++)
        {
            int u=rnd()%(i-1)+1,w=rnd()%5+1;
            g.add_edge(u,i,w),g.add_edge(i,u,w);
            rg.add_edge(i,u,w),rg.add_edge(u,i,w);
        }
        for(int i=1;i<=2;i++)
        {
            int u=rnd()%n+1,v=rnd()%n+1,w=rnd()%5+1;
            if(u==v)continue;
            g.add_edge(u,v,w),g.add_edge(v,u,w);
            rg.add_edge(v,u,w),rg.add_edge(u,v,w);
        }
        st=1,en=n;
        for(int kk=1;kk<=3;kk++)
        {
            ll got=kth_shortest(st,en,kk);
            int bf=brute_kth(st,en,kk);
            if(bf==-1)continue;
            tested++;
            if(got>bf||got<0)
            {
                ok=0,fail++;
                if(fail<=3)printf("  第 %d 组 k=%d: A*=%d 简单路暴力=%d\n",t,kk,got,bf);
            }
        }
    }
    printf("[k短路] %d 组带环图 校验 A* 结果不长于第 k 短简单路 %s\n",tested,(assert(ok),ok?"全部通过":"失败"));
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        n=3;g.init(n);rg.init(n);
        g.add_edge(1,2,2000000000);g.add_edge(2,3,2000000000);
        rg.add_edge(2,1,2000000000);rg.add_edge(3,2,2000000000);
        assert(kth_shortest(1,3,1)==4000000000LL);
        assert(kth_shortest(1,3,2)==-1&&kth_shortest(1,1,1)==0);
    }

    // 小样例：4 个点，无向边 1-2(1) 2-4(1) 1-3(1) 3-4(1) 1-4(5)
    // 两条长度 2 的路径 + 一条长度 4 的绕路 + 直连 5
    n=4,m=5,k=3,st=1,en=4;
    g.init(n),rg.init(n);
    int ee[5][3]={{1,2,1},{2,4,1},{1,3,1},{3,4,1},{1,4,5}};
    for(int i=0;i<5;i++)//正向图和反向图都要加，反向图用来算 h
    {
        int u=ee[i][0],v=ee[i][1],w=ee[i][2];
        g.add_edge(u,v,w),g.add_edge(v,u,w);
        rg.add_edge(v,u,w),rg.add_edge(u,v,w);
    }
    for(int i=1;i<=5;i++)
        printf("[k短路] 第 %d 短路 = %lld\n",i,kth_shortest(1,4,i));
    printf("[k短路] A* 允许重复走点，得到 2 2 4 4 4；暴力只数简单路，第 3 短 = %d\n",brute_kth(1,4,3));

    test_puzzle();
    test_kth();
    return 0;
}
