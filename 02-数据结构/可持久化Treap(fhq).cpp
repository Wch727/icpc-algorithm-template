#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 隐式无旋 Treap，路径复制；版本根永不原地修改
struct Treap
{
    struct Node
    {
        int lc=0,rc=0,sz=0;
        unsigned key=0;
        ll val=0,sum=0,add=0;
        bool rev=false;
    };
    vector<Node> tr={Node()};
    mt19937 rng{19260817};
    int clone(int p)
    {
        if(!p)return 0;
        Node z=tr[p];
        tr.push_back(z);
        return (int)tr.size()-1;
    }
    int new_node(ll v)
    {
        Node z;
        z.sz=1,z.val=z.sum=v,z.key=rng();
        tr.push_back(z);
        return (int)tr.size()-1;
    }
    void push_up(int p)
    {
        int l=tr[p].lc,r=tr[p].rc;
        tr[p].sz=tr[l].sz+tr[r].sz+1;
        tr[p].sum=tr[l].sum+tr[r].sum+tr[p].val;
    }
    void set_add(int p,ll v)
    {
        if(p)tr[p].val+=v,tr[p].sum+=v*tr[p].sz,tr[p].add+=v;
    }
    void set_rev(int p)
    {
        if(p)swap(tr[p].lc,tr[p].rc),tr[p].rev=!tr[p].rev;
    }
    // p 已复制，带标记的儿子必须先复制再下推
    void push_down(int p)
    {
        if(!tr[p].add&&!tr[p].rev)return;
        int l=clone(tr[p].lc),r=clone(tr[p].rc);
        tr[p].lc=l,tr[p].rc=r;
        set_add(l,tr[p].add),set_add(r,tr[p].add);
        if(tr[p].rev)set_rev(l),set_rev(r);
        tr[p].add=0,tr[p].rev=false;
    }
    // 按前 k 个元素分裂，允许 k=0 或整段长度
    pair<int,int> split(int p,int k)
    {
        if(!p)return {0,0};
        if(k<=0)return {0,p};
        if(k>=tr[p].sz)return {p,0};
        p=clone(p),push_down(p);
        int s=tr[tr[p].lc].sz;
        if(k<=s)
        {
            pair<int,int> z=split(tr[p].lc,k);
            tr[p].lc=z.second,push_up(p);
            return {z.first,p};
        }
        pair<int,int> z=split(tr[p].rc,k-s-1);
        tr[p].rc=z.first,push_up(p);
        return {p,z.second};
    }
    int merge(int a,int b)
    {
        if(!a||!b)return a?a:b;
        if(tr[a].key<tr[b].key)
        {
            a=clone(a),push_down(a);
            tr[a].rc=merge(tr[a].rc,b),push_up(a);
            return a;
        }
        b=clone(b),push_down(b);
        tr[b].lc=merge(a,tr[b].lc),push_up(b);
        return b;
    }
    // 在前 k 个元素之后插入
    int insert(int p,int k,ll v)
    {
        assert(0<=k&&k<=tr[p].sz);
        pair<int,int> z=split(p,k);
        return merge(merge(z.first,new_node(v)),z.second);
    }
    int erase(int p,int k)
    {
        assert(1<=k&&k<=tr[p].sz);
        pair<int,int> z=split(p,k-1),w=split(z.second,1);
        return merge(z.first,w.second);
    }
    int update(int p,int l,int r,ll v,bool rev)
    {
        assert(1<=l&&l<=r&&r<=tr[p].sz);
        pair<int,int> z=split(p,l-1),w=split(z.second,r-l+1);
        int b=clone(w.first);
        set_add(b,v);
        if(rev)set_rev(b);
        return merge(merge(z.first,b),w.second);
    }
    // 只读查询，携带祖先标记，不产生新节点
    ll query(int p,int l,int r,ll add=0,bool rev=false)
    {
        if(!p||l>r)return 0;
        if(l==1&&r==tr[p].sz)return tr[p].sum+add*tr[p].sz;
        int a=rev?tr[p].rc:tr[p].lc,b=rev?tr[p].lc:tr[p].rc,s=tr[a].sz;
        ll ans=0,v=add+tr[p].add;
        bool f=rev^tr[p].rev;
        if(l<=s)ans+=query(a,l,min(r,s),v,f);
        if(l<=s+1&&s+1<=r)ans+=tr[p].val+add;
        if(r>s+1)ans+=query(b,max(1,l-s-1),r-s-1,v,f);
        return ans;
    }
    ll range_sum(int p,int l,int r)
    {
        assert(1<=l&&l<=r&&r<=tr[p].sz);
        return query(p,l,r);
    }
};
