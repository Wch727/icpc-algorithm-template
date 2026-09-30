// 动态开点线段树：值域很大（1e9 级）时只开用到的结点
// 支持区间加 / 区间和，单次操作 O(log V)，V 是值域
// 空间：每次修改最多新建 O(log V) 个点，m 次修改共 O(m log V)
// 结点用 vector 按需 push_back（也可以开静态数组池，见注释）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct DynSeg{
    struct Node{
        int ls,rs;      // 左右儿子下标，0 表示没建出来（等价于整棵空树）
        ll sum,lazy;    // 区间和、加的懒标记
    };
    vector<Node> t;     // t[0] 是空结点哨兵，sum/lazy 都是 0
    ll lo,hi;           // 值域（闭区间），如 1..1e9
    DynSeg(ll lo_=1,ll hi_=1000000000){init(lo_,hi_);}
    void init(ll lo_,ll hi_)
    {
        lo=lo_,hi=hi_;
        t.clear();
        t.push_back(Node{0,0,0,0});//哨兵 t[0]，空结点
        t.push_back(Node{0,0,0,0});//根 t[1]，整棵树一开始只有一个根
    }
    int new_node()
    {
        t.push_back(Node{0,0,0,0});
        return (int)t.size()-1;
    }
    // p 的标记发给两个儿子，儿子不存在就建出来
    // 注意：t 是 vector，push_back 会重新分配内存，不能提前把 t[p] 取成引用
    void push_down(int p,ll l,ll r)
    {
        if(t[p].lazy==0||l==r)return;
        ll v=t[p].lazy,mid=(l+r)>>1;
        if(!t[p].ls)t[p].ls=new_node();
        if(!t[p].rs)t[p].rs=new_node();
        int L=t[p].ls,R=t[p].rs;        // 建完点之后再取，下标不会失效
        t[L].lazy+=v,t[L].sum+=v*(mid-l+1);
        t[R].lazy+=v,t[R].sum+=v*(r-mid);
        t[p].lazy=0;
    }
    void add(ll L,ll R,ll v){add(L,R,v,lo,hi,1);}
    // 区间 [L,R] 加 v，O(log V)
    void add(ll L,ll R,ll v,ll l,ll r,int p)
    {
        if(L<=l&&r<=R)//(l,r)<=(L,R)
        {
            t[p].sum+=v*(r-l+1),t[p].lazy+=v;
            return;
        }
        push_down(p,l,r);
        ll mid=(l+r)>>1;
        if(L<=mid)
        {
            if(!t[p].ls)t[p].ls=new_node();
            add(L,R,v,l,mid,t[p].ls);
        }
        if(R>mid)
        {
            if(!t[p].rs)t[p].rs=new_node();
            add(L,R,v,mid+1,r,t[p].rs);
        }
        t[p].sum=t[t[p].ls].sum+t[t[p].rs].sum;
    }
    ll sum(ll L,ll R){return sum(L,R,lo,hi,1);}
    // 区间 [L,R] 求和，O(log V)；结点没建出来说明这段全是 0
    ll sum(ll L,ll R,ll l,ll r,int p)
    {
        if(!p)return 0;             // 空结点直接返回 0，不用再往下走
        if(L<=l&&r<=R)return t[p].sum;
        push_down(p,l,r);
        ll mid=(l+r)>>1,res=0;
        if(L<=mid)res+=sum(L,R,l,mid,t[p].ls);
        if(R>mid)res+=sum(L,R,mid+1,r,t[p].rs);
        return res;
    }
    // 单点加就是 add(x,x,v)
    void point_add(ll x,ll v){add(x,x,v);}
    int nodes(){return (int)t.size()-1;}//已用结点数
};

// 静态数组池写法（赛场上省 vector 开销），容量按 m*log V 估：
// const int MAXNODE=2000005;
// int ls[MAXNODE],rs[MAXNODE];ll sum[MAXNODE],lazy[MAXNODE];int tot;
// int new_node(){++tot;ls[tot]=rs[tot]=sum[tot]=lazy[tot]=0;return tot;}

const ll V=1000000000;//值域 1..1e9
const int XN=4005;    // 对拍用到的坐标个数
