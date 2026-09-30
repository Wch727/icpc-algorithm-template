#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 单调队列：每个元素最多入队出队一次，O(n)，用于滑动窗口最值
// 手写数组队列，空间 O(n)，常数比 deque 小
int n,m;
int a[N];
int q[N];// 存下标，head..tail 是有效区间
int head,tail;

void init_q()
{
    head=1,tail=0;
}

void push_keep_inc(int i)// 维护队列内 a 单调递增（队首最小），用于求窗口最小值
{
    while(head<=tail&&a[q[tail]]>=a[i])tail--;
    q[++tail]=i;
}

void push_keep_dec(int i)// 维护队列内 a 单调递减（队首最大），用于求窗口最大值
{
    while(head<=tail&&a[q[tail]]<=a[i])tail--;
    q[++tail]=i;
}

void pop_expire(int i,int k)// 弹出下标 <= i-k 的队首
{
    while(head<=tail&&q[head]<=i-k)head++;
}

int q_front()
{
    return a[q[head]];
}

int win_min[N],win_max[N];// 窗口最值答案

void sliding_window(int k)// 窗口长度 k，结果存在 win_min/win_max[1..n-k+1]
{
    init_q();
    for(int i=1;i<=n;i++)
    {
        push_keep_inc(i);
        pop_expire(i,k);
        if(i>=k)win_min[i-k+1]=q_front();
    }
    init_q();
    for(int i=1;i<=n;i++)
    {
        push_keep_dec(i);
        pop_expire(i,k);
        if(i>=k)win_max[i-k+1]=q_front();
    }
}

int main()
{
    srand(20240519);
    // 自测1：洛谷 P1886 样例
    n=8,m=3;
    int v[9]={0,1,3,-1,-3,5,3,6,7};
    for(int i=1;i<=n;i++)a[i]=v[i];
    sliding_window(m);
    printf("min: ");
    for(int i=1;i<=n-m+1;i++)printf("%d ",win_min[i]);
    printf("\n");// -1 -3 -3 -3 3 3
    printf("max: ");
    for(int i=1;i<=n-m+1;i++)printf("%d ",win_max[i]);
    printf("\n");// 3 3 5 5 6 7

    // 自测2：与 O(nk) 暴力对拍
    for(int t=1;t<=500;t++)
    {
        n=rand()%40+1;
        m=rand()%n+1;
        for(int i=1;i<=n;i++)a[i]=rand()%21-10;
        sliding_window(m);
        for(int s=1;s+m-1<=n;s++)
        {
            int mn=a[s],mx=a[s];
            for(int i=s;i<=s+m-1;i++)mn=min(mn,a[i]),mx=max(mx,a[i]);
            if(win_min[s]!=mn||win_max[s]!=mx)
            {
                printf("fail window t=%d s=%d got=%d,%d want=%d,%d\n",t,s,win_min[s],win_max[s],mn,mx);
                return 0;
            }
        }
    }
    printf("monotonic queue self-check OK\n");

    // 自测3：洛谷 P1440 变形——求前 m 个数的最小值，不足 m 个输出 0
    n=6,m=2;
    int w[7]={0,7,8,1,4,3,2};
    for(int i=1;i<=n;i++)a[i]=w[i];
    init_q();
    printf("pre-min: 0 ");
    for(int i=1;i<n;i++)
    {
        push_keep_inc(i);
        pop_expire(i,m);
        printf("%d ",q_front());
    }
    printf("\n");// 0 7 7 1 1
    return 0;
}
