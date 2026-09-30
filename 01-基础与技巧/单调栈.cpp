#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 单调栈：每个元素最多入栈出栈一次，O(n)
int n;
int a[N];
int nge[N];// 右边第一个 >a[i] 的下标，没有记 0
int nge2[N];// 右边第一个 >=a[i] 的下标，没有记 0
int nse[N];// 右边第一个 <a[i] 的下标，没有记 0
int nle[N];// 左边第一个 <=a[i] 的下标，没有记 0
int st[N],top;// 手写数组栈，比 stack<int> 快
int bf1[N],bf2[N],bf3[N],bf4[N];// 暴力对拍用

void get_nge()// 下一个更大元素：栈内下标对应值单调递减
{
    top=0;
    memset(nge,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]<a[i])nge[st[top]]=i,top--;
        st[++top]=i;
    }
}

void get_nge2()// 下一个大于等于：弹栈条件改成 <=，保证相等不互相吃掉
{
    top=0;
    memset(nge2,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]<=a[i])nge2[st[top]]=i,top--;
        st[++top]=i;
    }
}

void get_nse()// 下一个更小元素
{
    top=0;
    memset(nse,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]>a[i])nse[st[top]]=i,top--;
        st[++top]=i;
    }
}

void get_nle()// 从左往右扫，得到左边第一个 <=a[i]
{
    top=0;
    memset(nle,0,sizeof(int)*(n+2));
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]>a[i])top--;// 弹掉左边比它大的，栈里全 <=a[i]
        if(top>0)nle[i]=st[top];// 栈顶就是离它最近的那个
        st[++top]=i;
    }
}

ll count_see()// 洛谷 P2866：每头牛向右能看到的牛数之和
{
    // 反过来数：连续一段严格矮于它的才会被它挡住视线
    // 维护一个「严格递减」的栈（存身高）：把 <=a[i] 的全弹掉，剩下的都能看见第 i 头牛
    top=0;
    ll ans=0;
    for(int i=1;i<=n;i++)
    {
        while(top>0&&a[st[top]]<=a[i])top--;// 矮的或一样高的都出栈
        ans+=top;
        st[++top]=i;
    }
    return ans;
}

int main()
{
    srand(20240518);
    // 自测1：经典样例
    n=8;
    int v[9]={0,3,1,4,1,5,9,2,6};
    for(int i=1;i<=n;i++)a[i]=v[i];
    get_nge();
    printf("nge : ");
    for(int i=1;i<=n;i++)printf("%d ",nge[i]);
    printf("\n");// 3 3 5 5 6 0 8 0
    get_nse();
    printf("nse : ");
    for(int i=1;i<=n;i++)printf("%d ",nse[i]);
    printf("\n");// 2 0 4 0 7 7 0 0
    get_nle();
    printf("nle : ");
    for(int i=1;i<=n;i++)printf("%d ",nle[i]);
    printf("\n");
    printf("count_see=%lld\n",count_see());

    // 自测2：与 O(n^2) 暴力对拍
    for(int t=1;t<=500;t++)
    {
        n=rand()%30+1;
        for(int i=1;i<=n;i++)a[i]=rand()%6;// 小值域，制造大量相等
        // 先算暴力（单调栈会挪动 a 里的元素，必须先算完）
        for(int i=1;i<=n;i++)
        {
            bf1[i]=bf2[i]=bf3[i]=bf4[i]=0;
            for(int j=i+1;j<=n;j++)
            {
                if(!bf1[i]&&a[j]>a[i])bf1[i]=j;
                if(!bf2[i]&&a[j]>=a[i])bf2[i]=j;
                if(!bf3[i]&&a[j]<a[i])bf3[i]=j;
            }
            for(int j=i-1;j>=1;j--)// 最近的一个才是答案
                if(a[j]<=a[i])
                {
                    bf4[i]=j;
                    break;
                }
        }
        // 可见牛数对拍：i 能看见 j 当且仅当 i+1..j 全都严格矮于 h[i]
        ll bs=0;
        for(int i2=1;i2<=n;i2++)
            for(int j2=i2+1;j2<=n;j2++)
            {
                if(a[j2]>=a[i2])break;// 一碰到不矮的就断了
                bs++;
            }
        get_nge();
        get_nge2();
        get_nse();
        get_nle();
        for(int i=1;i<=n;i++)
            if(nge[i]!=bf1[i]||nge2[i]!=bf2[i]||nse[i]!=bf3[i]||nle[i]!=bf4[i])
            {
                printf("fail stack t=%d i=%d got=%d,%d,%d,%d want=%d,%d,%d,%d\n",t,i,nge[i],nge2[i],nse[i],nle[i],bf1[i],bf2[i],bf3[i],bf4[i]);
                return 0;
            }
        ll got_see=count_see();
        if(got_see!=bs)
        {
            printf("fail count_see t=%d got=%lld want=%lld n=%d a:",t,got_see,bs,n);
            for(int z=1;z<=n;z++)printf(" %d",a[z]);
            printf("\n");
            return 0;
        }
    }
    printf("monotonic stack self-check OK\n");
    n=6;
    int wv[9]={0,10,3,7,4,12,2};// 洛谷 P2866 样例
    for(int i=1;i<=n;i++)a[i]=wv[i];
    printf("P2866 sample count_see=%lld (want 5)\n",count_see());
    return 0;
}
