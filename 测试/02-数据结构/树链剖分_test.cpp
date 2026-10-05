// 树链剖分 的测试与对拍代码
// 模板本体：02-数据结构/树链剖分.cpp
#include "../../02-数据结构/树链剖分.cpp"

// ---------- 以下为自测用的暴力 ----------
int bval[N];
int bsum_path(int u,int v)              // 暴力路径和
{
    int res=0;
    while(dep[u]>dep[v])res+=bval[u],u=fa[u];
    while(dep[v]>dep[u])res+=bval[v],v=fa[v];
    while(u!=v)res+=bval[u]+bval[v],u=fa[u],v=fa[v];
    return res+bval[u];
}
void bpath_add(int u,int v,int k)
{
    while(dep[u]>dep[v])bval[u]+=k,u=fa[u];
    while(dep[v]>dep[u])bval[v]+=k,v=fa[v];
    while(u!=v)bval[u]+=k,bval[v]+=k,u=fa[u],v=fa[v];
    bval[u]+=k;
}
void bsub_add(int u,int k)              // 暴力子树
{
    for(int i=1;i<=n;i++)
    {
        int x=i;
        while(x)
        {
            if(x==u){bval[i]+=k;break;}
            x=fa[x];
        }
    }
}
int bsub_sum(int u)
{
    int res=0;
    for(int i=1;i<=n;i++)
    {
        int x=i;
        while(x)
        {
            if(x==u){res+=bval[i];break;}
            x=fa[x];
        }
    }
    return res;
}

int main()
{
    srand(20240513);

    // 1. 手测：链 1-2-3-4-5，点权 1..5
    n=5,rt=1,cnt=0;
    for(int i=1;i<=n;i++)adj[i].clear(),a[i]=i;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    dfs1_iter(rt),dfs2(rt,rt);
    for(int i=1;i<=n;i++)val[i]=a[i],bval[i]=a[i];
    seg.build(1,n,1);
    printf("path_sum(2,5)=%lld subtree_sum(3)=%lld point(1)=%lld\n",path_sum(2,5),subtree_sum(3),subtree_sum(1)-subtree_sum(2));
    path_add(2,5,1);
    printf("after path+1: path_sum(2,5)=%lld path_sum(2,2)=%lld subtree_sum(3)=%lld\n",path_sum(2,5),path_sum(2,2),subtree_sum(3));
    subtree_add(3,10);
    printf("after sub+10: subtree_sum(3)=%lld path_sum(1,5)=%lld\n",subtree_sum(3),path_sum(1,5));

    // 2. 随机对拍：真随机树（父亲从 [1,i-1] 里取）+ 随机操作
    bool ok=true;
    for(int T=1;T<=20&&ok;T++)
    {
        n=rand()%14+2,rt=rand()%n+1,cnt=0;
        for(int i=1;i<=n;i++)adj[i].clear();
        for(int i=2;i<=n;i++)
        {
            int f=rand()%(i-1)+1;
            add_edge(i,f);
        }
        dfs1_iter(rt),dfs2(rt,rt);
        for(int i=1;i<=n;i++)a[i]=rand()%21-10,bval[i]=a[i],val[i]=a[i];
        seg.build(1,n,1);
        m=80;
        for(int i=1;i<=m;i++)
        {
            op=rand()%4;
            int u=rand()%n+1,v=rand()%n+1,k=rand()%11-5;
            if(op==0)path_add(u,v,k),bpath_add(u,v,k);
            else if(op==1)
            {
                if(path_sum(u,v)!=bsum_path(u,v)){ok=false;break;}
            }
            else if(op==2)subtree_add(u,k),bsub_add(u,k);
            else
            {
                if(subtree_sum(u)!=bsub_sum(u)){ok=false;break;}
            }
        }
        for(int u=1;u<=n&&ok;u++)           // 收尾全量比对
        {
            if(subtree_sum(u)!=bsub_sum(u))ok=false;
            for(int v=1;v<=n&&ok;v++)
            {
                int uu=u,vv=v;
                if(path_sum(uu,vv)!=bsum_path(uu,vv))ok=false;
            }
        }
        printf("random round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 链状极限数据：100000 个点，路径和 / 子树和不能爆栈超时
    n=100000,rt=1,cnt=0;
    for(int i=1;i<=n;i++)adj[i].clear(),a[i]=1;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    dfs1_iter(rt),dfs2(rt,rt);
    for(int i=1;i<=n;i++)val[i]=a[i];
    seg.build(1,n,1);
    printf("big chain: path_sum(1,100000)=%lld subtree_sum(50000)=%lld\n",path_sum(1,n),subtree_sum(50000));
    path_add(1,n,1);
    printf("after +1: path_sum(1,100000)=%lld subtree_sum(50000)=%lld\n",path_sum(1,n),subtree_sum(50000));
    return 0;
}
