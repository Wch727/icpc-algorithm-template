// 树形DP 的测试与对拍代码
// 模板本体：06-动态规划/树形DP.cpp
#include "../../06-动态规划/树形DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// O(n)，没有上司的舞会
// dp[x][0] 表示 x 不参加时子树的最大欢乐值(儿子可选可不选)
// dp[x][1] 表示 x 参加时的最大值(儿子都不能参加)
void dfs_party(int x)
{
    dp[x][0]=0,dp[x][1]=h[x];
    for(int i=0;i<(int)son[x].size();i++)
    {
        int y=son[x][i];
        dfs_party(y);
        dp[x][0]+=max(dp[y][0],dp[y][1]);
        dp[x][1]+=dp[y][0];
    }
}

// O(n*m^2)，树上背包(选课)：f[x][j] 表示以 x 为根的子树里选 j 门课的最大收益
// 先把所有儿子合并上来，最后再把自己塞进去，体现"选子必须先选父"
// 分数非负时 f 直接 0 初始化，含义是"至多 j 门"；分数可能为负就改成 NEG 初始化
void dfs_knap(int x)
{
    for(int t=0;t<=m;t++)f[x][t]=0;
    for(int i=0;i<(int)son[x].size();i++)
    {
        int y=son[x][i];
        dfs_knap(y);
        for(int t=m;t>=0;t--)// 倒序，每个儿子只贡献一次
            for(int j=t;j>=0;j--)
                f[x][t]=max(f[x][t],f[x][t-j]+f[y][j]);
    }
    if(x!=0)// 虚拟根 0 没有学分
        for(int t=m;t>0;t--)f[x][t]=f[x][t-1]+score[x];
}

// O(n)，树形 dp 求直径（边数）：d1[x] 是最长向下链，d2[x] 是次长
void dfs_dia(int x)
{
    d1[x]=0,d2[x]=0;
    for(int i=0;i<(int)son[x].size();i++)
    {
        int y=son[x][i];
        dfs_dia(y);
        int t=d1[y]+1;
        if(t>d1[x])d2[x]=d1[x],d1[x]=t;
        else if(t>d2[x])d2[x]=t;
    }
    dia=max(dia,d1[x]+d2[x]);// 拐点在自己身上的最长路
}

// 暴力：枚举所有子集，父亲和儿子不能同时选
int brute_party(int n)
{
    int best=0;
    for(int mask=0;mask<(1<<n);mask++)
    {
        int ok=1;
        for(int i=1;i<=n;i++)
            if(((mask>>(i-1))&1)&&par[i]&&((mask>>(par[i]-1))&1))ok=0;
        if(!ok)continue;
        int sum=0;
        for(int i=1;i<=n;i++)
            if((mask>>(i-1))&1)sum+=h[i];
        best=max(best,sum);
    }
    return best;
}

// 暴力：枚举所有子集，选了课必须先选先修课，且数量不超过 m
int brute_knap(int n,int m)
{
    int best=0;
    for(int mask=0;mask<(1<<n);mask++)
    {
        int cnt=0,ok=1,sum=0;
        for(int i=1;i<=n;i++)
            if((mask>>(i-1))&1)
            {
                cnt++,sum+=score[i];
                if(par[i]&&((mask>>(par[i]-1))&1)==0)ok=0;
            }
        if(ok&&cnt<=m)best=max(best,sum);
    }
    return best;
}

// 暴力：从每个点 BFS 求最远点
int brute_dia(int n)
{
    int ans=0;
    for(int s=1;s<=n;s++)
    {
        int dis[N];
        queue<int> q;
        memset(dis,-1,sizeof(dis));
        dis[s]=0,q.push(s);
        while(!q.empty())
        {
            int x=q.front();
            q.pop();
            ans=max(ans,dis[x]);
            for(int i=0;i<(int)adj[x].size();i++)
                if(dis[adj[x][i]]<0)dis[adj[x][i]]=dis[x]+1,q.push(adj[x][i]);
        }
    }
    return ans;
}

int main()
{
    srand(20240607);
    printf("==== 固定样例 ====\n");
    n=7;
    for(int i=1;i<=n;i++)h[i]=1,son[i].clear(),vis[i]=0,par[i]=0;
    int e[7][2]={{1,3},{2,3},{6,4},{7,4},{4,5},{3,5},{0,0}};
    for(int i=0;i<6;i++)
    {
        int l=e[i][0],k=e[i][1];
        par[l]=k,vis[l]=1,son[k].push_back(l);
    }
    root=0;
    for(int i=1;i<=n;i++)
        if(!vis[i]){root=i;break;}
    dfs_party(root);
    printf("没有上司的舞会 : %d (期望 5)\n",max(dp[root][0],dp[root][1]));
    n=7,m=4;
    for(int i=0;i<=n;i++)son[i].clear(),par[i]=0;
    int q[7][2]={{2,2},{0,1},{0,4},{2,1},{7,1},{7,6},{2,2}};// {先修课, 学分}
    for(int i=1;i<=n;i++)
    {
        par[i]=q[i-1][0],score[i]=q[i-1][1];
        son[par[i]].push_back(i);
    }
    dfs_knap(0);
    printf("选课 n=7 m=4 : %d (期望 13)\n",f[0][m]);
    n=5;
    for(int i=1;i<=n;i++)son[i].clear();
    for(int i=2;i<=n;i++)son[i-1].push_back(i);
    dia=0,dfs_dia(1);
    printf("链 1-2-3-4-5 的直径 : %d (期望 4)\n",dia);

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,9);
        for(int i=0;i<=n;i++)son[i].clear(),adj[i].clear(),par[i]=0,vis[i]=0;
        for(int i=1;i<=n;i++)h[i]=rndint(0,10),score[i]=rndint(0,10);
        for(int i=2;i<=n;i++)// 父亲编号小于自己，1 一定是根
        {
            par[i]=rndint(1,i-1);
            son[par[i]].push_back(i);
            vis[i]=1;
            adj[i].push_back(par[i]),adj[par[i]].push_back(i);
        }
        dfs_party(1);
        int ref=brute_party(n),cur=max(dp[1][0],dp[1][1]);
        if(ref!=cur){bad++;printf("WA! 舞会 轮%d n=%d ref=%d cur=%d\n",tt,n,ref,cur);break;}
        m=rndint(0,n);
        son[0].clear(),son[0].push_back(1);// 虚拟根，处理森林
        dfs_knap(0);
        ref=brute_knap(n,m),cur=f[0][m];
        if(ref!=cur){bad++;printf("WA! 选课 轮%d n=%d m=%d ref=%d cur=%d\n",tt,n,m,ref,cur);break;}
        dia=0,dfs_dia(1);
        ref=brute_dia(n);
        if(ref!=dia){bad++;printf("WA! 直径 轮%d n=%d ref=%d cur=%d\n",tt,n,ref,dia);break;}
    }
    if(!bad)printf("stress OK (300 轮，舞会/选课/直径 全部通过)\n");
    return 0;
}
