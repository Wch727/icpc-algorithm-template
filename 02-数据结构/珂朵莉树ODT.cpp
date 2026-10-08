#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 珂朵莉树(ODT / 颜色段均摊)：set 维护极长同色段
// 区间赋值遍历并删除的段可按生命周期均摊；每次 split 只新增 O(1) 段。
// add/kth/求和等会遍历但不删除段，重复扫很多段可达 O(nq)，有赋值也不保证性能。
// 仅赋值型删段操作有上述确定性均摊；混合操作的随机期望依赖输入分布，不能当最坏保证。
struct Node
{
    int l,r;
    mutable ll v;
    Node(int l,int r,ll v):l(l),r(r),v(v){}
    bool operator<(const Node &o)const{return l<o.l;}
};

int n;
set<Node> odt;

// 把位置 pos 所在的段拆成 [l,pos-1] 和 [pos,r]，返回起点为 pos 的迭代器
auto split(int pos)
{
    if(pos>n)return odt.end();//右端越界，直接返回尾迭代器
    auto it=odt.lower_bound(Node(pos,0,0));
    if(it!=odt.end()&&it->l==pos)return it;
    --it;
    int l=it->l,r=it->r;
    ll v=it->v;
    odt.erase(it);
    odt.insert(Node(l,pos-1,v));
    return odt.insert(Node(pos,r,v)).first;
}

// 区间赋值，O(段数 log n)
void assign(int l,int r,ll v)
{
    auto itr=split(r+1),itl=split(l);
    odt.erase(itl,itr);
    odt.insert(Node(l,r,v));
}

// 区间加，把 l..r 覆盖到的每段整体加上 v
void add(int l,int r,ll v)
{
    auto itr=split(r+1),itl=split(l);
    for(auto it=itl;it!=itr;++it)it->v+=v;
}

// 区间和
ll query_sum(int l,int r)
{
    auto itr=split(r+1),itl=split(l);
    ll s=0;
    for(auto it=itl;it!=itr;++it)s+=(ll)(it->r-it->l+1)*it->v;
    return s;
}

// 区间内等于 v 的个数
int query_cnt(int l,int r,ll v)
{
    auto itr=split(r+1),itl=split(l);
    int c=0;
    for(auto it=itl;it!=itr;++it)
        if(it->v==v)c+=it->r-it->l+1;
    return c;
}

// 区间第 k 小(1<=k<=r-l+1)，把覆盖到的段按值排序后累加长度
ll query_kth(int l,int r,int k)
{
    vector<pair<ll,int>> tmp;
    auto itr=split(r+1),itl=split(l);
    for(auto it= itl; it != itr; ++it)
        tmp.push_back({it->v, it->r - it->l + 1});
    sort(tmp.begin(),tmp.end());
    for(auto &x:tmp)
    {
        if(k<=x.second)return x.first;
        k-=x.second;
    }
    return -1;//k 超过区间长度
}
