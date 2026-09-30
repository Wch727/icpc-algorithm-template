#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
int head[N],to[N<<1],nxt[N<<1],num=0;
int dfn[N],low[N];
int is_cut[N];// 是否为割点
int timer=0;
int ea[N],eb[N],cut_edge[N],ecnt=0;// 桥的列表

void add_edge(int u,int v)
{
    to[++num]=v,nxt[num]=head[u],head[u]=num;
}

void add_undirected(int u,int v)
{
    add_edge(u,v);
    add_edge(v,u);
}

void tarjan(int u,int fa)// 无向图求割点与桥，O(n+m)；fa 是边的入边编号，避免走回父亲
{
    dfn[u]=low[u]=++timer;
    int child=0;
    for(int i=head[u];i;i=nxt[i])
    {
        int v=to[i];
        if(!dfn[v])
        {
            child++;
            tarjan(v,i);
            low[u]=min(low[u],low[v]);
            if(low[v]>dfn[u])// 子树回不到 u 及更早 -> 这条边是桥
            {
                ecnt++;
                ea[ecnt]=u,eb[ecnt]=v;
                cut_edge[i]=cut_edge[i^1]=1;
            }
            if(fa&&low[v]>=dfn[u])is_cut[u]=1;// 非根结点有子树回不到上面 -> 割点
        }
        else if(i!=(fa^1))low[u]=min(low[u],dfn[v]);// 反向边走一次即可
    }
    if(!fa&&child>=2)is_cut[u]=1;// 根结点有两棵以上子树才是割点
}

int main()
{
    // 自测 1：手造图 1-2 2-3 3-1 3-4 4-5 5-6 6-4（两个环靠 3-4 相连）
    n=6;
    int uu[7]={0,1,2,3,3,4,5},vv[7]={0,2,3,1,4,5,6};
    for(int i=1;i<=6;i++)add_undirected(uu[i],vv[i]);
    for(int i=1;i<=n;i++)if(!dfn[i])tarjan(i,0);
    printf("割点：");
    for(int i=1;i<=n;i++)if(is_cut[i])printf("%d ",i);
    printf("\n期望 3 4 5（5 在 4-6 环上是割点）\n");
    printf("桥：");
    for(int i=1;i<=ecnt;i++)printf("(%d,%d) ",ea[i],eb[i]);
    printf("\n期望 (3,4)\n");
    // 自测 2：随机图，割点/桥与「删点(边)后连通块数变多」暴力对拍
    for(int t=1;t<=300;t++)
    {
        n=rand()%7+1;
        for(int i=1;i<=n;i++)head[i]=0,dfn[i]=0,low[i]=0,is_cut[i]=0;
        num=1,timer=0,ecnt=0;// 边编号从 2 开始，保证 i^1 成对
        memset(cut_edge,0,sizeof(cut_edge));
        int e1[70],e2[70],mm=0;
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
                if(rand()%2)
                {
                    add_undirected(i,j);
                    mm++;
                    e1[mm]=i,e2[mm]=j;
                }
        // 暴力连通块数（0 号点被删 / 某条边被删）
        int base=0;
        {
            int f[10];
            for(int i=1;i<=n;i++)f[i]=i;
            for(int i=1;i<=mm;i++)
            {
                int x=e1[i],y=e2[i];
                while(f[x]!=x)x=f[x];
                while(f[y]!=y)y=f[y];
                f[x]=y;
            }
            for(int i=1;i<=n;i++)if(f[i]==i)base++;
        }
        for(int i=1;i<=n;i++)if(!dfn[i])tarjan(i,0);
        // 检查割点
        for(int del=1;del<=n;del++)
        {
            int f[10],cnt=0;
            for(int i=1;i<=n;i++)f[i]=i;
            for(int i=1;i<=mm;i++)
            {
                if(e1[i]==del||e2[i]==del)continue;
                int x=e1[i],y=e2[i];
                while(f[x]!=x)x=f[x];
                while(f[y]!=y)y=f[y];
                f[x]=y;
            }
            for(int i=1;i<=n;i++)if(i!=del&&f[i]==i)cnt++;
            int want=(cnt>base);// 删掉这个点后连通块变多才是割点（叶子点不算）
            if(want!=is_cut[del])
            {
                printf("WA 割点 t=%d del=%d got=%d want=%d\n",t,del,is_cut[del],want);
                return 0;
            }
        }
        // 检查桥
        for(int de=1;de<=mm;de++)
        {
            int f[10],cnt=0;
            for(int i=1;i<=n;i++)f[i]=i;
            for(int i=1;i<=mm;i++)
            {
                if(i==de)continue;
                int x=e1[i],y=e2[i];
                while(f[x]!=x)x=f[x];
                while(f[y]!=y)y=f[y];
                f[x]=y;
            }
            for(int i=1;i<=n;i++)if(f[i]==i)cnt++;
            int want=(cnt>base);
            // 找到模板里对应的边编号
            int got=0;
            for(int i=1;i<=ecnt;i++)
                if((ea[i]==e1[de]&&eb[i]==e2[de])||(ea[i]==e2[de]&&eb[i]==e1[de]))got=1;
            if(want!=got)
            {
                printf("WA 桥 t=%d edge=(%d,%d) got=%d want=%d\n",t,e1[de],e2[de],got,want);
                return 0;
            }
        }
    }
    printf("割点/桥 与暴力删点删边对拍 300 组通过\n");
    return 0;
}
/* 边编号必须从 2 开始（i^1 配对）；重边时 low[v]>dfn[u] 判定依旧正确
   双连通分量：把一个连通块里所有桥删掉，剩下的连通块就是边双；割点划分的是点双 */
