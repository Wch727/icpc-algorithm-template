#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 替罪羊树：重复值计数，alpha 重构，单次操作均摊 O(log n)
struct Scapegoat
{
    struct Node { int l=0,r=0,v=0,cnt=0,sz=0,tot=0; };
    vector<Node> tr;
    int rt=0;
    Scapegoat() { tr.push_back(Node()); }
    void pull(int p)
    {
        tr[p].sz=tr[tr[p].l].sz+tr[tr[p].r].sz+tr[p].cnt;
        tr[p].tot=tr[tr[p].l].tot+tr[tr[p].r].tot+1;
    }
    void collect(int p,vector<int> &a)
    {
        if(!p)return;
        collect(tr[p].l,a);
        if(tr[p].cnt)a.push_back(p);
        collect(tr[p].r,a);
    }
    int build(vector<int> &a,int l,int r)
    {
        if(l>r)return 0;
        int mid=(l+r)>>1,p=a[mid];
        tr[p].l=build(a,l,mid-1),tr[p].r=build(a,mid+1,r);
        pull(p);
        return p;
    }
    int rebuild(int p)
    {
        vector<int> a;
        collect(p,a);
        return build(a,0,(int)a.size()-1);
    }
    int change(int p,int x,int d)
    {
        if(!p)
        {
            if(d<0)return 0;
            Node u;
            u.v=x,u.cnt=u.sz=u.tot=1;
            tr.push_back(u);
            return (int)tr.size()-1;
        }
        if(x==tr[p].v)tr[p].cnt=max(0,tr[p].cnt+d);
        else if(x<tr[p].v)tr[p].l=change(tr[p].l,x,d);
        else tr[p].r=change(tr[p].r,x,d);
        pull(p);
        // 用不同键数衡量结构重量，删除后清除过多墓碑
        int w=max(tr[tr[p].l].tot,tr[tr[p].r].tot);
        if(w*4>tr[p].tot*3||tr[p].sz*2<tr[p].tot)p=rebuild(p);
        return p;
    }
    void insert(int x) { rt=change(rt,x,1); }
    void erase(int x) { rt=change(rt,x,-1); }
    int rank(int x)
    {
        int p=rt,ans=1;
        while(p)
        {
            if(x<=tr[p].v)p=tr[p].l;
            else ans+=tr[tr[p].l].sz+tr[p].cnt,p=tr[p].r;
        }
        return ans;
    }
    bool kth(int k,int &x)
    {
        if(k<1||k>tr[rt].sz)return false;
        int p=rt;
        while(p)
        {
            int s=tr[tr[p].l].sz;
            if(k<=s)p=tr[p].l;
            else if(k <= s + tr[p].cnt)
            {
                x= tr[p].v;
                return true;
            }
            else k-=s+tr[p].cnt,p=tr[p].r;
        }
        return false;
    }
    bool prev(int x,int &v) { return kth(rank(x)-1,v); }
    bool next(int x,int &v)
    {
        int p=rt,k=0;
        while(p)
        {
            if(tr[p].v<=x)k+=tr[tr[p].l].sz+tr[p].cnt,p=tr[p].r;
            else p=tr[p].l;
        }
        return kth(k+1,v);
    }
};
