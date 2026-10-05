// 单调队列优化DP 的测试与对拍代码
// 模板本体：06-动态规划/单调队列优化DP.cpp
#include "../../06-动态规划/单调队列优化DP.cpp"

int mx[N],mn[N],bmx[N],bmn[N];
void load_case(int len,int width,const int *aa)
{::n=len;::k=width;for(int i=1;i<=len;i++)::a[i]=aa[i];}
void load_jump(int len,int lo,int hi,const int *aa)
{load_case(len,1,aa);::L=lo;::R=hi;}

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

void brute_window(int n,int k,int a[],int mx[],int mn[])
{
    for(int i=1;i+k-1<=n;i++)
    {
        mx[i]=-INF,mn[i]=INF;
        for(int j=i;j<=i+k-1;j++)mx[i]=max(mx[i],a[j]),mn[i]=min(mn[i],a[j]);
    }
}

// 暴力：跳跃 dp，直接枚举 j
int brute_jump(int n,int L,int R,int a[])
{
    int ans=-INF;
    for(int i=1;i<=n;i++)
    {
        f[i]=a[i];// 注意这里会覆盖 f，所以单独用一个数组
        ans=max(ans,f[i]);
    }
    for(int i=1;i<=n;i++)
    {
        int best=a[i];
        for(int j=max(1,i-R);j<=i-L;j++)
            if(j>=1&&j<=n)best=max(best,f[j]+a[i]);
        f[i]=best;
        ans=max(ans,f[i]);
    }
    return ans;
}

int main()
{
    srand(20240608);
    printf("==== 固定样例 ====\n");
    n=8,k=3;
    int s1[9]={0,1,3,-1,-3,5,3,6,7};
    (load_case(n,k,s1),window_max(),copy(::res+1,::res+::n-::k+2,res+1));
    printf("窗口最大值 :");
    for(int i=1;i<=n-k+1;i++)printf(" %d",res[i]);
    printf(" (期望 3 3 5 5 6 7)\n");
    (load_case(n,k,s1),window_min(),copy(::res+1,::res+::n-::k+2,res+1));
    printf("窗口最小值 :");
    for(int i=1;i<=n-k+1;i++)printf(" %d",res[i]);
    printf(" (期望 -1 -3 -3 -3 3 3)\n");
    n=5,L=1,R=2;
    int s2[6]={0,1,-1,2,-2,3};
    printf("跳跃最大得分 : %d (期望 6)\n",(load_jump(n,L,R,s2),jump_max_score()));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=500;tt++)
    {
        n=rndint(1,200),k=rndint(1,n);
        for(int i=1;i<=n;i++)a[i]=rndint(-50,50);
        (load_case(n,k,a),window_max(),copy(::res+1,::res+::n-::k+2,mx+1)),(load_case(n,k,a),window_min(),copy(::res+1,::res+::n-::k+2,mn+1));
        brute_window(n,k,a,bmx,bmn);
        int wa=0;
        for(int i=1;i+k-1<=n;i++)
            if(mx[i]!=bmx[i]||mn[i]!=bmn[i])wa=1;
        if(wa){bad++;printf("WA! 窗口 轮%d n=%d k=%d\n",tt,n,k);break;}
        L=rndint(1,n),R=rndint(L,n);
        for(int i=1;i<=n;i++)a[i]=rndint(-50,50);
        int cur=(load_jump(n,L,R,a),jump_max_score());
        int ref=brute_jump(n,L,R,a);
        if(cur!=ref){bad++;printf("WA! 跳跃 轮%d n=%d L=%d R=%d ref=%d cur=%d\n",tt,n,L,R,ref,cur);break;}
    }
    if(!bad)printf("stress OK (500 轮，滑动窗口最大最小 / 跳跃 dp 全部通过)\n");
    return 0;
}
