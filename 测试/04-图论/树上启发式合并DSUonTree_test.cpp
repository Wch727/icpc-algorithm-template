// 树上启发式合并DSUonTree 的测试与对拍代码
// 模板本体：04-图论/树上启发式合并DSUonTree.cpp
#include "../../04-图论/树上启发式合并DSUonTree.cpp"

// 暴力：O(n^2) 逐个子树统计，只在小数据下用

void brute(int n_,int *bacnt,int *bamx)
{
    for(int u=1;u<=n_;u++)
    {
        int f[105]={0},s=0,mx=0;
        for(int x=1;x<=n_;x++)
        {
            int p=x,insub=0;
            while(p)
            {
                if(p==u){insub=1;break;}
                p=fa_[p];
            }
            if(insub){if(f[col[x]]==0)s++;f[col[x]]++;mx=max(mx,f[col[x]]);}
        }
        bacnt[u]=s,bamx[u]=mx;
    }
}

int main()
{
    srand(20240519);

    // 原期望写错：子树 3 只有颜色 2；根最大频次 3，子树 3 最大频次 2
    // 自测 1：手造树 1-2,1-3,2-4,2-5,3-6；颜色 1,1,2,1,2,2
    n=6,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,cnt[i]=0;
    add_edge(1,2),add_edge(1,3),add_edge(2,4),add_edge(2,5),add_edge(3,6);
    col[1]=1,col[2]=1,col[3]=2,col[4]=1,col[5]=2,col[6]=2;
    get_order(1);
    get_cnt(1,1);
    printf("树 1-2,1-3,2-4,2-5,3-6，颜色 1,1,2,1,2,2\n");
    printf("不同颜色数：");
    for(int u=1;u<=n;u++)printf("%d:%d ",u,color_cnt[u]);
    printf("（期望 2 2 1 1 1 1）\n");
    printf("最多颜色出现次数：");
    for(int u=1;u<=n;u++)printf("%d:%d ",u,color_mx[u]);
    printf("（期望 3 2 2 1 1 1）\n");

    // 自测 2：链 1-2-3-4 颜色 1,2,3,1，子树不同色数 3,3,2,1
    n=4,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,cnt[i]=0;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    col[1]=1,col[2]=2,col[3]=3,col[4]=1;
    get_order(1);
    get_cnt(1,1);
    printf("链颜色 1,2,3,1 不同色数：");
    for(int u=1;u<=n;u++)printf("%d:%d ",u,color_cnt[u]);
    printf("（期望 3 3 2 1）\n");

    // 自测 3：随机树随机颜色对拍
    bool ok=true;
    int round=0;
    for(int T=1;T<=300;T++)
    {
        n=rand()%11+2,ecnt=0;
        int C=rand()%4+1;
        for(int i=1;i<=n;i++)head[i]=0,cnt[i]=0;
        for(int i=2;i<=n;i++)add_edge(rand()%(i-1)+1,i);
        for(int i=1;i<=n;i++)col[i]=rand()%C+1;
        get_order(1);
        get_cnt(1,1);
        int bacnt[15],bamx[15];
        brute(n,bacnt,bamx);
        for(int u=1;u<=n;u++)
            if(color_cnt[u]!=bacnt[u]||color_mx[u]!=bamx[u])
            {
                printf("FAILED t=%d u=%d got=(%d,%d) want=(%d,%d)\n",
                       T,u,color_cnt[u],color_mx[u],bacnt[u],bamx[u]);
                ok=false;
                break;
            }
        if(!ok)break;
        if(++round%6==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("DSU on tree 对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 4：n=1e5 的链，只有 1 种颜色；不能爆栈
    n=100000,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,cnt[i]=0,col[i]=1;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    get_order(1);
    get_cnt(1,1);
    printf("大链 n=100000 单色：color_cnt[1]=%d（期望 1）color_mx[1]=%d（期望 100000）color_cnt[100000]=%d\n",
           color_cnt[1],color_mx[1],color_cnt[n]);
    return 0;
}
/* 用法：颜色从 1 开始编号（cnt 数组下标）。
   get_cnt(u,1) 之后，color_cnt[u]/color_mx[u] 就是子树 u 的答案。
   套路：先轻儿子且 keep=0 → 重儿子 keep=1 → 再暴力加轻儿子 → 加自己 → 记录答案。 */
