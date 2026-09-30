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
int deg[N],used[N<<1],it[N],stk[N];
// 有向图：出边的链式前向星 + 入度
int dhead[N],dto[N],dnxt[N],decnt,din[N],dout[N],dvis[N];
int path_[N],pcnt;

void init_undirected(int n_)
{
    n=n_,ecnt=0;
    for(int i=1;i<=n;i++)head[i]=0,deg[i]=0;
    for(int i=1;i<=2*m+2;i++)used[i]=0;
}

void add_uedge(int u,int v)// 无向边，一次加一对
{
    to[++ecnt]=v,nxt[ecnt]=head[u],head[u]=ecnt;
    to[++ecnt]=u,nxt[ecnt]=head[v],head[v]=ecnt;
    deg[u]++,deg[v]++;
}

void init_directed(int n_)
{
    n=n_,decnt=0;
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
    int c1=0,c2=0,cnt_start=0;
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

// 校验：走出来的序列相邻两点之间必须有一条没用过的边
int check_undirected()
{
    if(pcnt!=m+1)return 0;
    int visq[N<<1]={0},tmphead[N];
    for(int i=1;i<=n;i++)tmphead[i]=head[i];
    for(int i=1;i+1<=pcnt;i++)
    {
        int u=path_[i],v=path_[i+1],found=0;
        for(int e=tmphead[u];e;e=nxt[e])
            if(!visq[e]&&to[e]==v)// 顺着用过的边往下挪
            {
                visq[e]=visq[e^1]=1,found=1;
                tmphead[u]=nxt[e];
                break;
            }
        if(!found)return 0;
    }
    return 1;
}

int check_directed()
{
    if(pcnt!=m+1)return 0;
    int visq[N]={0};
    for(int i=1;i+1<=pcnt;i++)
    {
        int u=path_[i],v=path_[i+1],found=0;
        for(int e=dhead[u];e;e=dnxt[e])
            if(!visq[e]&&dto[e]==v){visq[e]=1,found=1;break;}
        if(!found)return 0;
    }
    return 1;
}

// 暴力：dfs 搜一条用完全部边的路（只在小数据下用）
int bedgeu[25],bedgev[25],bvis[25],bpc,bpath[25];
int bdfs_undirected(int u,int cnt)
{
    if(cnt==m)return 1;
    for(int i=1;i<=m;i++)
    {
        if(bvis[i])continue;
        int v=-1;
        if(bedgeu[i]==u)v=bedgev[i];
        else if(bedgev[i]==u)v=bedgeu[i];
        if(v<0)continue;
        bvis[i]=1,bpath[++bpc]=v;
        if(bdfs_undirected(v,cnt+1))return 1;
        bvis[i]=0,bpc--;
    }
    return 0;
}

int bdvis[25],bdpath[25],bdpc;
int bdfs_directed(int u,int cnt)
{
    if(cnt==m)return 1;
    for(int i=1;i<=m;i++)
    {
        if(bdvis[i]||bedgeu[i]!=u)continue;
        bdvis[i]=1,bdpath[++bdpc]=bedgev[i];
        if(bdfs_directed(bedgev[i],cnt+1))return 1;
        bdvis[i]=0,bdpc--;
    }
    return 0;
}

int main()
{
    srand(20240517);

    // 自测 1：无向 1-2-3-1 三角形，欧拉回路
    m=3;
    init_undirected(3);
    add_uedge(1,2),add_uedge(2,3),add_uedge(3,1);
    int s=0;
    int t1=euler_undirected(s);
    printf("无向三角形：类型 %d（期望 2 回路）路径长度 %d（期望 4）校验 %d\n",t1,pcnt,check_undirected());
    for(int i=1;i<=pcnt;i++)printf("%d%c",path_[i],i==pcnt?'\n':' ');

    // 自测 2：无向 1-2-3-4 一条链，欧拉路径
    m=3;
    init_undirected(4);
    add_uedge(1,2),add_uedge(2,3),add_uedge(3,4);
    int t2=euler_undirected(s);
    printf("无向链 1-2-3-4：类型 %d（期望 1 路径）起点 %d 路径 %d->%d 校验 %d\n",
           t2,s,path_[1],path_[pcnt],check_undirected());

    // 自测 3：无向 1-2-3-4-1 + 1-3，四个奇度点，不存在
    m=5;
    init_undirected(4);
    add_uedge(1,2),add_uedge(2,3),add_uedge(3,4),add_uedge(4,1),add_uedge(1,3);
    printf("无向四奇度点：类型 %d（期望 0 不存在）\n",euler_undirected(s));

    // 自测 4：有向 1->2->3->1，欧拉回路
    m=3;
    init_directed(3);
    add_dedge(1,2),add_dedge(2,3),add_dedge(3,1);
    int t4=euler_directed(s);
    printf("有向三元环：类型 %d（期望 2 回路）校验 %d\n",t4,check_directed());

    // 自测 5：有向 1->2->3，欧拉路径起点 1
    m=2;
    init_directed(3);
    add_dedge(1,2),add_dedge(2,3);
    int t5=euler_directed(s);
    printf("有向链 1->2->3：类型 %d（期望 1 路径）起点 %d（期望 1）校验 %d\n",t5,s,check_directed());

    // 自测 6：有向 1->2,1->3 出度不等，不存在
    m=2;
    init_directed(3);
    add_dedge(1,2),add_dedge(1,3);
    printf("有向出度不等：类型 %d（期望 0 不存在）\n",euler_directed(s));

    // 自测 7：随机图对拍（保证存在解：无向先造树，有向每点至少一条出边）
    bool ok=true;
    int round=0;
    for(int T=1;T<=150;T++)
    {
        // --- 无向：先造一棵树保证连通，余下随机加边，度数全是偶数就必定有回路
        n=rand()%6+2;
        int eu[15],ev[15],ec=0,degtmp[10]={0};
        for(int i=2;i<=n;i++)
        {
            int f=rand()%(i-1)+1;
            eu[++ec]=f,ev[ec]=i,degtmp[f]++,degtmp[i]++;
        }
        // 随机加边直到所有偶度不满意也没关系，直接交给模板判存在性，再与暴力比
        int extra=rand()%3;
        for(int i=1;i<=extra;i++)
        {
            int a=rand()%n+1,b=rand()%n+1;
            if(a==b)continue;
            eu[++ec]=a,ev[ec]=b;
        }
        m=ec;
        init_undirected(n);
        for(int i=1;i<=m;i++)add_uedge(eu[i],ev[i]);
        int got=euler_undirected(s);
        int gotexist=(got!=0),gotcheck=(got!=0?check_undirected():0);
        // 暴力
        for(int i=1;i<=m;i++)bvis[i]=0;
        bpc=0;
        int bexist=0;
        for(int st=1;st<=n&&!bexist;st++){bpath[++bpc]=st;if(bdfs_undirected(st,0))bexist=1;bpc=0;}
        if(gotexist!=bexist||(gotexist&&!gotcheck))
        {
            printf("FAILED 无向 t=%d n=%d m=%d got=%d 校验=%d 暴力=%d\n",T,n,m,gotexist,gotcheck,bexist);
            ok=false;
            break;
        }
        if(gotexist)
            for(int i=1;i<=m;i++)bedgeu[i]=eu[i],bedgev[i]=ev[i];
        // --- 有向：给每个点造一条出边（指向任意点），保证一定有欧拉路径
        n=rand()%6+2;
        int deu[15],dev[15],dc=0;
        for(int i=1;i<=n;i++)
        {
            int j=rand()%n+1;
            if(j==i)j=i%n+1;
            deu[++dc]=i,dev[dc]=j;
        }
        m=dc;
        init_directed(n);
        for(int i=1;i<=m;i++)add_dedge(deu[i],dev[i]);
        int dgot=euler_directed(s);
        if(dgot==0||!check_directed())
        {
            printf("FAILED 有向 t=%d n=%d m=%d got=%d 校验=%d\n",T,n,m,dgot,dgot?check_directed():0);
            ok=false;
            break;
        }
        for(int i=1;i<=m;i++)bedgeu[i]=deu[i],bedgev[i]=dev[i];
        for(int i=1;i<=m;i++)bdvis[i]=0;
        bdpc=0;
        int bdgot=0;
        for(int st=1;st<=n&&!bdgot;st++){bdpath[++bdpc]=st;if(bdfs_directed(st,0))bdgot=1;bdpc=0;}
        if(!bdgot)
        {
            printf("FAILED 有向暴力 t=%d\n",T);
            ok=false;
            break;
        }
        if(++round%5==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("欧拉路存在性/合法性对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 8：n=1e5 的大回路/大路径，显式栈不爆栈
    n=100000,m=n;
    init_undirected(n);
    for(int i=1;i<=n;i++)add_uedge(i,i%n+1);
    int bt=euler_undirected(s);
    printf("大环 n=100000：%s 序列长度 %d（期望 100001）\n",bt==2?"回路":"非回路",pcnt);
    n=100000,m=n-1;
    init_undirected(n);
    for(int i=1;i<n;i++)add_uedge(i,i+1);
    int bt2=euler_undirected(s);
    printf("大链 n=100000：%s 起点 %d 终点 %d\n",bt2==1?"路径":"非路径",path_[1],path_[pcnt]);
    return 0;
}
/* 坑点：
   1) 无向边必须成对加，(e^1) 是反向边，used 数组要开 2*m+2。
   2) 孤立点不影响判存在性，但连通性判断必须只统计有边的点。
   3) 字典序最小的欧拉路：把邻接表按终点排序，Hierholzer 仍然成立（会得到最小字典序）。 */
