// 递推与线性DP 的测试与对拍代码
// 模板本体：06-动态规划/递推与线性DP.cpp
#include "../../06-动态规划/递推与线性DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// O(n^2)，最长上升子序列（严格递增），f[i] 表示以 i 结尾的 LIS 长度
// 转移：f[i]=max(f[j])+1，其中 j<i 且 a[j]<a[i]
int lis_n2(int n,int a[])
{
    int ans=0;
    for(int i=1;i<=n;i++)
    {
        f[i]=1;
        for(int j=1;j<i;j++)
            if(a[j]<a[i])f[i]=max(f[i],f[j]+1);
        ans=max(ans,f[i]);
    }
    return ans;
}

// O(n log n)，LIS 贪心+二分：g[len] 存长度为 len 的上升子序列的最小结尾
int lis_nlogn(int n,int a[])
{
    int len=0;
    for(int i=1;i<=n;i++)
    {
        int p=lower_bound(g+1,g+len+1,a[i])-g;// 严格递增用 lower_bound，非严格用 upper_bound
        g[p]=a[i];
        len=max(len,p);
    }
    return len;
}

// O(n)，最大子段和：f[i] 表示以 i 结尾的最大子段和，f[i]=max(f[i-1],0)+a[i]
int max_subarray(int n,int a[])
{
    int ans=NEG;
    for(int i=1;i<=n;i++)
    {
        f[i]=max(f[i-1],0)+a[i];
        ans=max(ans,f[i]);
    }
    return ans;
}

// O(n^2)，数字三角形：从顶走到底的最大路径和，顺推
// 转移：dp[i][j]=max(dp[i-1][j-1],dp[i-1][j])+tri[i][j]
// 边界必须判掉：j=1 没有左上方，j=i 没有正上方（否则会把没算过的 0 当答案）
int triangle_max(int n,int tri[][15])
{
    for(int i=1;i<=n;i++)
        for(int j=1;j<=i;j++)
        {
            if(j==1)dp[i][j]=dp[i-1][j]+tri[i][j];
            else if(j==i)dp[i][j]=dp[i-1][j-1]+tri[i][j];
            else dp[i][j]=max(dp[i-1][j-1],dp[i-1][j])+tri[i][j];
        }
    int ans=NEG;
    for(int j=1;j<=n;j++)ans=max(ans,dp[n][j]);
    return ans;
}

// O(n^2)，网格路径数：只能往右/往下走，grid=1 是障碍
ll grid_paths(int n,int m,int grid[][15])
{
    ll dp2[15][15];
    for(int i=0;i<=n;i++)
        for(int j=0;j<=m;j++)dp2[i][j]=0;
    dp2[1][1]=(grid[1][1]==1)?0:1;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
        {
            if(i==1&&j==1)continue;
            if(grid[i][j]==1){dp2[i][j]=0;continue;}
            dp2[i][j]=dp2[i-1][j]+dp2[i][j-1];// 从上面或左面推过来
        }
    return dp2[n][m];
}

// O(n^2)，最长公共子序列长度
int lcs(char s[],char t[],int n,int m)
{
    for(int i=0;i<=n;i++)
        for(int j=0;j<=m;j++)
        {
            if(i==0||j==0){dp[i][j]=0;continue;}
            if(s[i]==t[j])dp[i][j]=dp[i-1][j-1]+1;
            else dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
        }
    return dp[n][m];
}

// O(n)，爬楼梯方案数（每次 1 或 2 级），f[i]=f[i-1]+f[i-2]
ll climb(int n)
{
    ll x=1,y=1;// f[0]=1,f[1]=1
    for(int i=2;i<=n;i++)
    {
        ll z=x+y;
        x=y,y=z;
    }
    return y;
}

// O(n)，数字解码方案数：'1'~'9' 单独成一位，'10'~'26' 两位成一位
ll decode_ways(char s[],int n)
{
    ll f0=1,f1=(n>=1&&s[1]!='0')?1:0;// f[i-2],f[i-1]
    if(n==0)return 0;
    if(n==1)return f1;
    for(int i=2;i<=n;i++)
    {
        ll cur=0;
        if(s[i]!='0')cur+=f1;
        int two=(s[i-1]-'0')*10+(s[i]-'0');
        if(two>=10&&two<=26)cur+=f0;
        f0=f1,f1=cur;
    }
    return f1;
}

// 暴力：数字三角形的所有路径
int brute_triangle(int i,int j,int n,int tri[][15])
{
    if(i==n)return tri[i][j];
    return tri[i][j]+max(brute_triangle(i+1,j,n,tri),brute_triangle(i+1,j+1,n,tri));
}

// 暴力：所有子段和取最大
int brute_subarray(int n,int a[])
{
    int ans=NEG;
    for(int i=1;i<=n;i++)
    {
        int s=0;
        for(int j=i;j<=n;j++)s+=a[j],ans=max(ans,s);
    }
    return ans;
}

// 暴力：枚举所有上升子序列（n 很小，2^n 枚举子集）
int brute_lis(int n,int a[])
{
    int ans=0;
    for(int mask=0;mask<(1<<n);mask++)
    {
        int last=NEG,cnt=0,ok=1;
        for(int i=1;i<=n;i++)
            if(mask>>(i-1)&1)
            {
                if(a[i]<=last){ok=0;break;}
                last=a[i],cnt++;
            }
        if(ok)ans=max(ans,cnt);
    }
    return ans;
}

// 暴力：网格路径数 dfs
ll brute_grid(int i,int j,int n,int m,int grid[][15])
{
    if(i<1||i>n||j<1||j>m||grid[i][j]==1)return 0;
    if(i==n&&j==m)return 1;
    return brute_grid(i+1,j,n,m,grid)+brute_grid(i,j+1,n,m,grid);
}

// 暴力：LCS 用记忆化递归单独写，避免和 dp 表混淆
int bdp[15][15];
int brute_lcs(int i,int j,char s[],char t[])
{
    if(i==0||j==0)return 0;
    if(bdp[i][j]>=0)return bdp[i][j];
    int ans;
    if(s[i]==t[j])ans=brute_lcs(i-1,j-1,s,t)+1;
    else ans=max(brute_lcs(i-1,j,s,t),brute_lcs(i,j-1,s,t));
    return bdp[i][j]=ans;
}

// 暴力：爬楼梯（枚举每步走 1 或 2）
ll brute_climb(int n)
{
    if(n==0)return 1;
    if(n<0)return 0;
    return brute_climb(n-1)+brute_climb(n-2);
}

// 暴力：数字解码方案数
ll brute_decode(int i,char s[],int n)
{
    if(i>n)return 1;
    ll ans=0;
    if(s[i]!='0')ans+=brute_decode(i+1,s,n);
    if(i+1<=n)
    {
        int two=(s[i]-'0')*10+(s[i+1]-'0');
        if(two>=10&&two<=26)ans+=brute_decode(i+2,s,n);
    }
    return ans;
}

int main()
{
    srand(20240611);
    printf("==== 固定样例 ====\n");
    int s1[9]={0,10,9,2,5,3,7,101,18};
    printf("LIS  n=8 [10 9 2 5 3 7 101 18] : %d %d (期望 4 4)\n",lis_n2(8,s1),lis_nlogn(8,s1));
    int s2[7]={0,-2,1,-3,4,-1,2};
    printf("最大子段和 [-2 1 -3 4 -1 2] : %d (期望 5，即子段 [4 -1 2])\n",max_subarray(6,s2));
    n=5;
    int t5[15][15]={{0},{0,7},{0,3,8},{0,8,1,0},{0,2,7,4,4},{0,4,5,2,6,5}};
    printf("数字三角形 n=5 : %d (期望 30)\n",triangle_max(n,t5));
    n=3,m=3;
    int gg[15][15]={{0},{0,0,0,0},{0,0,1,0},{0,0,0,0}};
    printf("网格 3x3 中间有障碍 : %lld (期望 2)\n",grid_paths(n,m,gg));
    strcpy(s+1,"abcde"),strcpy(t+1,"ace");
    printf("LCS abcde/ace : %d (期望 3)\n",lcs(s,t,5,3));
    strcpy(s+1,"226");
    printf("解码 226 : %lld (期望 3)\n",decode_ways(s,3));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,14);
        for(int i=1;i<=n;i++)a[i]=rndint(-20,20);
        if(lis_n2(n,a)!=brute_lis(n,a)){bad++;printf("WA! LIS n^2 轮%d\n",tt);break;}
        if(lis_nlogn(n,a)!=brute_lis(n,a)){bad++;printf("WA! LIS nlogn 轮%d\n",tt);break;}
        if(max_subarray(n,a)!=brute_subarray(n,a)){bad++;printf("WA! 最大子段和 轮%d\n",tt);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,8);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)tri[i][j]=rndint(-9,9);
        if(triangle_max(n,tri)!=brute_triangle(1,1,n,tri)){bad++;printf("WA! 数字三角形 轮%d\n",tt);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,6),m=rndint(1,6);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)grid[i][j]=(rndint(1,4)==1);
        ll cur=grid_paths(n,m,grid),ref=brute_grid(1,1,n,m,grid);
        if(cur!=ref){bad++;printf("WA! 网格路径 轮%d n=%d m=%d ref=%lld cur=%lld\n",tt,n,m,ref,cur);break;}
    }
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,10),m=rndint(1,10);
        for(int i=1;i<=n;i++)s[i]="abc"[rndint(0,2)];
        for(int j=1;j<=m;j++)t[j]="acd"[rndint(0,2)];
        for(int i=0;i<=n;i++)
            for(int j=0;j<=m;j++)bdp[i][j]=-1;
        int cur=lcs(s,t,n,m),ref=brute_lcs(n,m,s,t);
        if(cur!=ref){bad++;printf("WA! LCS 轮%d ref=%d cur=%d\n",tt,ref,cur);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(0,14);
        if(climb(n)!=brute_climb(n)){bad++;printf("WA! 爬楼梯 轮%d\n",tt);break;}
    }
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,10);
        for(int i=1;i<=n;i++)s[i]='0'+rndint(0,9);
        ll cur=decode_ways(s,n),ref=brute_decode(1,s,n);
        if(cur!=ref){bad++;printf("WA! 解码 轮%d s=%s ref=%lld cur=%lld\n",tt,s+1,ref,cur);break;}
    }
    if(!bad)printf("stress OK (LIS 300 + 最大子段和 300 + 三角形 200 + 网格 200 + LCS 300 + 爬楼梯 200 + 解码 300 全部通过)\n");
    return 0;
}

/*
数字三角形样例（IOI1994）：
5
7
3 8
8 1 0
2 7 4 4
4 5 2 6 5
答案 30，路径 7->3->8->7->5
*/
