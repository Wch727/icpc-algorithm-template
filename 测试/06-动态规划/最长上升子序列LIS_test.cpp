// 最长上升子序列LIS 的测试与对拍代码
// 模板本体：06-动态规划/最长上升子序列LIS.cpp
#include "../../06-动态规划/最长上升子序列LIS.cpp"

int lcsf[15][15];

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// 暴力：枚举所有子序列，type 0 严格上升 1 不降 2 严格下降
int brute_lis(int n,int a[],int type)
{
    int ans=0;
    for(int mask=0;mask<(1<<n);mask++)
    {
        int last=0,cnt=0,ok=1;
        for(int i=1;i<=n;i++)
            if((mask>>(i-1))&1)
            {
                if(last)
                {
                    if(type==0&&!(a[last]<a[i]))ok=0;
                    if(type==1&&!(a[last]<=a[i]))ok=0;
                    if(type==2&&!(a[last]>a[i]))ok=0;
                }
                if(!ok)break;
                last=i,cnt++;
            }
        if(ok)ans=max(ans,cnt);
    }
    return ans;
}

// O(n^2)，LCS 朴素 dp，用来给 lcs_perm 对拍
int lcs_n2(int n,int a[],int b[])
{
    for(int i=0;i<=n;i++)lcsf[i][0]=0,lcsf[0][i]=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            if(a[i]==b[j])lcsf[i][j]=lcsf[i-1][j-1]+1;
            else lcsf[i][j]=max(lcsf[i-1][j],lcsf[i][j-1]);
    return lcsf[n][n];
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        int v[]={0,INT_MIN,INT_MAX,INT_MIN};
        assert(lis_nlogn(3,v)==2&&lnds_nlogn(3,v)==2&&lds_nlogn(3,v)==2);
    }

    srand(20240603);
    printf("==== 固定样例 ====\n");
    n=7;
    int s1[8]={0,1,7,3,5,9,4,8};
    printf("LIS n^2=%d  n log n=%d  暴力=%d (期望 4)\n",lis_n2(n,s1),lis_nlogn(n,s1),brute_lis(n,s1,0));
    lis_scheme(n,s1);
    n=3;
    int s2[4]={0,2,2,2};
    printf("不降 %d  严格 %d (期望 3 与 1)\n",lnds_nlogn(n,s2),lis_nlogn(n,s2));
    int s3[4]={0,5,4,3};
    printf("严格下降 %d (期望 3)\n",lds_nlogn(n,s3));
    n=5;
    int s4[6]={0,1,2,3,4,5},s5[6]={0,3,2,1,4,5};
    printf("排列 LCS 转 LIS=%d  O(n^2) LCS=%d (期望 3)\n",lcs_perm(n,s4,s5),lcs_n2(n,s4,s5));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=1000;tt++)
    {
        n=rndint(1,10);
        for(int i=1;i<=n;i++)a[i]=rndint(-5,5);
        int b0=brute_lis(n,a,0),b1=brute_lis(n,a,1),b2=brute_lis(n,a,2);
        if(lis_n2(n,a)!=b0){bad++;printf("WA! lis_n2 轮%d\n",tt);break;}
        if(lis_nlogn(n,a)!=b0){bad++;printf("WA! lis_nlogn 轮%d\n",tt);break;}
        if(lnds_nlogn(n,a)!=b1){bad++;printf("WA! lnds 轮%d\n",tt);break;}
        if(lds_nlogn(n,a)!=b2){bad++;printf("WA! lds 轮%d\n",tt);break;}
        n=rndint(1,9);
        for(int i=1;i<=n;i++)a[i]=i;
        for(int i=n;i>=2;i--)swap(a[i],a[rndint(1,i)]);// 随机排列
        for(int i=1;i<=n;i++)tmp[i]=i;
        for(int i=n;i>=2;i--)swap(tmp[i],tmp[rndint(1,i)]);
        int ref=lcs_n2(n,a,tmp),cur=lcs_perm(n,a,tmp);
        if(ref!=cur){bad++;printf("WA! lcs_perm 轮%d ref=%d cur=%d\n",tt,ref,cur);break;}
    }
    if(!bad)printf("stress OK (1000 轮，LIS n^2 / n log n / 不降 / 下降 / LCS转LIS 全部通过)\n");
    return 0;
}
