#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
int n,m;
char a[N],b[N];
int f[N];// 一维滚动数组
int g[N][N];// LCS 二维表，还原方案用
int st[N];// 还原方案用的栈

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// O(nm)，最长公共子序列：g[i][j] 表示 a 前 i 个与 b 前 j 个的 LCS 长度
int lcs_nm(int n,int m,char a[],char b[])
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
int lcs_roll(int n,int m,char a[],char b[])
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
void lcs_scheme(int n,int m,char a[],char b[])
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
int edit_dist(int n,int m,char a[],char b[])
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
int edit_dist_roll(int n,int m,char a[],char b[])
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
int lcsubstr(int n,int m,char a[],char b[])
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

// 暴力：LCS 递归搜索
int brute_lcs(int i,int j)
{
    if(i>n||j>m)return 0;
    if(a[i]==b[j])return brute_lcs(i+1,j+1)+1;
    return max(brute_lcs(i+1,j),brute_lcs(i,j+1));
}

// 暴力：编辑距离递归
int brute_edit(int i,int j)
{
    if(i==0)return j;
    if(j==0)return i;
    if(a[i]==b[j])return brute_edit(i-1,j-1);
    return min(min(brute_edit(i-1,j),brute_edit(i,j-1)),brute_edit(i-1,j-1))+1;
}

// 暴力：最长公共子串，枚举 a 的所有子串
int brute_lcsubstr(int n,int m,char a[],char b[])
{
    int ans=0;
    for(int i=1;i<=n;i++)
        for(int len=1;i+len-1<=n;len++)
        {
            int ok=0;
            for(int j=1;j+len-1<=m&&!ok;j++)
            {
                int same=1;
                for(int k=0;k<len;k++)
                    if(a[i+k]!=b[j+k])same=0;
                if(same)ok=1;
            }
            if(ok)ans=max(ans,len);
        }
    return ans;
}

int main()
{
    srand(20240604);
    printf("==== 固定样例 ====\n");
    strcpy(a+1,"abcbdab"),n=strlen(a+1);
    strcpy(b+1,"bdcaba"),m=strlen(b+1);
    printf("LCS 二维=%d 滚动=%d 暴力=%d (期望 4)\n",lcs_nm(n,m,a,b),lcs_roll(n,m,a,b),brute_lcs(1,1));
    lcs_scheme(n,m,a,b);
    strcpy(a+1,"kitten"),n=strlen(a+1);
    strcpy(b+1,"sitting"),m=strlen(b+1);
    printf("编辑距离 二维=%d 滚动=%d 暴力=%d (期望 3)\n",edit_dist(n,m,a,b),edit_dist_roll(n,m,a,b),brute_edit(n,m));
    strcpy(a+1,"ababc"),n=strlen(a+1);
    strcpy(b+1,"babc"),m=strlen(b+1);
    printf("最长公共子串 滚动=%d 暴力=%d (期望 4，即 babc)\n",lcsubstr(n,m,a,b),brute_lcsubstr(n,m,a,b));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=1000;tt++)
    {
        n=rndint(0,8),m=rndint(0,8);
        for(int i=1;i<=n;i++)a[i]='a'+rndint(0,2);
        for(int i=1;i<=m;i++)b[i]='a'+rndint(0,2);
        a[n+1]=b[m+1]='\0';
        int r0=brute_lcs(1,1);
        if(lcs_nm(n,m,a,b)!=r0){bad++;printf("WA! lcs_nm 轮%d\n",tt);break;}
        if(lcs_roll(n,m,a,b)!=r0){bad++;printf("WA! lcs_roll 轮%d\n",tt);break;}
        int r1=brute_edit(n,m);
        if(edit_dist(n,m,a,b)!=r1){bad++;printf("WA! edit_dist 轮%d\n",tt);break;}
        if(edit_dist_roll(n,m,a,b)!=r1){bad++;printf("WA! edit_dist_roll 轮%d\n",tt);break;}
        int r2=brute_lcsubstr(n,m,a,b);
        if(lcsubstr(n,m,a,b)!=r2){bad++;printf("WA! lcsubstr 轮%d\n",tt);break;}
    }
    if(!bad)printf("stress OK (1000 轮，LCS 二维/滚动、编辑距离二维/滚动、最长公共子串 全部通过)\n");
    return 0;
}
