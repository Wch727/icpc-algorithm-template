// 单调队列优化DP 的测试与对拍代码
// 模板本体：06-动态规划/单调队列优化DP.cpp
#include "../../06-动态规划/单调队列优化DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// O(n)，滑动窗口最大值
// 三步：入队前把队尾比它差的弹掉 -> 入队 -> 把越界的队首弹掉
void window_max(int n,int k,int a[],int res[])
{
    int head=0,tail=0;
    for(int i=1;i<=n;i++)
    {
        while(head<tail&&a[q[tail-1]]<=a[i])tail--;
        q[tail++]=i;
        while(q[head]<i-k+1)head++;// 窗口左端是 i-k+1
        if(i>=k)res[i-k+1]=a[q[head]];
    }
}

// O(n)，滑动窗口最小值，队列改成单调递增
void window_min(int n,int k,int a[],int res[])
{
    int head=0,tail=0;
    for(int i=1;i<=n;i++)
    {
        while(head<tail&&a[q[tail-1]]>=a[i])tail--;
        q[tail++]=i;
        while(q[head]<i-k+1)head++;
        if(i>=k)res[i-k+1]=a[q[head]];
    }
}

// O(n)，单调队列优化转移：f[i]=a[i]+max(f[j])，i-R<=j<=i-L
// f[i] 表示以 i 结尾的最大得分；j 也可以不选(原地起步)
// 处理 i 时先把下标 i-L 入队，再把 < i-R 的弹掉，窗口正好是 [i-R,i-L]
int jump_max_score(int n,int L,int R,int a[])
{
    int head=0,tail=0,ans=-INF;
    for(int i=1;i<=n;i++)
    {
        int add=i-L;
        if(add>=1)
        {
            while(head<tail&&f[q[tail-1]]<=f[add])tail--;
            q[tail++]=add;
        }
        while(head<tail&&q[head]<i-R)head++;
        f[i]=a[i];
        if(head<tail)f[i]=max(f[i],f[q[head]]+a[i]);
        ans=max(ans,f[i]);
    }
    return ans;
}

// 暴力：滑动窗口
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
    window_max(n,k,s1,res);
    printf("窗口最大值 :");
    for(int i=1;i<=n-k+1;i++)printf(" %d",res[i]);
    printf(" (期望 3 3 5 5 6 7)\n");
    window_min(n,k,s1,res);
    printf("窗口最小值 :");
    for(int i=1;i<=n-k+1;i++)printf(" %d",res[i]);
    printf(" (期望 -1 -3 -3 -3 3 3)\n");
    n=5,L=1,R=2;
    int s2[6]={0,1,-1,2,-2,3};
    printf("跳跃最大得分 : %d (期望 6)\n",jump_max_score(n,L,R,s2));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=500;tt++)
    {
        n=rndint(1,200),k=rndint(1,n);
        for(int i=1;i<=n;i++)a[i]=rndint(-50,50);
        window_max(n,k,a,mx),window_min(n,k,a,mn);
        brute_window(n,k,a,bmx,bmn);
        int wa=0;
        for(int i=1;i+k-1<=n;i++)
            if(mx[i]!=bmx[i]||mn[i]!=bmn[i])wa=1;
        if(wa){bad++;printf("WA! 窗口 轮%d n=%d k=%d\n",tt,n,k);break;}
        L=rndint(1,n),R=rndint(L,n);
        for(int i=1;i<=n;i++)a[i]=rndint(-50,50);
        int cur=jump_max_score(n,L,R,a);
        int ref=brute_jump(n,L,R,a);
        if(cur!=ref){bad++;printf("WA! 跳跃 轮%d n=%d L=%d R=%d ref=%d cur=%d\n",tt,n,L,R,ref,cur);break;}
    }
    if(!bad)printf("stress OK (500 轮，滑动窗口最大最小 / 跳跃 dp 全部通过)\n");
    return 0;
}
