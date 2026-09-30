// 分组背包 的测试与对拍代码
// 模板本体：06-动态规划/分组背包.cpp
#include "../../06-动态规划/分组背包.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// O(V*总物品数)，分组背包：每组至多选一个
// 必须先枚举容量(倒序)、再枚举组内物品，否则同组会选多个
int group_knap(int g,int V)
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int k=1;k<=g;k++)
        for(int j=V;j>=0;j--)
            for(int i=1;i<=cnt[k];i++)
                if(j>=gw[k][i])
                    f[j]=max(f[j],f[j-gw[k][i]]+gv[k][i]);
    return f[V];
}

// O(V*4n)，依赖背包：主件+最多 2 个附件，把每个主件的合法方案当成一组
// 想买附件必须先买主件，所以组内方案只有 4 种
int depend_knap(int n,int V)
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int i=1;i<=n;i++)
    {
        int cw[5],cv[5],tot=0;// 该主件的所有合法方案
        tot++,cw[tot]=mw[i],cv[tot]=mv[i];// 只买主件
        if(ac[i]>=1)tot++,cw[tot]=mw[i]+aw[i][1],cv[tot]=mv[i]+av[i][1];
        if(ac[i]>=2)
        {
            tot++,cw[tot]=mw[i]+aw[i][2],cv[tot]=mv[i]+av[i][2];
            if(mw[i]+aw[i][1]+aw[i][2]<=V)
                tot++,cw[tot]=mw[i]+aw[i][1]+aw[i][2],cv[tot]=mv[i]+av[i][1]+av[i][2];
        }
        for(int j=V;j>=0;j--)// 倒序，一个主件只取一种方案
            for(int t=1;t<=tot;t++)
                if(j>=cw[t])f[j]=max(f[j],f[j-cw[t]]+cv[t]);
    }
    return f[V];
}

// 暴力：从第 k 组起，剩 rem 容量，每组选 0 个或 1 个
int brute_group(int k,int rem)
{
    if(k>g)return 0;
    int best=brute_group(k+1,rem);
    for(int i=1;i<=cnt[k];i++)
        if(gw[k][i]<=rem)best=max(best,brute_group(k+1,rem-gw[k][i])+gv[k][i]);
    return best;
}

// 暴力：第 i 个主件起，剩 rem 容量
int brute_depend(int i,int rem)
{
    if(i>n)return 0;
    int best=brute_depend(i+1,rem);
    if(mw[i]<=rem)best=max(best,brute_depend(i+1,rem-mw[i])+mv[i]);
    if(ac[i]>=1&&mw[i]+aw[i][1]<=rem)
        best=max(best,brute_depend(i+1,rem-mw[i]-aw[i][1])+mv[i]+av[i][1]);
    if(ac[i]>=2)
    {
        if(mw[i]+aw[i][2]<=rem)
            best=max(best,brute_depend(i+1,rem-mw[i]-aw[i][2])+mv[i]+av[i][2]);
        if(mw[i]+aw[i][1]+aw[i][2]<=rem)
            best=max(best,brute_depend(i+1,rem-mw[i]-aw[i][1]-aw[i][2])+mv[i]+av[i][1]+av[i][2]);
    }
    return best;
}

int main()
{
    srand(20240602);
    printf("==== 固定样例 ====\n");
    g=3,V=10;
    cnt[1]=2,gw[1][1]=2,gv[1][1]=3,gw[1][2]=3,gv[1][2]=4;
    cnt[2]=2,gw[2][1]=4,gv[2][1]=5,gw[2][2]=5,gv[2][2]=6;
    cnt[3]=2,gw[3][1]=3,gv[3][1]=4,gw[3][2]=6,gv[3][2]=11;
    printf("分组背包 V=10 : %d (期望 16)  暴力 %d\n",group_knap(g,V),brute_group(1,V));
    n=2,V=10;
    mw[1]=5,mv[1]=6,ac[1]=2,aw[1][1]=2,av[1][1]=3,aw[1][2]=3,av[1][2]=4;
    mw[2]=4,mv[2]=5,ac[2]=0;
    printf("依赖背包 V=10 : %d (期望 13)  暴力 %d\n",depend_knap(n,V),brute_depend(1,V));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0,ref,cur;
    for(tt=1;tt<=600;tt++)
    {
        g=rndint(1,4),V=rndint(0,25);
        for(int k=1;k<=g;k++)
        {
            cnt[k]=rndint(1,3);
            for(int i=1;i<=cnt[k];i++)gw[k][i]=rndint(1,8),gv[k][i]=rndint(1,12);
        }
        ref=brute_group(1,V),cur=group_knap(g,V);
        if(ref!=cur){bad++;printf("WA! 分组 轮%d ref=%d cur=%d\n",tt,ref,cur);break;}
        n=rndint(1,4);
        for(int i=1;i<=n;i++)
        {
            mw[i]=rndint(1,8),mv[i]=rndint(1,12),ac[i]=rndint(0,2);
            for(int j=1;j<=ac[i];j++)aw[i][j]=rndint(1,6),av[i][j]=rndint(1,8);
        }
        ref=brute_depend(1,V),cur=depend_knap(n,V);
        if(ref!=cur){bad++;printf("WA! 依赖 轮%d ref=%d cur=%d\n",tt,ref,cur);break;}
    }
    if(!bad)printf("stress OK (600 轮，分组/依赖 全部通过)\n");
    return 0;
}
