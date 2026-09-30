// 树的重心、直径、树上最远点
// 直径两种写法：两遍 bfs / 树形 DP；求法都是 O(n)
// 重心：删掉它之后最大连通块最小（重心最多两个，这里求编号最小的那个）
// 最远点：任意点 x 的最远点一定是直径的某个端点，O(1) 查询
// 递归写法给 n<=3e4 用；n=1e5 的链请用下面的迭代版（显式栈），否则可能爆栈
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n;
int head[N],to[N<<1],nxt[N<<1],num=0;
int dis[N],que[N],order[N],fa[N],sz[N];// que 是 bfs 队列，order 是 bfs 序
int da,db,dia;// 直径两端点与长度

// 带权图把 to[] 换成 to[]+w[]，dis 累加边权即可
void add_edge(int u,int v)
{
    to[++num]=v,nxt[num]=head[u],head[u]=num;
    to[++num]=u,nxt[num]=head[v],head[v]=num;
}

// 从 s 出发 bfs，返回最远点的编号；dis 顺手算好，O(n)，迭代不爆栈
int bfs(int s)
{
    for(int i=1;i<=n;i++)dis[i]=-1;
    int hd=0,tl=0,far=s;
    que[tl++]=s,dis[s]=0;
    while(hd<tl)
    {
        int u=que[hd++];
        if(dis[u]>dis[far])far=u;
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(dis[v]==-1)dis[v]=dis[u]+1,que[tl++]=v;
        }
    }
    return far;
}

// 两遍 bfs：第一次找端点，第二次定长度，O(n)
int get_diameter()
{
    da=bfs(1);
    db=bfs(da);
    dia=dis[db];
    return dia;
}

// 树形 DP 求直径：dp1/dp2 记向下最长、次长，O(n)，迭代不爆栈
int get_diameter_dp()
{
    int hd=0,tl=0;
    for(int i=1;i<=n;i++)fa[i]=0;
    que[tl++]=1,fa[1]=-1,dis[1]=0;
    while(hd<tl)
    {
        int u=que[hd++];
        order[hd]=u;// order[1..tl] 就是 bfs 序
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(v==fa[u])continue;
            fa[v]=u,dis[v]=dis[u]+1,que[tl++]=v;
        }
    }
    int dp1[N],dp2[N],ans=0;
    for(int i=tl;i>=1;i--)// 逆序保证儿子先算完
    {
        int u=order[i];
        dp1[u]=dp2[u]=0;
        for(int j=head[u];j;j=nxt[j])
        {
            int v=to[j];
            if(v==fa[u])continue;
            int d=dp1[v]+1;
            if(d>dp1[u])dp2[u]=dp1[u],dp1[u]=d;
            else if(d>dp2[u])dp2[u]=d;
        }
        ans=max(ans,dp1[u]+dp2[u]);
    }
    return ans;
}

// 求重心：最大子树最小的点，O(n)，迭代不爆栈
int get_centroid()
{
    int hd=0,tl=0;
    for(int i=1;i<=n;i++)fa[i]=0;
    que[tl++]=1,fa[1]=-1;
    while(hd<tl)
    {
        int u=que[hd++];
        order[hd]=u;
        for(int i=head[u];i;i=nxt[i])
        {
            int v=to[i];
            if(v==fa[u])continue;
            fa[v]=u,que[tl++]=v;
        }
    }
    for(int i=tl;i>=1;i--)
    {
        int u=order[i];
        sz[u]=1;
        for(int j=head[u];j;j=nxt[j])
            if(to[j]!=fa[u])sz[u]+=sz[to[j]];
    }
    int best=1,mx=n+1;
    for(int u=1;u<=n;u++)
    {
        int m=n-sz[u];// 父亲那一边的块
        for(int j=head[u];j;j=nxt[j])
            if(to[j]!=fa[u])m=max(m,sz[to[j]]);
        if(m<mx)mx=m,best=u;
    }
    return best;
}

// 树上最远点：x 的最远点必是 da 或 db 之一，O(1)（dis 由 bfs(da) 得到）
int far_node(int x)
{
    return dis[x]>dis[db]-dis[x]?da:db;
}

// 朴素递归版求直径（n 小的时候用，链会爆栈）
int dfs_dia(int u,int f,int &res)
{
    int a=0;// 往下的最长链，次长的不单独存
    for(int i=head[u];i;i=nxt[i])
    {
        int v=to[i];
        if(v==f)continue;
        int d=dfs_dia(v,u,res)+1;
        res=max(res,a+d),a=max(a,d);
    }
    return a;
}

int main()
{
    srand(20240514);

    // 自测 1：手造链 1-2-3-4-5，直径 4、重心 3、最远点
    n=5,num=0;
    for(int i=1;i<=n;i++)head[i]=0;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    printf("链 n=5：直径 %d（期望 4），重心 %d（期望 3）\n",get_diameter(),get_centroid());
    int c=get_centroid();
    bfs(c);
    printf("重心 %d 到最远点 %d 的距离 %d（期望 2）\n",c,bfs(da),dia);

    // 自测 2：手造星形 1 为心，直径 2、重心 1
    n=6,num=0;
    for(int i=1;i<=n;i++)head[i]=0;
    for(int i=2;i<=n;i++)add_edge(1,i);
    printf("星形 n=6：直径 %d（期望 2），重心 %d（期望 1）\n",get_diameter(),get_centroid());

    // 自测 3：三条叉的树 1-2,2-3,3-4,2-5,5-6，直径 4，重心 2 或 3（取小编号 2）
    n=6,num=0;
    for(int i=1;i<=n;i++)head[i]=0;
    add_edge(1,2),add_edge(2,3),add_edge(3,4),add_edge(2,5),add_edge(5,6);
    printf("叉树 n=6：直径 %d（期望 4），重心 %d（期望 2）\n",get_diameter(),get_centroid());

    // 自测 4：随机树对拍，直径（两遍 bfs / DP / 递归）与重心全部和暴力比
    bool ok=true;
    int round=0;
    for(int T=1;T<=300;T++)
    {
        n=rand()%12+2,num=0;
        for(int i=1;i<=n;i++)head[i]=0;
        for(int i=2;i<=n;i++)add_edge(rand()%(i-1)+1,i);
        // 暴力：矩阵 bfs 求所有点对距离
        int bd[15][15];
        for(int s=1;s<=n;s++)
        {
            for(int i=1;i<=n;i++)bd[s][i]=-1;
            int q2[15],h2=0,t2=0;
            q2[t2++]=s,bd[s][s]=0;
            while(h2<t2)
            {
                int u=q2[h2++];
                for(int i=head[u];i;i=nxt[i])
                    if(bd[s][to[i]]==-1)bd[s][to[i]]=bd[s][u]+1,q2[t2++]=to[i];
            }
        }
        int bdia=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)bdia=max(bdia,bd[i][j]);
        int d1=get_diameter(),d2=get_diameter_dp();
        int d3=0;
        dfs_dia(1,0,d3);
        if(d1!=bdia||d2!=bdia||d3!=bdia)
        {
            printf("FAILED 直径 t=%d n=%d bfs=%d dp=%d 递归=%d 暴力=%d\n",T,n,d1,d2,d3,bdia);
            ok=false;
            break;
        }
        if(dis[da]!=bdia||dis[db]!=0)
        {
            printf("FAILED 直径端点 t=%d\n",T);
            ok=false;
            break;
        }
        // 暴力重心：删掉该点后最大连通块最小
        int bm2=n+1,bc2=-1;
        for(int u=1;u<=n;u++)
        {
            int mark[15]={0};
            mark[u]=1;
            int m=0;
            for(int s=1;s<=n;s++)
            {
                if(mark[s])continue;
                int cnt=0,q2[15],h2=0,t2=0;
                mark[s]=1,q2[t2++]=s;
                while(h2<t2)
                {
                    int x=q2[h2++];
                    cnt++;
                    for(int i=head[x];i;i=nxt[i])
                        if(!mark[to[i]])mark[to[i]]=1,q2[t2++]=to[i];
                }
                m=max(m,cnt);
            }
            if(m<bm2)bm2=m,bc2=u;
        }
        int c1=get_centroid();
        if(c1!=bc2)
        {
            printf("FAILED 重心 t=%d n=%d got=%d want=%d\n",T,n,c1,bc2);
            ok=false;
            break;
        }
        // 最远点：x 的最远距离必须等于两端点距离的较大值
        bfs(da);
        for(int x=1;x<=n;x++)
        {
            int best=0;
            for(int y=1;y<=n;y++)best=max(best,bd[x][y]);
            int got=max(dis[x],dia-dis[x]);
            if(got!=best)
            {
                printf("FAILED 最远点 t=%d x=%d got=%d want=%d\n",T,x,got,best);
                ok=false;
                break;
            }
        }
        if(!ok)break;
        if(++round%6==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("随机树直径/重心/最远点对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 5：n=1e5 的链（深度 1e5），迭代版不能爆栈
    n=100000,num=0;
    for(int i=1;i<=n;i++)head[i]=0;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    printf("大链 n=100000：直径 %d（期望 99999），DP 直径 %d（期望 99999），重心 %d（期望 50000 或 50001）\n",
           get_diameter(),get_diameter_dp(),get_centroid());
    return 0;
}
/* 关键点：dis 只有最近一次 bfs 的结果；要查最远点先 bfs(da) 一次。
   重心若有 2 个，本模板返回编号小的那个；直径长度按边数算（边权 >1 请累加边权）。 */
