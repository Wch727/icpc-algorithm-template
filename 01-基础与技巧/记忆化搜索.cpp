#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 滑雪：从 (x,y) 出发只能走向严格更低的相邻格子，求最长路径。
// 状态：dfs(x,y) 为从该格出发的最长路径长度，包含当前格；转移取下一格答案 +1。
// 缓存下标须包含影响后续结果的全部信息；剩余次数、已选集合等若有影响也要入状态。
// f[x][y]=0 表示未计算；每个状态算一次，O(nm)，最坏递归深度 O(nm)。
// 本题答案至少为 1；答案可能为 0/负数时，用独立 vis 标记或不会与答案重合的哨兵。
// 高度严格下降保证无环；存在循环依赖时，缓存或访问标记本身不能解决状态转移。
const int N=1005;
int n,m,h[N][N],f[N][N];
int dx[4]={0,0,1,-1},dy[4]={1,-1,0,0};

int dfs(int x,int y)
{
    if(f[x][y])return f[x][y];
    f[x][y]=1;
    for(int i=0;i<4;i++)
    {
        int nx=x+dx[i],ny=y+dy[i];
        if(nx<1||nx>n||ny<1||ny>m||h[nx][ny]>=h[x][y])continue;
        f[x][y]=max(f[x][y],dfs(nx,ny)+1);
    }
    return f[x][y];
}
int solve()
{
    memset(f,0,sizeof f);
    int ans=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)ans=max(ans,dfs(i,j));
    return ans;
}
// 多测或修改输入后须清缓存；长链可能爆栈，可按高度从低到高排序后迭代转移。
