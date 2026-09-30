#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int INF=0x3f3f3f3f;
int n;
int a[N],d[N],f[N],pre[N],tmp[N],mp[N],c[N];
int st[N];// 还原方案用的栈
int lcsf[15][15];

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// O(n^2)，最长严格上升子序列：f[i] 表示以 a[i] 结尾的最长长度
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

// O(n log n)，最长严格上升子序列
// d[len] 表示长度 len 的上升子序列的最小结尾，d 单调不减
int lis_nlogn(int n,int a[])
{
    int len=0;
    d[0]=-INF;// 哨兵，保证第一个数一定能接上
    for(int i=1;i<=n;i++)
    {
        if(a[i]>d[len])d[++len]=a[i];// 比最长结尾还大就直接接上
        else
        {
            int l=1,r=len;// 二分第一个 >= a[i] 的位置替换掉
            while(l<r)
            {
                int mid=(l+r)>>1;
                if(d[mid]>=a[i])r=mid;
                else l=mid+1;
            }
            d[l]=a[i];
        }
    }
    return len;
}

// O(n log n)，最长不降子序列：只把 >= 改成 >，二分找第一个比 a[i] 大的
int lnds_nlogn(int n,int a[])
{
    int len=0;
    d[0]=-INF;
    for(int i=1;i<=n;i++)
    {
        if(a[i]>=d[len])d[++len]=a[i];
        else
        {
            int l=1,r=len;
            while(l<r)
            {
                int mid=(l+r)>>1;
                if(d[mid]>a[i])r=mid;
                else l=mid+1;
            }
            d[l]=a[i];
        }
    }
    return len;
}

// O(n log n)，最长严格下降子序列：把值全部取负就变成严格上升
int lds_nlogn(int n,int a[])
{
    for(int i=1;i<=n;i++)tmp[i]=-a[i];
    return lis_nlogn(n,tmp);
}

// O(n log n)，两个排列(值 1..n)的 LCS 转 LIS
// a 中每个值的位置记下来，按 b 的顺序排成 c，c 的 LIS 就是 LCS
int lcs_perm(int n,int a[],int b[])
{
    for(int i=1;i<=n;i++)mp[a[i]]=i;// 值 -> 在 a 中的位置
    for(int i=1;i<=n;i++)c[i]=mp[b[i]];
    return lis_nlogn(n,c);
}

// O(n^2)，记录前驱并输出一组最优解
void lis_scheme(int n,int a[])
{
    int ans=0,best=0,top=0;
    for(int i=1;i<=n;i++)
    {
        f[i]=1,pre[i]=0;
        for(int j=1;j<i;j++)
            if(a[j]<a[i]&&f[j]+1>f[i])f[i]=f[j]+1,pre[i]=j;
        if(f[i]>ans)ans=f[i],best=i;
    }
    while(best)st[++top]=a[best],best=pre[best];// 逆着走前驱
    printf("长度 %d，一组方案：",ans);
    for(int i=top;i>=1;i--)printf("%d ",st[i]);
    printf("\n");
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
    srand(20240603);
    printf("==== 固定样例 ====\n");
    n=8;
    int s1[9]={0,1,7,3,5,9,4,8};
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
