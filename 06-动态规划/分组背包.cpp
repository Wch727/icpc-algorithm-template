#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=105;// 组数上限
const int K=1005;// 每组物品数上限
const int VMAX=20005;// 容量上限
const int KN=3;// 每个主件最多 2 个附件
int n,V,g;
int cnt[N],gw[N][K],gv[N][K];// 每组物品数 与 组内物品
int f[VMAX];// 一维滚动数组，f[j] 表示容量 j 的最大价值
int mw[N],mv[N],ac[N];// 主件重量 价值 附件数
int aw[N][KN],av[N][KN];// 附件重量 价值，1-indexed

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
