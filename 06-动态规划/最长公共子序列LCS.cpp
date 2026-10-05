#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
int n,m;
char a[N],b[N];
int f[N];// 一维滚动数组
int g[N][N];// LCS 二维表，还原方案用
int st[N];// 还原方案用的栈

// O(nm)，最长公共子序列：g[i][j] 表示 a 前 i 个与 b 前 j 个的 LCS 长度
int lcs_nm()
{
    for(int i=0;i<=n;i++)g[i][0]=0;
    for(int j=0;j<=m;j++)g[0][j]=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            if(a[i]==b[j])g[i][j]=g[i-1][j-1]+1;// 这一位匹配，直接要
            else g[i][j]=max(g[i-1][j],g[i][j-1]);// 否则丢 a 的尾或 b 的尾
    return g[n][m];
}

// O(nm)，LCS 一维滚动：f[j] 是当前行的值，pre 存上一行左上角
int lcs_roll()
{
    for(int j=0;j<=m;j++)f[j]=0;
    for(int i=1;i<=n;i++)
    {
        int pre=0;// f[i-1][j-1]
        for(int j=1;j<=m;j++)
        {
            int tmpj=f[j];// 旧的 f[i-1][j]
            if(a[i]==b[j])f[j]=pre+1;
            else f[j]=max(f[j],f[j-1]);// f[j-1] 已是 f[i][j-1]
            pre=tmpj;
        }
    }
    return f[m];
}

// O(nm)，还原一组 LCS 并输出（必须先调用 lcs_nm 建好 g）
void lcs_scheme()
{
    int top=0,i=n,j=m;
    while(i>0&&j>0)
    {
        if(a[i]==b[j])st[++top]=a[i],i--,j--;
        else if(g[i-1][j]>=g[i][j-1])i--;
        else j--;
    }
    printf("长度 %d，一组方案：",g[n][m]);
    for(int k=top;k>=1;k--)putchar(st[k]);
    printf("\n");
}

// O(nm)，编辑距离：g[i][j] 表示 a 前 i 个变成 b 前 j 个的最少操作数
int edit_dist()
{
    for(int i=0;i<=n;i++)g[i][0]=i;// 全删
    for(int j=0;j<=m;j++)g[0][j]=j;// 全插
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            if(a[i]==b[j])g[i][j]=g[i-1][j-1];
            else g[i][j]=min(min(g[i-1][j],g[i][j-1]),g[i-1][j-1])+1;// 删 插 换
    return g[n][m];
}

// O(nm)，编辑距离一维滚动：f[j-1] 是 f[i][j-1]，pre 是 f[i-1][j-1]
int edit_dist_roll()
{
    for(int j=0;j<=m;j++)f[j]=j;// 第 0 行
    for(int i=1;i<=n;i++)
    {
        int pre=f[0];// f[i-1][0]
        f[0]=i;// f[i][0]=i
        for(int j=1;j<=m;j++)
        {
            int tmpj=f[j];// 旧的 f[i-1][j]
            if(a[i]==b[j])f[j]=pre;
            else f[j]=min(min(f[j],f[j-1]),pre)+1;
            pre=tmpj;
        }
    }
    return f[m];
}

// O(nm)，最长公共子串（必须连续）：不匹配就直接清 0
int lcsubstr()
{
    int ans=0;
    for(int j=0;j<=m;j++)f[j]=0;
    for(int i=1;i<=n;i++)
    {
        int pre=0;
        for(int j=1;j<=m;j++)
        {
            int tmpj=f[j];
            if(a[i]==b[j])f[j]=pre+1;
            else f[j]=0;
            pre=tmpj;
            ans=max(ans,f[j]);
        }
    }
    return ans;
}
