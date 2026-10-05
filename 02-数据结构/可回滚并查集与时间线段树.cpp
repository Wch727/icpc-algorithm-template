#include<bits/stdc++.h>
using namespace std;

// 按大小合并，禁止路径压缩；snapshot 记栈长，rollback 撤回到该时刻。
// find 最坏 O(log n)，一次撤销 O(1)；无需为没有实际合并的操作入栈。
struct RollbackDSU
{
    vector<int> fa,sz;
    vector<pair<int,int>> history;// 被挂的根、挂接前父根大小
    RollbackDSU(int n):fa(n+1),sz(n+1,1){iota(fa.begin(),fa.end(),0);}
    int find(int x){while(fa[x]!=x)x=fa[x];return x;}
    int snapshot(){return history.size();}
    bool merge(int x,int y)
    {
        x=find(x),y=find(y);
        if(x==y)return false;
        if(sz[x]<sz[y])swap(x,y);
        history.push_back({y,sz[x]});
        fa[y]=x,sz[x]+=sz[y];
        return true;
    }
    void rollback(int t)
    {
        while((int)history.size()>t)
        {
            auto [y,old]=history.back();history.pop_back();
            sz[fa[y]]=old,fa[y]=y;
        }
    }
};

// 点 1..n，初始无边；操作 {type,u,v}：0 加边，1 删边，2 查询连通。
// 无向边允许重复加入：全部副本删完才失效；删除必须有对应的活动副本。
// 每条边生效区间 [l,r) 挂到时间线段树；进入结点合并，退出撤销。
// q 次操作 O(q log q log n)，存边 O(q log q)，按查询出现顺序返回 0/1。
int n;
vector<array<int,3>> ops;
vector<int> dynamic_connectivity()
{
    int q=ops.size();
    if(!q)return {};
    vector<vector<pair<int,int>>> seg(4*q);
    auto put=[&](auto &&self,int p,int l,int r,int L,int R,pair<int,int> e)->void
    {
        if(R<=l||r<=L)return;
        if(L<=l&&r<=R){seg[p].push_back(e);return;}
        int m=(l+r)/2;
        self(self,p*2,l,m,L,R,e),self(self,p*2+1,m,r,L,R,e);
    };
    map<pair<int,int>,pair<int,int>> active;// 边 -> {副本数,起始时刻}
    for(int t=0;t<q;t++)
    {
        auto [type,u,v]=ops[t];
        assert(0<=type&&type<=2&&1<=u&&u<=n&&1<=v&&v<=n);
        if(type==2)continue;
        if(u>v)swap(u,v);
        auto &a=active[{u,v}];
        if(type==0){if(a.first++==0)a.second=t;}
        else
        {
            assert(a.first>0);
            if(--a.first==0)put(put,1,0,q,a.second,t,{u,v});
        }
    }
    for(auto [e,a]:active)if(a.first)put(put,1,0,q,a.second,q,e);
    RollbackDSU d(n);
    vector<int> ans;
    auto dfs=[&](auto &&self,int p,int l,int r)->void
    {
        int old=d.snapshot();
        for(auto [u,v]:seg[p])d.merge(u,v);
        if(r-l==1)
        {
            if(ops[l][0]==2)ans.push_back(d.find(ops[l][1])==d.find(ops[l][2]));
        }
        else{int m=(l+r)/2;self(self,p*2,l,m),self(self,p*2+1,m,r);}
        d.rollback(old);
    };
    dfs(dfs,1,0,q);
    return ans;
}
