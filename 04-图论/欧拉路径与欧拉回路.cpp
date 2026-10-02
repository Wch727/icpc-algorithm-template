// 欧拉路径 / 欧拉回路（Hierholzer 算法，用显式栈写，n=1e5 不爆栈）
// 无向图：所有点度数为偶 → 欧拉回路；恰有 0/2 个奇度点 → 有欧拉路径
// 有向图：所有点入度=出度 → 欧拉回路；恰有 1 个出度比入度大 1（起点）、
//         1 个入度比出度大 1（终点）、其余相等 → 有欧拉路径
// 前置条件：忽略孤立点后图连通
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
// 无向图：链式前向星，边成对存，(e^1) 是反向边
int head[N],to[N<<1],nxt[N<<1],ecnt;
int deg[N],used[N<<1],it[N],stk[N+5];
// 有向图：出边的链式前向星 + 入度
int dhead[N],dto[N],dnxt[N],decnt,din[N],dout[N],dvis[N];
int path_[N+5],pcnt;

void init_undirected(int n_)
{
    n=n_,ecnt=1,pcnt=0;
    for(int i=1;i<=n;i++)head[i]=0,deg[i]=0;
    memset(used,0,sizeof(used));
}

void add_uedge(int u,int v)// 无向边，一次加一对
{
    to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    to[++ecnt]=u,nxt[ecnt]=head[v],head[v]=ecnt;
    deg[u]++,deg[v]++;
}

void init_directed(int n_)
{
    n=n_,decnt=0,pcnt=0;
    for(int i=1;i<=n;i++)dhead[i]=0,din[i]=0,dout[i]=0;
}

void add_dedge(int u,int v)// 有向边
{
    dto[++decnt]=v,dnxt[decnt]=dhead[u],dhead[u]=decnt;
    dvis[decnt]=0;
    dout[u]++,din[v]++;
}

// 忽略孤立点后是否连通（无向）
int connected_undirected()
{
    int s=0;
    for(int i=1;i<=n;i++)if(deg[i]){s=i;break;}
    if(!s)return 1;// 没有边
    int vis2[N]={0},q[N],hd=0,tl=0,cnt=0,all=0;
    for(int i=1;i<=n;i++)if(deg[i])all++;
    vis2[s]=1,q[tl++]=s;
    while(hd<tl)
    {
        int u=q[hd++];
        cnt++;
        for(int i=head[u];i;i=nxt[i])
            if(!vis2[to[i]])vis2[to[i]]=1,q[tl++]=to[i];
    }
    return cnt==all;
}

// 有向图弱连通（按无向边判连通，需要另建一份邻接，这里偷懒用并查集）
int ufa[N];
int findd(int x){return ufa[x]==x?x:ufa[x]=findd(ufa[x]);}
int connected_directed()
{
    for(int i=1;i<=n;i++)ufa[i]=i;
    int all=0;
    for(int i=1;i<=n;i++)if(din[i]||dout[i])all++;
    for(int u=1;u<=n;u++)
        for(int e=dhead[u];e;e=dnxt[e])
            ufa[findd(u)]=findd(dto[e]);
    if(!all)return 1;
    int r=-1;
    for(int i=1;i<=n;i++)if(din[i]||dout[i]){r=findd(i);break;}
    for(int i=1;i<=n;i++)if((din[i]||dout[i])&&findd(i)!=r)return 0;
    return 1;
}

// Hierholzer：无向图。返回 0 表示不存在，1 表示路径，2 表示回路；路径存在 path_[1..pcnt] 里
int euler_undirected(int &s)
{
    pcnt=0;
    int odd=0;
    for(int i=1;i<=n;i++)
        if(deg[i]&1)odd++,s=i;
    if(odd!=0&&odd!=2)return 0;
    if(odd==2)
    {
        int c=0;
        for(int i=1;i<=n;i++)if(deg[i]&1){c++;if(c==1)s=i;}
    }
    else
    {
        s=0;
        for(int i=1;i<=n;i++)if(deg[i]){s=i;break;}
    }
    if(!s)return 2;
    if(!connected_undirected())return 0;
    for(int i=1;i<=n;i++)it[i]=head[i];
    int tp=0;
    stk[++tp]=s,pcnt=0;
    while(tp)
    {
        int u=stk[tp];
        int e=it[u];
        while(e&&used[e])e=nxt[e];// 跳过已经走过的边
        it[u]=e;
        if(e)
        {
            used[e]=used[e^1]=1;
            stk[++tp]=to[e];
        }
        else
        {
            path_[++pcnt]=u;// 出栈顺序就是欧拉路的逆序
            tp--;
        }
    }
    reverse(path_+1,path_+pcnt+1);
    if(pcnt!=m+1)return 0;// 边没走完说明图不连通
    return odd==2?1:2;
}

// Hierholzer：有向图。返回 0/1/2 同上
int euler_directed(int &s)
{
    pcnt=0;
    int c1=0,c2=0;
    s=0;
    for(int i=1;i<=n;i++)
    {
        if(dout[i]-din[i]==1)c1++,s=i;
        else if(din[i]-dout[i]==1)c2++;
        else if(din[i]!=dout[i])return 0;
    }
    if(!((c1==1&&c2==1)||(c1==0&&c2==0)))return 0;
    if(c1==0)
    {
        for(int i=1;i<=n;i++)if(dout[i]){s=i;break;}
    }
    if(!s)return 2;
    if(!connected_directed())return 0;
    for(int i=1;i<=n;i++)it[i]=dhead[i];
    int tp=0;
    stk[++tp]=s,pcnt=0;
    while(tp)
    {
        int u=stk[tp];
        int e=it[u];
        while(e&&dvis[e])e=dnxt[e];
        it[u]=e;
        if(e)
        {
            dvis[e]=1;
            stk[++tp]=dto[e];
        }
        else
        {
            path_[++pcnt]=u;
            tp--;
        }
    }
    reverse(path_+1,path_+pcnt+1);
    if(pcnt!=m+1)return 0;
    return c1==1?1:2;
}
