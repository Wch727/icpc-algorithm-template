// 适用：存在交换论证的调度选择题；堆替换正确性要先证明，普通背包不能直接套。
// vector 输入为 0-indexed；收益总和与性价比交叉乘积须在 ll 范围内。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 反悔贪心：先用最朴素的贪心一路选，选不下了/发现亏了就用堆把之前的选择「退掉」
// 套路固定：答案累加 -> 把当前选择丢进堆 -> 堆超限或更优时弹出堆顶（代价最大的那个）
// 堆里存的都是「已经做出的选择」，弹出 = 反悔
// 复杂度：每个元素进堆出堆各一次，O(n log n)
int n,m,k;
ll a[N],b[N];

// ---- 模板1：工作调度（洛谷 P2949 / P4053 一类）----
// 每件工作有截止时间 d 和收益 p，同一时刻只能做一件，求最大收益
// 贪心：按截止时间排序，能塞就塞；塞满了还来活，就把堆里收益最小的踢掉换成当前这个
// 单位时长工作，空间 O(1)；d 为非负截止时刻，p 为非负收益。
struct Job
{
    int d,p;
};

// O(1)，比较 x、y 工作；先处理早截止任务，后续工作不会放宽之前的期限。
bool cmp_job(const Job& x,const Job& y)
{
    if(x.d!=y.d)return x.d<y.d;// 截止时间早的先考虑
    return x.p>y.p;
}

// 传值进来，不改调用方的数组
// O(n log n) 时间、O(n) 空间；v 为单位时长工作集合，返回最大总收益。
ll solve_job_schedule(vector<Job> v)
{
    sort(v.begin(),v.end(),cmp_job);
    priority_queue<int,vector<int>,greater<int>> pq;// 小根堆，堆顶是已选里收益最小的
    ll ans=0;
    for(int i=0;i<(int)v.size();i++)
    {
        if((int)pq.size()<v[i].d)// 前面还有空位，直接做
        {
            pq.push(v[i].p);
            ans+=v[i].p;
        }
        else if(!pq.empty()&&pq.top()<v[i].p)// 没位置了，反悔掉收益最小的那个
        {
            ans-=pq.top();
            pq.pop();
            pq.push(v[i].p);
            ans+=v[i].p;
        }
    }
    return ans;
}

// ---- 股票问题的两种写法：别把「配对贪心」当成正解 ----
// 题目：手里一开始没钱没股票，每天可以买一股或卖一股（同一时刻最多持一股），求最大收益
// 结论（三种独立方法互相验证过）：不限交易次数时答案就是「所有相邻正差之和」，O(n) 一行
// 而「来了更低价就换持仓、否则卖掉」的堆配对贪心答案是错的：
//    它允许同一个买点被反复配对，[5,3,11,6,2,9,18,0,20,1] 会算出 48，正解是 44
//    [1,2,3] 它算 3，正解是 2。写反悔贪心遇到这题一定要拿暴力 DP 对拍
// O(n) 时间、O(1) 空间；p 为每天价格，无手续费、持股上限一股，累加相邻上涨段。
ll stock_unlimited_best(const vector<ll>& p)
{
    ll ans=0;
    for(int i=1;i<(int)p.size();i++)
        if(p[i]>p[i-1])ans+=p[i]-p[i-1];
    return ans;
}

// 错解，留作错题本：堆配对会凭空多赚
// O(n log n) 时间、O(n) 空间；p 为价格；此历史示例不满足单股持仓约束，不作为求解接口。
ll solve_stock_wrong(vector<ll> p)
{
    priority_queue<ll,vector<ll>,greater<ll>> pq;
    ll ans=0;
    for(int i=0;i<(int)p.size();i++)
    {
        if(!pq.empty()&&pq.top()<p[i])
        {
            ll buy=pq.top();
            pq.pop();
            ans+=p[i]-buy;// 这次配对凭空复用了一个买点
            pq.push(p[i]);
            pq.push(buy);
        }
        else pq.push(p[i]);
    }
    return ans;
}

// 限制「最多 k 次交易」时必须用 DP：dp[j][0/1] = 做完 j 次交易、手上有没有股票
// 哨兵不能用 LLONG_MIN/4 这种「离 0 不远」的值，减价格会绕回正数，必须显式判可达
// O(|p|*k) 时间、O(k) 空间；k 为最多完成的交易数，p 为价格，均须非负。
// j 逆序避免本日新状态流向下一次交易；显式判 NEG 后才能加减，实际收益仍须防溢出。
ll dp_stock_k(const vector<ll>& p,int k)
{
    int len=(int)p.size();
    const ll NEG=LLONG_MIN/4;
    vector<vector<ll>> dp(k+1,vector<ll>(2,NEG));
    dp[0][0]=0;
    for(int i=0;i<len;i++)
    {
        for(int j=k;j>=1;j--)
        {
            if(dp[j][1]!=NEG)dp[j][0]=max(dp[j][0],dp[j][1]+p[i]);// 卖出，完成一次交易
            if(dp[j-1][0]!=NEG)dp[j][1]=max(dp[j][1],dp[j-1][0]-p[i]);// 买入，算是第 j 次
        }
    }
    ll res=0;
    for(int j=0;j<=k;j++)res=max(res,dp[j][0]);
    return res;
}

// ---- 模板3：反悔贪心通用骨架（选物品）----
// 求「从 n 个物品里选若干个，总代价不超过 lim，收益尽量大」
// 注意：0/1 背包是 NP-hard，这套贪心+反悔只是启发式，不保证最优！
// 想要精确解老老实实写 DP。这里保留它是为了演示「反悔」这个动作怎么写，
// 以及演示「反悔时要连收益一起退」这个高频 bug
// 物品收益 val 与成本 cost，空间 O(1)；性价比排序要求 cost>0、val>=0。
struct Item
{
    ll val,cost;
};

// 按性价比 val/cost 从大到小排，比只看 val 靠谱得多
// O(1)，按 x、y 的收益/成本排序；交叉乘积须在 ll 内，避免浮点比较的不稳定。
bool cmp_item(const Item& x,const Item& y)
{
    ll l=x.val*y.cost,r=y.val*x.cost;// 交叉相乘比大小，避免浮点误差
    if(l!=r)return l>r;
    return x.cost<y.cost;
}

// 堆里放 (cost,val)，堆顶是已选里代价最大的
// 反悔时 cost 和 val 必须一起退，只退 cost 会在答案里留下幽灵收益
// O(n log n) 时间、O(n) 空间；it 为物品、lim 为预算，返回启发式收益，不保证全局最优。
ll solve_pick_greedy(vector<Item> it,ll lim)
{
    sort(it.begin(),it.end(),cmp_item);
    priority_queue<pair<ll,ll>> pq;
    ll sum=0,ans=0;
    for(int i=0;i<(int)it.size();i++)
    {
        if(sum+it[i].cost<=lim)// 放得下就直接放
        {
            sum+=it[i].cost;
            ans+=it[i].val;
            pq.push({it[i].cost,it[i].val});
        }
        else if(!pq.empty()&&pq.top().first>it[i].cost)// 放不下就反悔代价最大的那个
        {
            sum-=pq.top().first;
            ans-=pq.top().second;// 收益要跟着一起退
            pq.pop();
            sum+=it[i].cost;
            ans+=it[i].val;
            pq.push({it[i].cost,it[i].val});
        }
    }
    return ans;
}
