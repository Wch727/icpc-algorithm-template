// 三种莫队：输入值已经离散化到 0..sigma-1；例题指标供替换 add/remove/check。
// 带修改：区间不同数，记录修改前后值；排序块长 n^(2/3)，典型移动量 O(n^(5/3))（q~n）。
// 树上：路径不同色，Euler 每点出现两次，进出时翻转 active；LCA 额外加入一次。
// 回滚：区间同值最远距离，只增加右端、临时增加左端后撤销，O((n+q)sqrt n)。
#include<bits/stdc++.h>
using namespace std;
struct Modification{int pos,before,after;};
struct TimedQuery{int l,r,time;};
vector<int> a,color;
vector<Modification> change;
vector<TimedQuery> query;
vector<vector<int>> g;
vector<pair<int,int>> queries,range_query;
vector<int> modified_mo(int sigma)
{
    int n=a.size(),block=max(1,(int)pow(max(1,n),2.0/3));vector<int> order(query.size());iota(order.begin(),order.end(),0);
    sort(order.begin(),order.end(),[&](int x,int y){auto a=query[x],b=query[y];return tuple(a.l/block,a.r/block,a.time)<tuple(b.l/block,b.r/block,b.time);});
    vector<int> freq(sigma),ans(query.size());int l=0,r=-1,t=0,different=0;
    auto add=[&](int x){if(!freq[x]++)different++;};auto del=[&](int x){if(!--freq[x])different--;};
    auto apply=[&](int i,bool forward){auto c=change[i];if(l<=c.pos&&c.pos<=r)del(a[c.pos]);a[c.pos]=forward?c.after:c.before;if(l<=c.pos&&c.pos<=r)add(a[c.pos]);};
    for(int id:order){auto q=query[id];while(t<q.time)apply(t++,true);while(t>q.time)apply(--t,false);while(l>q.l)add(a[--l]);while(r<q.r)add(a[++r]);while(l<q.l)del(a[l++]);while(r>q.r)del(a[r--]);ans[id]=different;}
    return ans;
}
vector<int> tree_mo(int sigma)
{
    int n=g.size();if(!n){assert(queries.empty());return {};}
    int log=1;while((1LL<<log)<=n)log++;vector<vector<int>> up(log,vector<int>(n));vector<int> tin(n),tout(n),depth(n),euler,idx(n),parent(n,-1),stack={0};
    parent[0]=0;
    while(!stack.empty())
    {int u=stack.back();if(idx[u]==0){tin[u]=euler.size();euler.push_back(u);up[0][u]=parent[u];for(int k=1;k<log;k++)up[k][u]=up[k-1][up[k-1][u]];}
        if(idx[u]<(int)g[u].size()){int v=g[u][idx[u]++];if(v==parent[u])continue;parent[v]=u;depth[v]=depth[u]+1;stack.push_back(v);}else{tout[u]=euler.size();euler.push_back(u);stack.pop_back();}}
    auto lca=[&](int a,int b){if(depth[a]<depth[b])swap(a,b);int d=depth[a]-depth[b];for(int k=0;k<log;k++)if(d>>k&1)a=up[k][a];if(a==b)return a;for(int k=log-1;k>=0;k--)if(up[k][a]!=up[k][b])a=up[k][a],b=up[k][b];return up[0][a];};
    struct Q{int l,r,extra,id;};vector<Q> q;
    for(int i=0;i<(int)queries.size();i++){auto [u,v]=queries[i];if(tin[u]>tin[v])swap(u,v);int a=lca(u,v);if(a==u)q.push_back({tin[u],tin[v],-1,i});else q.push_back({tout[u],tin[v],a,i});}
    int block=max(1,(int)sqrt(2*n));sort(q.begin(),q.end(),[&](Q a,Q b){if(a.l/block!=b.l/block)return a.l<b.l;return (a.l/block&1)?a.r>b.r:a.r<b.r;});
    vector<int> freq(sigma),ans(q.size());vector<bool> active(n);int distinct=0,l=0,r=-1;
    auto flip=[&](int u){int c=color[u];if(active[u]){if(!--freq[c])distinct--;}else if(!freq[c]++)distinct++;active[u]=!active[u];};
    for(auto a:q){while(l>a.l)flip(euler[--l]);while(r<a.r)flip(euler[++r]);while(l<a.l)flip(euler[l++]);while(r>a.r)flip(euler[r--]);if(a.extra>=0)flip(a.extra);ans[a.id]=distinct;if(a.extra>=0)flip(a.extra);}
    return ans;
}
vector<int> rollback_mo(int sigma)
{
    int n=a.size(),block=max(1,(int)sqrt(max(1,n)));vector<int> order(range_query.size()),ans(range_query.size());iota(order.begin(),order.end(),0);
    sort(order.begin(),order.end(),[&](int i,int j){return pair(range_query[i].first/block,range_query[i].second)<pair(range_query[j].first/block,range_query[j].second);});
    vector<int> first(sigma,n),last(sigma,-1);struct Change{int v,first,last,best;};vector<Change> log;int best=0;
    auto add=[&](int pos){int v=a[pos];log.push_back({v,first[v],last[v],best});first[v]=min(first[v],pos);last[v]=max(last[v],pos);best=max(best,last[v]-first[v]);};
    auto rollback=[&](int size){while((int)log.size()>size){auto c=log.back();log.pop_back();first[c.v]=c.first;last[c.v]=c.last;best=c.best;}};
    int current=-1,right=-1;
    for(int id:order)
    {
        auto [l,r]=range_query[id];int b=l/block,end=min(n,(b+1)*block);
        if(b!=current){rollback(0);current=b;right=end-1;}
        if(r<end){int save=log.size();for(int p=l;p<=r;p++)add(p);ans[id]=best;rollback(save);}
        else{while(right<r)add(++right);int save=log.size();for(int p=end-1;p>=l;p--)add(p);ans[id]=best;rollback(save);}
    }
    return ans;
}
