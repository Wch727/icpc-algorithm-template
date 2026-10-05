// 递推与线性DP 的测试与对拍代码
// 模板本体：06-动态规划/递推与线性DP.cpp
#include "../../06-动态规划/递推与线性DP.cpp"

void load_array(int len,const int *aa)
{::n=len;for(int i=1;i<=len;i++)::a[i]=aa[i];}
void load_triangle(int len,const int tt[][15])
{::n=len;for(int i=1;i<=len;i++)for(int j=1;j<=i;j++)::tri[i][j]=tt[i][j];}
void load_grid(int rows,int cols,const int gg[][15])
{::n=rows;::m=cols;for(int i=1;i<=rows;i++)for(int j=1;j<=cols;j++)::grid[i][j]=gg[i][j];}

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

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
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        int v[]={0,INT_MIN,INT_MAX}; assert((load_array(2,v),lis_nlogn())==2);
        int one[]={0,INT_MIN};assert((load_array(1,one),max_subarray())==INT_MIN);
    }

    srand(20240611);
    printf("==== 固定样例 ====\n");
    int s1[9]={0,10,9,2,5,3,7,101,18};
    printf("LIS  n=8 [10 9 2 5 3 7 101 18] : %d %d (期望 4 4)\n",(load_array(8,s1),lis_n2()),(load_array(8,s1),lis_nlogn()));
    int s2[7]={0,-2,1,-3,4,-1,2};
    printf("最大子段和 [-2 1 -3 4 -1 2] : %d (期望 5，即子段 [4 -1 2])\n",(load_array(6,s2),max_subarray()));
    n=5;
    int t5[15][15]={{0},{0,7},{0,3,8},{0,8,1,0},{0,2,7,4,4},{0,4,5,2,6,5}};
    printf("数字三角形 n=5 : %d (期望 30)\n",(load_triangle(n,t5),triangle_max()));
    n=3,m=3;
    int gg[15][15]={{0},{0,0,0,0},{0,0,1,0},{0,0,0,0}};
    printf("网格 3x3 中间有障碍 : %lld (期望 2)\n",(load_grid(n,m,gg),grid_paths()));
    strcpy(s+1,"abcde"),strcpy(t+1,"ace");
    printf("LCS abcde/ace : %d (期望 3)\n",(::n=5,::m=3,lcs()));
    strcpy(s+1,"226");
    printf("解码 226 : %lld (期望 3)\n",(::n=3,decode_ways()));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,14);
        for(int i=1;i<=n;i++)a[i]=rndint(-20,20);
        if((load_array(n,a),lis_n2())!=brute_lis(n,a)){bad++;printf("WA! LIS n^2 轮%d\n",tt);break;}
        if((load_array(n,a),lis_nlogn())!=brute_lis(n,a)){bad++;printf("WA! LIS nlogn 轮%d\n",tt);break;}
        if((load_array(n,a),max_subarray())!=brute_subarray(n,a)){bad++;printf("WA! 最大子段和 轮%d\n",tt);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,8);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=i;j++)tri[i][j]=rndint(-9,9);
        if((load_triangle(n,tri),triangle_max())!=brute_triangle(1,1,n,tri)){bad++;printf("WA! 数字三角形 轮%d\n",tt);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,6),m=rndint(1,6);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)grid[i][j]=(rndint(1,4)==1);
        ll cur=(load_grid(n,m,grid),grid_paths()),ref=brute_grid(1,1,n,m,grid);
        if(cur!=ref){bad++;printf("WA! 网格路径 轮%d n=%d m=%d ref=%lld cur=%lld\n",tt,n,m,ref,cur);break;}
    }
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,10),m=rndint(1,10);
        for(int i=1;i<=n;i++)s[i]="abc"[rndint(0,2)];
        for(int j=1;j<=m;j++)t[j]="acd"[rndint(0,2)];
        for(int i=0;i<=n;i++)
            for(int j=0;j<=m;j++)bdp[i][j]=-1;
        int cur=(::n=n,::m=m,lcs()),ref=brute_lcs(n,m,s,t);
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
        ll cur=(::n=n,decode_ways()),ref=brute_decode(1,s,n);
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
