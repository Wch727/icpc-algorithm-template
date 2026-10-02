#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

// 小根可并堆：root 是节点编号，0 表示空；merge 后旧根不能再当独立堆使用。
// 两个待合并堆必须没有共享节点；相同权值允许，节点删除后不回收。
// merge 沿右链递归，右子树空路径长不大于左边；合并/插入/删顶 O(log n)。
struct LeftistHeap
{
    struct Node{ll val=0;int l=0,r=0,d=0;};
    vector<Node> tr=vector<Node>(1);
    void clear(){tr.assign(1,Node{});}
    int merge(int x,int y)
    {
        if(!x||!y)return x|y;
        assert(x!=y);
        if(tr[x].val>tr[y].val)swap(x,y);
        tr[x].r=merge(tr[x].r,y);
        if(tr[tr[x].l].d<tr[tr[x].r].d)swap(tr[x].l,tr[x].r);
        tr[x].d=tr[tr[x].r].d+1;
        return x;
    }
    int insert(int root,ll x)
    {
        int p=tr.size();
        tr.push_back({x,0,0,1});
        return merge(root,p);
    }
    ll top(int root){assert(root);return tr[root].val;}
    int pop(int root)
    {
        assert(root);
        int p=merge(tr[root].l,tr[root].r);
        tr[root].l=tr[root].r=0,tr[root].d=1;
        return p;
    }
};
