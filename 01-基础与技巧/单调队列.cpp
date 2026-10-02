#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// q 存候选下标；每个候选最多入队、出队一次，总 O(n)。
// 适用：候选按入队顺序失效，过期后不再有效；队首始终是当前最优候选。
// 新候选在旧候选剩余有效期内始终有效且不劣，才能永久删除旧候选。
// 若候选优劣随查询改变、可能反转，不能直接套这个循环。
const int N=1000005;
int n,k,a[N],ans[N];

// 窗口最小值，下标从 1 开始，1<=k<=n；先加入 i，再查询包含 i 的窗口。
void solve()
{
    deque<int> q;
    for(int i=1;i<=n;i++)
    {
        while(!q.empty()&&q.front()<=i-k)q.pop_front();
        while(!q.empty()&&a[q.back()]>=a[i])q.pop_back();
        q.push_back(i);
        ans[i]=q.front(); // 记录当前最优候选下标；具体取值/转移在这里改
    }
}
// 窗口 [max(1,i-k+1),i]；求最大值把队尾比较 >= 改为 <=。
// 上述非严格比较保留相等值中最新的下标；改成 > / < 则保留最早的下标。
// ans[i] 存下标，窗口值为 a[ans[i]]；只要完整窗口时，i>=k 才记录答案。
// 自定义题目：将队首条件换成 expired(id,i)，队尾条件换成 check(old,now)。
// expired 判断是否失效，check 判断新候选能否永久淘汰旧候选，须满足上面的适用条件。

// DP 示例：f[0]=0，f[i]=a[i]+max{f[j] | max(0,i-k)<=j<i}，a 下标从 1 开始，k>=1。
// 先过期、再转移、最后加入 i；初始候选 0 不可漏，不能让 i 转移到自己。
vector<ll> dp(const vector<ll> &a,int k)
{
    int n=(int)a.size()-1;
    vector<ll> f(n+1);
    deque<int> q{0};
    for(int i=1;i<=n;i++)
    {
        while(!q.empty()&&q.front()<i-k)q.pop_front();
        f[i]=f[q.front()]+a[i];
        while(!q.empty()&&f[q.back()]<=f[i])q.pop_back();
        q.push_back(i);
    }
    return f;
}
// 按题目改转移值、有效区间及队尾比较；只把可达状态入队，空队时当前状态不可达。
