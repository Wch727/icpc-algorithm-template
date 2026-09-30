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
