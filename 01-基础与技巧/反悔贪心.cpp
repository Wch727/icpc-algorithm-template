// 重复权值哈夫曼：最小 w 有 c 个时，批量 floor(c/2) 次合并，代价 2*w*floor(c/2)，生成 2*w。
// c 为奇数时，余下一个 w 与当前次小配对；原权排序后与新生成的单调队列取两边最小。
// 若每种符号必须编码，零权符号也不能扔掉；计数与总代价用足够宽的整数。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 按截止时间排序，堆保存已选任务；放不下时撤销最不值得保留的。
// 收益最大：踢最低收益，用小根堆；件数最多：踢最长耗时，用大根堆。
// 两种模型均为 O(n log n)，不能推广为任意耗时、任意收益的调度。

// 1. 单位时长，最大化收益（P2949）：d>=0、p>=0。
// 到截止时刻 d 最多选 d 件，小根堆维护已选任务的收益。
struct Job
{
    int d,p;
};

ll job_schedule(vector<Job> v)
{
    sort(v.begin(),v.end(),[](const Job &a,const Job &b){return a.d<b.d;});
    priority_queue<int,vector<int>,greater<int> > q;
    ll ans=0;
    for(Job x:v)
    {
        q.push(x.p),ans+=x.p;
        if((int)q.size()>x.d)ans-=q.top(),q.pop(); // 连新任务一起比较，撤销最低收益
    }
    return ans;
}
// 为什么能撤销：此前任务已可行，本轮只可能多出一件；删除任意一件都恢复可行。
// 同样腾出一个时段，删最低收益损失最小；后续截止时间不会更早。

// 2. 不同耗时，最大化件数（P4053）：t>0、d>=0，从时刻 0 起可安排任务。
// tot 为已选总耗时；任务按截止时间顺序执行，大根堆保存耗时。
struct Task
{
    ll t,d;
};

int task_schedule(vector<Task> v)
{
    sort(v.begin(),v.end(),[](const Task &a,const Task &b){return a.d<b.d;});
    priority_queue<ll> q;
    ll tot=0;
    for(Task x:v)
    {
        if(tot+x.t<=x.d)
        {
            tot+=x.t;
            q.push(x.t);
        }
        else if(!q.empty()&&q.top()>x.t)
        {
            tot+=x.t-q.top(); // 数量不变，总耗时变短，为后续任务留下空间
            q.pop();
            q.push(x.t);
        }
    }
    return (int)q.size();
}
// 替换为何可行：移除旧任务后，其余任务只会提前；新任务放在当前末尾。
// 新耗时更短，所以新 tot 小于旧 tot，而旧 tot<=当前截止时间。
// 同样的件数，删最长任务能省下最多时间；新任务不更短时直接跳过。
// 例：(耗时,截止)=(4,4),(2,5),(3,6)：先选 4，再换成 2，最后可选 2 和 3。
// 不同耗时还要求收益最大时，以上两种堆策略都不保证最优。
