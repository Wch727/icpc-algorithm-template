#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 动态森林：维护点权路径和，操作均摊 O(log n)，下标从 1 开始
struct LCT
{
    struct Node { int ch[2]={0,0},fa=0; ll v=0,sum=0; bool rev=false; };
    vector<Node> tr;
    LCT(int n):tr(n+1) {}
    bool is_root(int x)
    {
        int f=tr[x].fa;
        return tr[f].ch[0]!=x&&tr[f].ch[1]!=x;
    }
    void pull(int x) { tr[x].sum=tr[tr[x].ch[0]].sum+tr[tr[x].ch[1]].sum+tr[x].v; }
    void reverse_node(int x)
    {
        if(x)swap(tr[x].ch[0],tr[x].ch[1]),tr[x].rev=!tr[x].rev;
    }
    void push(int x)
    {
        if(tr[x].rev)reverse_node(tr[x].ch[0]),reverse_node(tr[x].ch[1]),tr[x].rev=false;
    }
    void rotate(int x)
    {
        int y=tr[x].fa,z=tr[y].fa,k=tr[y].ch[1]==x,w=tr[x].ch[k^1];
        if(!is_root(y))tr[z].ch[tr[z].ch[1]==y]=x;
        tr[x].fa=z,tr[x].ch[k^1]=y,tr[y].fa=x,tr[y].ch[k]=w;
        if(w)tr[w].fa=y;
        pull(y),pull(x);
    }
    void splay(int x)
    {
        vector<int> st;
        for(int y=x;;y=tr[y].fa)
        {
            st.push_back(y);
            if(is_root(y))break;
        }
        for(int i=(int)st.size()-1;i>=0;i--)push(st[i]);
        while(!is_root(x))
        {
            int y=tr[x].fa,z=tr[y].fa;
            if(!is_root(y))rotate((tr[y].ch[0]==x)==(tr[z].ch[0]==y)?y:x);
            rotate(x);
        }
    }
    void access(int x)
    {
        for(int y=0;x;y=x,x=tr[x].fa)splay(x),tr[x].ch[1]=y,pull(x);
    }
    void makeroot(int x) { access(x),splay(x),reverse_node(x); }
    int findroot(int x)
    {
        access(x),splay(x),push(x);
        while(tr[x].ch[0])x=tr[x].ch[0],push(x);
        splay(x);
        return x;
    }
    bool connected(int x,int y) { return x==y||findroot(x)==findroot(y); }
    bool link(int x,int y)
    {
        makeroot(x);
        if(findroot(y)==x)return false;
        tr[x].fa=y;
        return true;
    }
    bool cut(int x,int y)
    {
        makeroot(x),access(y),splay(y);
        if(tr[y].ch[0]!=x||tr[x].ch[1])return false;
        tr[y].ch[0]=tr[x].fa=0,pull(y);
        return true;
    }
    void set_val(int x,ll v) { access(x),splay(x),tr[x].v=v,pull(x); }
    bool query(int x,int y,ll &sum)
    {
        if(!connected(x,y))return false;
        makeroot(x),access(y),splay(y),sum=tr[y].sum;
        return true;
    }
};
