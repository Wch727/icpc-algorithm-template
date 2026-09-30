// 欧拉路径与欧拉回路 的测试与对拍代码
// 模板本体：04-图论/欧拉路径与欧拉回路.cpp
#include "../../04-图论/欧拉路径与欧拉回路.cpp"

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
                // 不能跳过此前未使用的其他终点边
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

    // 自测 3：四边形加对角线只有两个奇度点，原期望 0 写错，应为路径
    m=5;
    init_undirected(4);
    add_uedge(1,2),add_uedge(2,3),add_uedge(3,4),add_uedge(4,1),add_uedge(1,3);
    printf("无向四边形加对角线：类型 %d（期望 1 路径）\n",euler_undirected(s));

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
        for(int i=1;i<=m;i++)add_uedge(eu[i],ev[i]),bedgeu[i]=eu[i],bedgev[i]=ev[i];
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
        for(int i=1;i<=m;i++)bedgeu[i]=deu[i],bedgev[i]=dev[i];
        for(int i=1;i<=m;i++)bdvis[i]=0;
        bdpc=0;
        int bdgot=0;
        for(int st=1;st<=n&&!bdgot;st++){bdpath[++bdpc]=st;if(bdfs_directed(st,0))bdgot=1;bdpc=0;}
        // 每点一条出边并不保证欧拉路，原自测的存在性假设写错
        if((dgot!=0)!=bdgot||(dgot&&!check_directed()))
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
