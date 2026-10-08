#include<bits/stdc++.h>
using namespace std;

// a[1..n]；0 操作：{0,位置,新值,0}；1 操作：{1,l,r,k}。
struct Operation{int type,x,y,k;};
// 按值域二分：<=mid 的修改进入 BIT，查询数出区间内这一半有几个。
// 查询去右半时 k 减掉左半数量；分组必须保持原时间顺序，递归前撤销 BIT。
// 修改拆成旧值 -1、新值 +1；O((n+q) log V log n)，V 是不同值数。
// 空间 O(n+q)：分组后释放父层事件，避免同一批事件在多层递归中重复保留。
vector<int> a;
vector<Operation> ops;
vector<int> range_kth()
{
    int n=(int)a.size()-1;
    assert(n>=0);
    vector<int> value(a.begin()+1,a.end());
    for(auto e : ops)
        if(e.type == 0)
            value.push_back(e.y);
    sort(value.begin(),value.end());
    value.erase(unique(value.begin(),value.end()),value.end());
    auto id= [&](int x)
    { return lower_bound(value.begin(), value.end(), x) - value.begin(); };
    struct Event{int type,x,y,k,delta,id;};
    vector<Event> events;
    for(int i= 1; i <= n; i++)
        events.push_back({0, i, (int)id(a[i]), 0, 1, 0});
    int queries=0;
    for(auto e:ops)
    {
        if(e.type==0)
        {
            assert(1<=e.x&&e.x<=n);
            events.push_back({0,e.x,(int)id(a[e.x]),0,-1,0});
            events.push_back({0,e.x,(int)id(e.y),0,1,0});
            a[e.x]=e.y;
        }
        else
        {
            assert(e.type==1&&1<=e.x&&e.x<=e.y&&e.y<=n&&1<=e.k&&e.k<=e.y-e.x+1);
            events.push_back({1,e.x,e.y,e.k,0,queries++});
        }
    }
    vector<int> ans(queries),bit(n+1);
    if(!queries)return ans;
    auto add= [&](int x, int v)
    {
        for(; x <= n; x+= x & -x)
            bit[x]+= v;
    };
    auto sum= [&](int x)
    {
        int s= 0;
        for(; x; x-= x & -x)
            s+= bit[x];
        return s;
    };
    auto solve=[&](auto &&self,int l,int r,vector<Event> e)->void
    {
        if(e.empty())return;
        if(l==r)
        {
            for(auto t:e)
            {
                if(t.type==1)ans[t.id]=value[l];
            }
            return;
        }
        int m=(l+r)/2;
        vector<Event> left,right;
        for(auto t:e)
        {
            if(t.type==0)
            {
                if(t.y<=m)add(t.x,t.delta),left.push_back(t);
                else right.push_back(t);
            }
            else
            {
                int count=sum(t.y)-sum(t.x-1);
                if(t.k<=count)left.push_back(t);
                else t.k-=count,right.push_back(t);
            }
        }
        for(auto t : left)
            if(t.type == 0)
                add(t.x, -t.delta);
        vector<Event>().swap(e);
        self(self,l,m,move(left)),self(self,m+1,r,move(right));
    };
    solve(solve,0,value.size()-1,move(events));
    return ans;
}
