#include<bits/stdc++.h>
using namespace std;

// 无向无标号树，vector 邻接表，点 1..n；输入必须为合法树，空树用长度 1 的 adj。
// 根的类型=排序后的子树类型列表；共享字典精确编号，没有随机哈希碰撞。
// 无根树从重心取根，最多两个；O(n log n)，空间 O(n)，遍历用栈避免长链递归。
vector<int> tree_centroids(const vector<vector<int>> &adj)
{
    int n=(int)adj.size()-1;
    if(!n)
        return {};
    vector<int> fa(n+1),sz(n+1,1),order{1},ans;
    for(int i=0;i<(int)order.size();i++)
    {
        int u=order[i];
        for(int v : adj[u])
            if(v != fa[u])
                fa[v]= u, order.push_back(v);
    }
    for(int i=n-1;i>=0;i--)
    {
        int u=order[i],mx=0;
        for(int v : adj[u])
            if(v != fa[u])
                sz[u]+= sz[v], mx= max(mx, sz[v]);
        if(max(mx,n-sz[u])<=n/2)ans.push_back(u);
    }
    return ans;
}

int tree_code(const vector<vector<int>> &adj,int root,map<vector<int>,int> &ids)
{
    int n=(int)adj.size()-1;
    vector<int> fa(n+1),code(n+1),order{root};
    for(int i=0;i<n;i++)
    {
        int u=order[i];
        for(int v : adj[u])
            if(v != fa[u])
                fa[v]= u, order.push_back(v);
    }
    for(int i=n-1;i>=0;i--)
    {
        int u=order[i];
        vector<int> children;
        for(int v : adj[u])
            if(v != fa[u])
                children.push_back(code[v]);
        sort(children.begin(),children.end());
        auto [it,inserted]=ids.emplace(move(children),ids.size()+1);
        code[u]=it->second;
    }
    return code[root];
}

bool same_tree(const vector<vector<int>> &a,const vector<vector<int>> &b)
{
    assert(!a.empty()&&!b.empty());
    if(a.size()!=b.size())return false;
    if(a.size()==1)return true;
    auto ca=tree_centroids(a),cb=tree_centroids(b);
    if(ca.size()!=cb.size())return false;
    map<vector<int>,int> ids;
    int x=tree_code(a,ca[0],ids);
    for(int root : cb)
        if(tree_code(b, root, ids) == x)
            return true;
    return false;
}
