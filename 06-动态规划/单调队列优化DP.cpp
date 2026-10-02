#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int INF=0x3f3f3f3f;
int n,k,L,R;
int a[N],f[N],res[N];
int q[N];// 单调队列，存下标（也可以用 deque<int>，见 P1886 写法）
int mx[N],mn[N],bmx[N],bmn[N];// 对拍用的窗口结果

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
