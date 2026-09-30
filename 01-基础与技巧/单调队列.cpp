// 适用：固定长度滑动窗口最值；输入 a[1..n]，要求 1<=k<=n<N。
// 队列存下标才能判过期；相等时保留较新元素，值相同但剩余有效时间更长。
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

// O(1)，重置有效区间为空；每轮求最小/最大值前调用，不必清空整个 q。
void init_q()
{
    head=1,tail=0;
}

// 均摊 O(1)、单次最坏 O(n)，i 为递增输入下标；尾部 >= 当前值者不再可能最优。
void push_keep_inc(int i)// 维护队列内 a 单调递增（队首最小），用于求窗口最小值
{
    while(head<=tail&&a[q[tail]]>=a[i])tail--;
    q[++tail]=i;
}

// 均摊 O(1)、单次最坏 O(n)，i 为递增输入下标；维护最大值候选。
void push_keep_dec(int i)// 维护队列内 a 单调递减（队首最大），用于求窗口最大值
{
    while(head<=tail&&a[q[tail]]<=a[i])tail--;
    q[++tail]=i;
}

// 均摊 O(1)、单次最坏 O(n)，i 为窗口右端、k 为长度；保留闭窗口 [i-k+1,i]。
void pop_expire(int i,int k)// 弹出下标 <= i-k 的队首
{
    while(head<=tail&&q[head]<=i-k)head++;
}

// O(1)，取队首对应值；必须 head<=tail，不能在空队列调用。
int q_front()
{
    return a[q[head]];
}

int win_min[N],win_max[N];// 窗口最值答案

// O(n) 时间、O(n) 空间；k 为窗口长度，答案下标 j 对应 [j,j+k-1]。
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
