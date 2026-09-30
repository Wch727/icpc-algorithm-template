// 记忆化搜索 的测试与对拍代码
// 模板本体：01-基础与技巧/记忆化搜索.cpp
#include "../../01-基础与技巧/记忆化搜索.cpp"

// 滑雪的暴力 DFS（不记忆化），小数据对拍用

int brute_ski(int x,int y)
{
    int res=1;
    for(int d=0;d<4;d++)
    {
        int nx=x+dx[d],ny=y+dy[d];
        if(nx<1||nx>n||ny<1||ny>m)continue;
        if(h[nx][ny]<h[x][y])res=max(res,brute_ski(nx,ny)+1);
    }
    return res;
}

// ---- 剪枝：可行性剪枝 + 最优性剪枝 ----
// 以「数字三角形里是否存在一条和 >= 目标 S 的路径」为例
// 可行性剪枝：剩下的格子全取最大值都不够 -> 直接砍
// 两个坑：
//   1) maxbest 必须从当前三角形内部算，i>n 的位置要是极小值，
//      否则会读到上一组数据残留的格子，剪掉真正的解
//   2) 递归前必须先挡越界（i>n）！否则会算 sum+INT_MIN，溢出成一个大正数（UB），
//      叶子被误判成"还能更优"，一直递归过底，答案就错了
int best[N][N];
ll S;

int maxbest(int i,int j)
{
    if(i>n)return INT_MIN;// 越界位，表示「这条边不存在」
    if(best[i][j]!=INT_MIN)return best[i][j];
    if(i==n)return best[i][j]=a[i][j];
    int down=maxbest(i+1,j),diag=maxbest(i+1,j+1);
    return best[i][j]=a[i][j]+max(down,diag);
}

bool dfs_cut(int i,int j,ll sum)
{
    if(i>n)return false;// 越界（左右边界都算），必须先挡掉，否则下面会 sum+INT_MIN 溢出
    if(i==n)return sum+a[i][j]>=S;// 叶子要把自己算进去再比
    if(sum+a[i][j]+maxbest(i+1,j)<S&&sum+a[i][j]+maxbest(i+1,j+1)<S)return false;// 两个儿子的乐观估计都不够
    if(dfs_cut(i+1,j,sum+a[i][j]))return true;
    if(dfs_cut(i+1,j+1,sum+a[i][j]))return true;
    return false;
}

// 无剪枝的暴力，用于验证剪枝没砍掉正解
// 注意 sum 的含义要和 dfs_cut 完全一致：只累计「当前位置之前」的格子
// 这里必须把叶子自己也算进去再比较，否则两边判的不是同一件事
bool brute_cut(int i,int j,ll sum)
{
    if(i>n)return false;
    if(i==n)return sum+a[i][j]>=S;
    if(brute_cut(i+1,j,sum+a[i][j]))return true;
    if(brute_cut(i+1,j+1,sum+a[i][j]))return true;
    return false;
}

int main()
{
    srand(20240601);
    // 自测1：数字三角形 记忆化 vs 递推 vs 暴力枚举所有路径
    for(int t=1;t<=300;t++)
    {
        n=rand()%8+1;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)a[i][j]=rand()%21-10;// 含负数，边界要试
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)vis[i][j]=0;
        ll got=dfs_triangle(1,1);
        ll want=dp_triangle();
        if(got!=want)
        {
            printf("fail triangle memo n=%d got=%lld want=%lld\n",n,got,want);
            return 0;
        }
    }
    printf("triangle memo vs dp self-check OK\n");

    // 自测2：状态编码写法必须和二维写法结果相同
    for(int t=1;t<=300;t++)
    {
        n=rand()%8+1,m=n+1;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)a[i][j]=rand()%21-10;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)vis[i][j]=0;
        memset(bvis,0,sizeof(bvis));
        ll g1=dfs_triangle(1,1);
        ll g2=dfs_triangle_code(1,1);
        if(g1!=g2)
        {
            printf("fail code state n=%d got=%lld want=%lld\n",n,g1,g2);
            return 0;
        }
    }
    printf("state encoding self-check OK\n");

    // 自测3：滑雪 记忆化 vs 暴力 DFS
    for(int t=1;t<=300;t++)
    {
        n=rand()%5+1,m=rand()%5+1;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)h[i][j]=rand()%8;
        memset(f,0,sizeof(f));
        int got=0,want=0;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)got=max(got,dfs_ski(i,j));
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)want=max(want,brute_ski(i,j));
        if(got!=want)
        {
            printf("fail ski got=%d want=%d\n",got,want);
            return 0;
        }
    }
    printf("ski memo self-check OK\n");

    // 自测4：剪枝版与无剪枝暴力结果必须一致（剪枝不能砍掉正解）
    for(int t=1;t<=500;t++)
    {
        n=rand()%7+1;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)a[i][j]=rand()%11-5;
        // best 是全局数组，上一轮残留会污染这一轮，必须先清干净
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)best[i][j]=INT_MIN;
        S=rand()%41-20;
        int g1=(int)dfs_cut(1,1,0),g2=(int)brute_cut(1,1,0);
        if(g1!=g2)
        {
            printf("fail cut t=%d n=%d S=%lld got=%d want=%d\n",t,n,S,g1,g2);
            return 0;
        }
    }
    printf("pruning self-check OK\n");

    // 自测5：套题演示——P1216 样例
    // maxbest 依赖全局 n，算完要立刻存下来，后面改 n 就不能再调它了
    n=5;
    int tri[6][6]={{0},{0,7},{0,3,8},{0,8,1,0},{0,2,7,4,4},{0,4,5,2,6,5}};
    for(int i=1;i<=n;i++)
        for(int j=1;j<=i;j++)a[i][j]=tri[i][j],vis[i][j]=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=i;j++)best[i][j]=INT_MIN;
    int demo_best=maxbest(1,1);
    printf("P1216 样例 ans=%lld (want 30) 最大路径和=%d\n",dfs_triangle(1,1),demo_best);

    // 自测6：P1434 迷你数据（1..9 排成 3x3）
    // 注意只能上下左右走，走不了斜线，所以最长是 1-4-7 这类一条直线 = 5，不是 9
    n=3,m=3;
    int hill[4][4]={{0,0,0,0},{0,1,2,3},{0,4,5,6},{0,7,8,9}};
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)h[i][j]=hill[i][j];
    memset(f,0,sizeof(f));
    int ans=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)ans=max(ans,dfs_ski(i,j));
    printf("P1434 迷你 1..9 方格最长=%d (want 5，只能四方向走)\n",ans);

    // 自测7：记忆化的效率对比——滑雪在 100x100 随机高度上的耗时
    n=100,m=100;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)h[i][j]=rand()%10000;
    memset(f,0,sizeof(f));
    clock_t st=clock();
    int mx=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)mx=max(mx,dfs_ski(i,j));
    printf("ski 100x100 memo ans=%d time=%.3fs\n",mx,(double)(clock()-st)/CLOCKS_PER_SEC);
    return 0;
}
