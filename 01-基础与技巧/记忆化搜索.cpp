#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;

// 记忆化搜索：递归 + 把算过的状态存下来，本质就是自顶向下的 DP
// 三步：想清楚状态 -> 想清楚转移 -> 想清楚边界和返回条件
// 好处是只算用得到的状态（比递推省），坏处是递归深度大时爆栈、常数比递推大
ll memo[N][N];
int vis[N][N];
int a[N][N];
int n,m;

// ---- 例1：数字三角形（洛谷 P1216）----
// dp(i,j) = 从 (i,j) 走到最底层的最大路径和
// 转移：dp(i,j)=a[i][j]+max(dp(i+1,j),dp(i+1,j+1))
// 边界：i==n 时没有下一层，直接返回 a[i][j]
// 记忆化后每个状态只算一次，O(n^2)
ll dfs_triangle(int i,int j)
{
    if(i==n)return a[i][j];
    if(vis[i][j])return memo[i][j];
    vis[i][j]=1;
    return memo[i][j]=a[i][j]+max(dfs_triangle(i+1,j),dfs_triangle(i+1,j+1));
}

// 递推版（自底向上），用来和记忆化对拍
ll dp_triangle()
{
    static ll f[N][N];
    for(int j=1;j<=n;j++)f[n][j]=a[n][j];
    for(int i=n-1;i>=1;i--)
        for(int j=1;j<=i;j++)
            f[i][j]=a[i][j]+max(f[i+1][j],f[i+1][j+1]);
    return f[1][1];
}

// ---- 状态编码：把多维状态压成一个整数当数组下标 ----
// 例如状态 (i,j)，j 只有 m 种取值，就压成 i*m+j
// 好处是能开一维数组 + 直接 memset，缺点是下标要自己算，容易算错边界
int code(int i,int j)
{
    return i*m+j;
}

// 用编码数组重写数字三角形，验证编码写法与二维一致
ll bad[N*N];
int bvis[N*N];
ll dfs_triangle_code(int i,int j)
{
    if(i==n)return a[i][j];
    int id=code(i,j);
    if(bvis[id])return bad[id];
    bvis[id]=1;
    return bad[id]=a[i][j]+max(dfs_triangle_code(i+1,j),dfs_triangle_code(i+1,j+1));
}

// ---- 例2：滑雪（洛谷 P1434）----
// dp(i,j) = 从 (i,j) 出发能滑的最长长度，只能往严格更低的地方滑
// 这题没法直接递推（依赖关系是四方向的），记忆化搜索是标准写法
int h[N][N],f[N][N];
int dx[4]={0,0,1,-1},dy[4]={1,-1,0,0};

int dfs_ski(int x,int y)
{
    if(f[x][y])return f[x][y];// 算过就直接返回
    f[x][y]=1;// 至少能站在自己这一格
    for(int d=0;d<4;d++)
    {
        int nx=x+dx[d],ny=y+dy[d];
        if(nx<1||nx>n||ny<1||ny>m)continue;// 越界
        if(h[nx][ny]<h[x][y])f[x][y]=max(f[x][y],dfs_ski(nx,ny)+1);
    }
    return f[x][y];
}
