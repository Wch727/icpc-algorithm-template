#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 珂朵莉树(ODT / 颜色段均摊)：set 维护极长同色段
// 区间赋值时把中间段全删掉，段数均摊下降，随机数据下近似 O((n+m) log n)
// 注意：必须保证有区间赋值操作，否则段数不收敛会退化成暴力
struct Node
{
    int l,r;
    mutable ll v;
    Node(int l,int r,ll v):l(l),r(r),v(v){}
    bool operator<(const Node &o)const
    {
        return l<o.l;
    }
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
    for(auto it=itl;it!=itr;++it)tmp.push_back({it->v,it->r-it->l+1});
    sort(tmp.begin(),tmp.end());
    for(auto &x:tmp)
    {
        if(k<=x.second)return x.first;
        k-=x.second;
    }
    return -1;//k 超过区间长度
}

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

ll a[105],br[105];

// 自测：赋值/加/和/计数/第k小全部与暴力数组对拍
int main()
{
    srand(19260817);
    ll bad=0,cnt=0;
    for(int t=1;t<=30;t++)
    {
        n=rnd(1,40);
        odt.clear();
        for(int i=1;i<=n;i++)a[i]=rnd(1,5),br[i]=a[i],odt.insert(Node(i,i,a[i]));
        for(int q=1;q<=200;q++)
        {
            int op=rnd(1,5),l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(op==1)
            {
                ll v=rnd(1,5);
                assign(l,r,v);
                for(int i=l;i<=r;i++)br[i]=v;
            }
            else if(op==2)
            {
                ll v=rnd(-5,5);
                add(l,r,v);
                for(int i=l;i<=r;i++)br[i]+=v;
            }
            else if(op==3)
            {
                ll x=query_sum(l,r),z=0;
                for(int i=l;i<=r;i++)z+=br[i];
                cnt++;
                if(x!=z)bad++;
            }
            else if(op==4)
            {
                ll v=rnd(1,5);
                int x=query_cnt(l,r,v),z=0;
                for(int i=l;i<=r;i++)
                    if(br[i]==v)z++;
                cnt++;
                if(x!=z)bad++;
            }
            else
            {
                int k=rnd(1,r-l+1);
                vector<ll> tmp;
                for(int i=l;i<=r;i++)tmp.push_back(br[i]);
                sort(tmp.begin(),tmp.end());
                cnt++;
                if(query_kth(l,r,k)!=tmp[k-1])bad++;
            }
        }
    }
    printf("珂朵莉树ODT vs 暴力: %s, 校验=%lld, 错=%lld\n",bad?"FAIL":"OK",cnt,bad);
    // 小样例：[1,5] 全赋 2，再给 [2,4] 加 10，求和/计数/第2小
    n=5;
    odt.clear();
    odt.insert(Node(1,n,1));
    assign(1,5,2);
    add(2,4,10);
    printf("小样例: sum[1,5]=%lld cnt(1,5,2)=%d kth(1,5,2)=%lld\n",query_sum(1,5),query_cnt(1,5,2),query_kth(1,5,2));
    return 0;
}
