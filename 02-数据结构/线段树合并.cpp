// 线段树合并（权值线段树合并）：求每个子树内不同颜色数 / 每个权值出现次数
// 建树 O(n log n)；合并总复杂度 O(结点总数) = O(n log n)
// 关键性质：merge 里只要有一边是空就直接返回另一边，
//           否则两边都不空 -> 当前这个结点被丢弃，每个结点一生只会被丢一次，
//           所以所有 merge 加起来是 O(总结点数)，均摊到一次合并是 O(log n)
// 坑：合并是「破坏性」的，merge(a,b) 之后 b 的子结点已经被挂到 a 上，b 不能再单独用
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int LOG=18;      // log2(N)+1
const int MAXNODE=N*(LOG+1);//n 个叶子各建一条到根的链

int n,col[N];
vector<int> adj[N];

void add_edge(int u,int v)
{
    adj[u].push_back(v);
}

struct MergeSeg{
    int ls[MAXNODE],rs[MAXNODE],sum[MAXNODE],cnt[MAXNODE],tot;//cnt 是这段里被占用的叶子数
    void init(){tot=0;}
    int new_node()
    {
        ++tot;
        ls[tot]=rs[tot]=sum[tot]=cnt[tot]=0;
        return tot;
    }
    // 在位置 pos 插入 cnt 个（叶子），返回根，O(log n)
    int insert(int p,int l,int r,int pos,int c)
    {
        if(!p)p=new_node();
        if(l==r)
        {
            if(sum[p]==0)cnt[p]=1;//这个叶子原来没有颜色，现在有了，占用数 +1
            sum[p]+=c;
            return p;
        }
        int mid=(l+r)>>1;
        if(pos<=mid)ls[p]=insert(ls[p],l,mid,pos,c);
        else rs[p]=insert(rs[p],mid+1,r,pos,c);
        sum[p]=sum[ls[p]]+sum[rs[p]];
        cnt[p]=cnt[ls[p]]+cnt[rs[p]];
        return p;
    }
    // 把 b 合并进 a，返回合并后的根，均摊 O(log n)；要求 a、b 表示的颜色集合不交
    // cnt 是「这段里被占用的叶子数（不同颜色数）」
    // 关键：递归会把子树的 cnt 改掉，所以先把 ca、cb 和两个儿子的原值存下来，
    //       再用「合并前两边之和 - 合并后的交集叶子数」推出共有几种颜色
    int merge(int a,int b)
    {
        if(!a||!b)return a|b;       // 有一边空，直接接过去；b 空则只剩 a
        int ca=cnt[a],cb=cnt[b];
        int la=ls[a],lb=ls[b],ra=rs[a],rb=rs[b];
        if(!la&&!lb&&!ra&&!rb)      // 两个都是叶子结点（同一个颜色），并成一个
        {
            sum[a]+=sum[b],cnt[a]=1;
            return a;
        }
        int cla=cnt[la],clb=cnt[lb],cra=cnt[ra],crb=cnt[rb];
        int mla=merge(la,lb),mra=merge(ra,rb);
        ls[a]=mla,rs[a]=mra;
        int dup=(cla+clb-cnt[mla])+(cra+crb-cnt[mra]);//两边都有的颜色数
        sum[a]=sum[mla]+sum[mra];
        cnt[a]=ca+cb-dup;
        return a;
    }
    // 不同颜色数 = 权值线段树里 sum>0 的叶子个数，直接返回维护好的 cnt
    int count_distinct(int a){return cnt[a];}
}seg;

int ans[N];//每个子树内不同颜色数
int root[N];//每个点对应的权值线段树根

// 自底向上：把自己的颜色建树，再把每个儿子的树合并进来，最后统计根上的 sum
void dfs(int u,int fa)
{
    root[u]=seg.insert(0,1,n,col[u],1);
    for(int v:adj[u])
    {
        if(v==fa)continue;
        dfs(v,u);
        root[u]=seg.merge(root[u],root[v]);
    }
    ans[u]=seg.count_distinct(root[u]);
}
