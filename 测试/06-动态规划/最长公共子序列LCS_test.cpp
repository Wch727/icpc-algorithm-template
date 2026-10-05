// 最长公共子序列LCS 的测试与对拍代码
// 模板本体：06-动态规划/最长公共子序列LCS.cpp
#include "../../06-动态规划/最长公共子序列LCS.cpp"


int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

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
    printf("LCS 二维=%d 滚动=%d 暴力=%d (期望 4)\n",lcs_nm(),lcs_roll(),brute_lcs(1,1));
    lcs_scheme();
    strcpy(a+1,"kitten"),n=strlen(a+1);
    strcpy(b+1,"sitting"),m=strlen(b+1);
    printf("编辑距离 二维=%d 滚动=%d 暴力=%d (期望 3)\n",edit_dist(),edit_dist_roll(),brute_edit(n,m));
    strcpy(a+1,"ababc"),n=strlen(a+1);
    strcpy(b+1,"babc"),m=strlen(b+1);
    printf("最长公共子串 滚动=%d 暴力=%d (期望 4，即 babc)\n",lcsubstr(),brute_lcsubstr(n,m,a,b));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=1000;tt++)
    {
        n=rndint(0,8),m=rndint(0,8);
        for(int i=1;i<=n;i++)a[i]='a'+rndint(0,2);
        for(int i=1;i<=m;i++)b[i]='a'+rndint(0,2);
        a[n+1]=b[m+1]='\0';
        int r0=brute_lcs(1,1);
        if(lcs_nm()!=r0){bad++;printf("WA! lcs_nm 轮%d\n",tt);break;}
        if(lcs_roll()!=r0){bad++;printf("WA! lcs_roll 轮%d\n",tt);break;}
        int r1=brute_edit(n,m);
        if(edit_dist()!=r1){bad++;printf("WA! edit_dist 轮%d\n",tt);break;}
        if(edit_dist_roll()!=r1){bad++;printf("WA! edit_dist_roll 轮%d\n",tt);break;}
        int r2=brute_lcsubstr(n,m,a,b);
        if(lcsubstr()!=r2){bad++;printf("WA! lcsubstr 轮%d\n",tt);break;}
    }
    if(!bad)printf("stress OK (1000 轮，LCS 二维/滚动、编辑距离二维/滚动、最长公共子串 全部通过)\n");
    return 0;
}
