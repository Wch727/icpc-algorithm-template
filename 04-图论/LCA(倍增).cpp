#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m,root;
int head[N],to[N<<1],nxt[N<<1],num=0;
int dep[N],fa[N][20];// fa[u][k]：u 往上跳 2^k 步的祖先
int diff[N];// 树上差分数组

void add_edge(int u,int v)
{
    to[++num]=v,nxt[num]=head[u],head[u]=num;
}

void add_undirected(int u,int v)
{
    add_edge(u,v);
    add_edge(v,u);
}

void dfs(int u,int f)// 预处理深度和倍增表，O(n log n)
{
    dep[u]=dep[f]+1;
    fa[u][0]=f;
    for(int k=1;k<20;k++)fa[u][k]=fa[fa[u][k-1]][k-1];
    for(int i=head[u];i;i=nxt[i])
        if(to[i]!=f)dfs(to[i],u);
}

int lca(int x,int y)// 倍增求 LCA，O(log n)
{
    if(dep[x]<dep[y])swap(x,y);
    int d=dep[x]-dep[y];
    for(int k=0;k<20;k++)if(d>>k&1)x=fa[x][k];// 先把深的提到同一层
    if(x==y)return x;
    for(int k=19;k>=0;k--)
        if(fa[x][k]!=fa[y][k])x=fa[x][k],y=fa[y][k];
    return fa[x][0];
}

int dist(int x,int y)// 树上两点距离（边权为 1，带权就把 dep 换成前缀和）
{
    int f=lca(x,y);
    return dep[x]+dep[y]-2*dep[f];
}

void add_path(int x,int y)// 路径 (x,y) 差分打标记
{
    int f=lca(x,y);
    diff[x]++,diff[y]++,diff[f]-=2;// 边差分：点上减 2 次
}

void collect(int u,int f)// 自底向上合并差分，得到每条边被覆盖的次数
{
    for(int i=head[u];i;i=nxt[i])
        if(to[i]!=f)
        {
            collect(to[i],u);
            diff[u]+=diff[to[i]];
        }
}

int main()
{
    // 自测 1：手造树 1-2 1-3 2-4 2-5 3-6，验证 LCA 与距离
    n=6;
    int eu[6]={0,1,1,2,2,3},ev[6]={0,2,3,4,5,6};
    for(int i=1;i<n;i++)add_undirected(eu[i],ev[i]);
    root=1;
    dfs(root,0);
    int pa[11]={0,4,5,4,5,6,4,6,1,2,3},pb[11]={0,5,6,6,3,6,2,3,4,1,2};
    printf("LCA 与距离：\n");
    for(int i=1;i<=10;i++)
        printf("lca(%d,%d)=%d dist=%d\n",pa[i],pb[i],lca(pa[i],pb[i]),dist(pa[i],pb[i]));
    printf("期望：2/2 1/4 1/4 1/3 6/0 2/1 3/1 1/2 1/1 1/2（lca/dist）\n");
    // 自测 2：树上差分，路径 4-5、4-6，检查每条边被覆盖次数
    memset(diff,0,sizeof(diff));
    add_path(4,5);
    add_path(4,6);
    collect(root,0);
    printf("边 (u,父) 覆盖次数：");
    for(int u=2;u<=n;u++)printf("%d:%d ",u,diff[u]);
    printf("\n期望 2:1 3:1 4:2 5:1 6:1（路径 4-5 和 4-6 都经过边 4-2）\n");
    // 自测 3：随机树与暴力对拍（暴力沿父亲往上爬）
    for(int t=1;t<=200;t++)
    {
        n=rand()%30+2;
        for(int i=1;i<=n;i++)head[i]=0;
        num=0;
        for(int i=2;i<=n;i++)
        {
            int u=rand()%(i-1)+1;
            add_undirected(u,i);
        }
        root=1;
        dfs(root,0);
        for(int qq=1;qq<=30;qq++)
        {
            int x=rand()%n+1,y=rand()%n+1;
            // 暴力：把 x 到根路径标记，再让 y 往上直到碰上标记点
            int mark[40]={0};
            mark[0]=1;// 0 号点是根的父亲，作为哨兵防止越界
            for(int p=x;p;p=fa[p][0])mark[p]=1;
            int z=y;
            while(!mark[z])z=fa[z][0];
            if(z!=lca(x,y))
            {
                printf("WA lca t=%d x=%d y=%d got=%d want=%d\n",t,x,y,lca(x,y),z);
                return 0;
            }
            int dd=0,p=x;
            while(p!=z)dd++,p=fa[p][0];
            p=y;
            while(p!=z)dd++,p=fa[p][0];
            if(dd!=dist(x,y))
            {
                printf("WA dist t=%d %d %d\n",t,dd,dist(x,y));
                return 0;
            }
        }
    }
    printf("LCA/距离 与暴力对拍 200 组通过\n");
    // 自测 4：树上差分与暴力路径计数对拍
    for(int t=1;t<=100;t++)
    {
        n=rand()%20+2;
        for(int i=1;i<=n;i++)head[i]=0;
        num=0;
        for(int i=2;i<=n;i++)add_undirected(rand()%(i-1)+1,i);
        root=1;
        dfs(root,0);
        memset(diff,0,sizeof(diff));
        int cnt[25]={0};// cnt[u]：边 (u,fa[u]) 被覆盖次数
        int qq=rand()%10+1;
        for(int i=1;i<=qq;i++)
        {
            int x=rand()%n+1,y=rand()%n+1;
            add_path(x,y);
            int z=lca(x,y),p=x;
            while(p!=z)cnt[p]++,p=fa[p][0];
            p=y;
            while(p!=z)cnt[p]++,p=fa[p][0];
        }
        collect(root,0);
        for(int u=2;u<=n;u++)
            if(diff[u]!=cnt[u])
            {
                printf("WA diff t=%d u=%d got=%d want=%d\n",t,u,diff[u],cnt[u]);
                return 0;
            }
    }
    printf("树上差分与暴力对拍 100 组通过\n");
    return 0;
}
/* 数组 fa[N][20] 支持 n<2^20；LCA 前必须 dfs 一次建表，根节点 fa[root][0]=0 且 dep[0]=0 */
