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
        t.push_back(Node{0,0,0,0});//哨兵
    }
    int new_node()
    {
        t.push_back(Node{0,0,0,0});
        return (int)t.size()-1;
    }
    // p 的标记发给两个儿子，儿子不存在就建出来
    void push_down(int p,ll l,ll r)
    {
        if(t[p].lazy==0||l==r)return;
        ll v=t[p].lazy,mid=(l+r)>>1;
        if(!t[p].ls)t[p].ls=new_node();
        if(!t[p].rs)t[p].rs=new_node();
        int L=t[p].ls,R=t[p].rs;
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

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

ll xs[XN];

// 暴力：坐标只取采样点，用差分求每个采样区间的值
// 先把 <L 的差分全部吃掉得到 cur，再从左端点开始逐段累加、到 R 就停
ll brute(int k,map<ll,ll> &df,ll L,ll R)
{
    ll res=0,cur=0;
    for(auto &pr:df)if(pr.first<L)cur+=pr.second;
    for(int i=1;i<=k;i++)
    {
        if(xs[i]<L){if(df.count(xs[i]))cur+=df[xs[i]];continue;}
        if(xs[i]>R)break;
        if(df.count(xs[i]))cur+=df[xs[i]];
        ll a=xs[i],b=min(R,i+1<=k?xs[i+1]-1:R);
        res+=cur*(b-a+1);
    }
    return res;
}

int main()
{
    srand(20240513);

    // 1. 小样例（值域 1..1e9，只用到 1..8 这几个位置）
    DynSeg ds(1,V);
    ds.add(5,8,10);
    ds.add(1,5,1);
    printf("小样例: sum[1,4]=%lld sum[5,5]=%lld sum[6,8]=%lld sum[1,1e9]=%lld\n",
        ds.sum(1,4),ds.sum(5,5),ds.sum(6,8),ds.sum(1,V));

    // 2. 对拍：大值域随机区间加 / 随机区间和 vs 差分暴力
    bool ok=true;
    for(int T=1;T<=10&&ok;T++)
    {
        int m=rnd(1,300),k=0;
        for(int i=1;i<=m;i++)      // 采样 2m 个坐标，落在 1..1e9
        {
            xs[++k]=rnd(1,1000000000);
            xs[++k]=rnd(1,1000000000);
        }
        sort(xs+1,xs+k+1);
        k=unique(xs+1,xs+k+1)-xs-1;
        DynSeg seg(1,V);
        map<ll,ll> df;
        for(int q=1;q<=400;q++)
        {
            int op=rnd(1,2),i=rnd(1,k),j=rnd(1,k);
            if(i>j)swap(i,j);
            ll L=xs[i],R=xs[j];
            if(op==1)
            {
                ll v=rnd(-100,100);
                seg.add(L,R,v);
                df[L]+=v,df[R+1]-=v;
            }
            else
            {
                ll got=seg.sum(L,R),want=brute(k,df,L,R);
                if(got!=want)
                {
                    printf("第 %d 轮错: [%lld,%lld] got=%lld want=%lld\n",T,L,R,got,want);
                    ok=false;
                    break;
                }
            }
        }
        printf("动态开点第 %d 轮 %s (结点数=%d)\n",T,ok?"passed":"FAILED",seg.nodes());
    }

    // 3. 规模测试：2e5 次修改，看结点数是不是 O(m log V)
    DynSeg big(1,V);
    for(int i=1;i<=200000;i++)
    {
        ll L=(ll)(rand()%1000000)*1000+1;
        big.add(L,L+5000,1);
    }
    printf("规模: 200000 次修改 -> 结点 %d (上界 200000*31)\n",big.nodes());
    printf("结果: %s\n",ok?"OK":"FAILED");
    return 0;
}
