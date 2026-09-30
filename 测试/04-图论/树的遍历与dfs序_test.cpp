// 树的遍历与dfs序 的测试与对拍代码
// 模板本体：04-图论/树的遍历与dfs序.cpp
#include "../../04-图论/树的遍历与dfs序.cpp"

int main()
{
    srand(20240515);

    // 自测 1：手造树 1-2 1-3 2-4 2-5 3-6，检查 dfs 序与子树区间
    n=6,ecnt=0,root=1;
    for(int i=1;i<=n;i++)head[i]=0,val[i]=i;
    add_edge(1,2),add_edge(1,3),add_edge(2,4),add_edge(2,5),add_edge(3,6);
    get_dfn(root);
    printf("树 1-2,1-3,2-4,2-5,3-6，dfn：");
    for(int u=1;u<=n;u++)printf("%d:%d ",u,in[u]);
    printf("\n子树区间：");
    for(int u=1;u<=n;u++)printf("[%d,%d] ",in[u],out[u]);
    printf("\n欧拉序长度 %d（期望 11 = 2n-1），lca(4,5)=%d（期望 2）lca(4,6)=%d（期望 1）\n",
           2*n-1,lca(4,5),lca(4,6));

    // 自测 2：子树转区间 + 树状数组
    for(int u=1;u<=n;u++)bit.add(in[u],val[u]);
    printf("子树和：");
    for(int u=1;u<=n;u++)printf("%d:%lld ",u,bit.range(in[u],out[u]));
    printf("（期望 21 11 9 4 5 6，分别对应子树 {1..6} {2,4,5} {3,6} {4} {5} {6}）\n");

    // 自测 3：随机树对拍，检查时间戳、子树区间、子树大小、LCA、子树和
    bool ok=true;
    int round=0;
    for(int T=1;T<=200;T++)
    {
        n=rand()%12+2,ecnt=0,root=1;
        for(int u=1;u<=n;u++)head[u]=0,val[u]=rand()%20+1,bit.tr[u]=0;
        for(int i=2;i<=n;i++)add_edge(rand()%(i-1)+1,i);
        get_dfn(root);
        int bpar[15]={0},q2[15],h2=0,t2=0;
        q2[t2++]=1;
        while(h2<t2)
        {
            int u=q2[h2++];
            for(int i=head[u];i;i=nxt[i])
                if(to[i]!=bpar[u])bpar[to[i]]=u,q2[t2++]=to[i];
        }
        // 时间戳合法 且 区间长度 == 子树大小
        for(int u=1;u<=n;u++)
        {
            if(in[u]<1||in[u]>n||out[u]<in[u]||out[u]>n){ok=false;break;}
            if(out[u]-in[u]+1!=sz[u]){ok=false;break;}
            if(rnk[in[u]]!=u){ok=false;break;}
        }
        // 区间里的点恰好是子树
        for(int u=1;u<=n&&ok;u++)
        {
            for(int x=1;x<=n;x++)
            {
                int p=x,insub=0;
                while(p)
                {
                    if(p==u){insub=1;break;}
                    p=bpar[p];
                }
                int inrange=in[u]<=in[x]&&in[x]<=out[u];
                if(insub!=inrange){ok=false;break;}
            }
        }
        // LCA 与暴力比
        for(int x=1;x<=n&&ok;x++)
            for(int y=1;y<=n;y++)
            {
                int depx=0,depy=0,p=x;
                while(p)depx++,p=bpar[p];
                p=y;
                while(p)depy++,p=bpar[p];
                int a=x,b=y;
                while(depx>depy)a=bpar[a],depx--;
                while(depy>depx)b=bpar[b],depy--;
                while(a!=b)a=bpar[a],b=bpar[b];
                if(lca(x,y)!=a){ok=false;break;}
            }
        if(!ok){printf("FAILED 结构 t=%d\n",T);break;}
        // 子树和与暴力比
        for(int u=1;u<=n;u++)bit.add(in[u],val[u]);// 按 dfs 序建树状数组
        for(int u=1;u<=n;u++)
        {
            ll s=0;
            for(int x=1;x<=n;x++)
            {
                int p=x;
                while(p)
                {
                    if(p==u){s+=val[x];break;}
                    p=bpar[p];
                }
            }
            if(bit.range(in[u],out[u])!=s){ok=false;break;}
        }
        if(!ok){printf("FAILED 子树和 t=%d\n",T);break;}
        if(++round%5==0)printf("随机对拍 round %d passed\n",round);
    }
    printf("dfs 序/欧拉序/子树区间对拍 %d 轮 %s\n",round,ok?"OK":"FAILED");

    // 自测 4：链 n=1e5，迭代 dfs 不爆栈；LCA 正确
    n=100000,ecnt=0,root=1;
    for(int i=1;i<=n;i++)head[i]=0,val[i]=1;
    for(int i=1;i<n;i++)add_edge(i,i+1);
    get_dfn(root);
    printf("大链 n=100000：in[1]=%d out[1]=%d（期望 1 100000）sz[1]=%d lca(1,100000)=%d lca(50000,99999)=%d\n",
           in[1],out[1],sz[1],lca(1,n),lca(50000,99999));
    return 0;
}
/* 注意：in/out 是 [1,n] 的闭区间，子树求和直接 bit.range(in[u],out[u])；
   欧拉序用「每步都记」的版本（长度 2n-1），另一种 2n 版本是进/出各记一次。 */
