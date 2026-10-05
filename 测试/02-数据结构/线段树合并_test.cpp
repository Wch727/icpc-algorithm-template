// 线段树合并 的测试与对拍代码
// 模板本体：02-数据结构/线段树合并.cpp
#include "../../02-数据结构/线段树合并.cpp"

// 只往儿子走，就不会顺着无向边爬回父亲或跑出子树
int vis[N],mark[N];
vector<int> son[N];

int brute_distinct(int r)
{
    for(int i=1;i<=n;i++)mark[i]=0,vis[i]=0;
    int cnt=0,top=0;
    int su[N];
    su[++top]=r;
    while(top)
    {
        int u=su[top--];
        mark[u]=1;
        if(!vis[col[u]])vis[col[u]]=1,cnt++;
        for(size_t i=0;i<son[u].size();i++)su[++top]=son[u][i];
    }
    return cnt;
}

int main()
{
    srand(20240513);

    // 1. 小样例：链 1-2-3-4-5，颜色 1 2 1 3 2
    //    子树颜色集：{1,2,3}=3、{2,1,3}=3、{1,3,2}=3、{3,2}=2、{2}=1
    n=5;
    int ini[]={0,1,2,1,3,2};
    for(int i=1;i<=n;i++)col[i]=ini[i];
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=1;i<n;i++)add_edge(i,i+1),add_edge(i+1,i);
    seg.init();
    dfs(1,0);
    printf("小样例(链): ans[1..5] =");
    for(int i=1;i<=n;i++)printf(" %d",ans[i]);
    printf("  (应为 3 3 3 2 1)\n");

    // 2. 对拍：随机树 + 随机颜色，与暴力 dfs 比较
    bool ok=true;
    for(int T=1;T<=30&&ok;T++)
    {
        n=rand()%60+1;
        int C=rand()%5+1;
        for(int i=1;i<=n;i++)col[i]=rand()%C+1,son[i].clear();
        for(int i=1;i<=n;i++)adj[i].clear();
        for(int i=2;i<=n;i++)      // 随机父结点，保证是棵树
        {
            int fa=rand()%(i-1)+1;
            add_edge(fa,i),add_edge(i,fa);
            son[fa].push_back(i);//暴力用：只往儿子走
        }
        seg.init();
        dfs(1,0);
        for(int r=1;r<=n;r++)
        {
            int want=brute_distinct(r);
            if(want!=ans[r])
            {
                printf("第 %d 轮错: 点 %d got=%d want=%d\n",T,r,ans[r],want);
                ok=false;
                break;
            }
        }
        printf("线段树合并第 %d 轮 %s (结点数=%d)\n",T,ok?"passed":"FAILED",seg.tot);
    }

    // 3. 菊花图：根 1 挂 n-1 个叶子，每个点一种颜色 -> 根答案是 n（n 种颜色）
    n=8;
    for(int i=1;i<=n;i++)col[i]=i,son[i].clear();
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=2;i<=n;i++)add_edge(1,i),add_edge(i,1),son[1].push_back(i);
    seg.init();
    dfs(1,0);
    printf("菊花图: ans[1]=%d (应=%d) ans[2]=%d (应=1)\n",ans[1],n,ans[2]);
    if(ans[1]!=n||ans[2]!=1)ok=false;

    // 4. 规模测试：n=100000 的随机树，全是 1 种颜色 -> 所有答案都是 1
    n=100000;
    for(int i=1;i<=n;i++)col[i]=1;
    for(int i=1;i<=n;i++)adj[i].clear();
    for(int i=2;i<=n;i++)
    {
        int fa=(int)(rand()%(i-1))+1;
        add_edge(fa,i),add_edge(i,fa);
    }
    seg.init();
    dfs(1,0);
    int bad=0;
    for(int i=1;i<=n;i++)if(ans[i]!=1)bad++;
    printf("规模: n=100000 单色 -> 错 %d 个, 结点数 %d\n",bad,seg.tot);
    printf("结果: %s\n",(ok&&bad==0)?"OK":"FAILED");
    return 0;
}
