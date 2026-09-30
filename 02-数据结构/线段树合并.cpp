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
int head[N],to[N<<1],nxt[N<<1],tot_edge;//链式前向星存树

void add_edge(int u,int v)
{
    to[++tot_edge]=v,nxt[tot_edge]=head[u],head[u]=tot_edge;
}

struct MergeSeg{
    int ls[MAXNODE],rs[MAXNODE],sum[MAXNODE],tot;
    void init(){tot=0;}
    int new_node()
    {
        ++tot;
        ls[tot]=rs[tot]=sum[tot]=0;
        return tot;
    }
    // 在位置 pos 插入 cnt 个（叶子），返回根，O(log n)
    int insert(int p,int l,int r,int pos,int cnt)
    {
        if(!p)p=new_node();
        if(l==r){sum[p]+=cnt;return p;}
        int mid=(l+r)>>1;
        if(pos<=mid)ls[p]=insert(ls[p],l,mid,pos,cnt);
        else rs[p]=insert(rs[p],mid+1,r,pos,cnt);
        sum[p]=sum[ls[p]]+sum[rs[p]];
        return p;
    }
    // 把 b 合并进 a，返回合并后的根，均摊 O(log n)
    int merge(int a,int b)
    {
        if(!a||!b)return a|b;       // 有一边空，直接接过去；b 空则只剩 a
        ls[a]=merge(ls[a],ls[b]);
        rs[a]=merge(rs[a],rs[b]);
        sum[a]=sum[ls[a]]+sum[rs[a]];
        return a;
    }
    // 不同的颜色数 = 权值线段树里 sum>0 的叶子个数 = 根上的 sum（每个叶子最多是 1）
    int count_distinct(int a){return sum[a];}
}seg;

int ans[N];//每个子树内不同颜色数
int root[N];//每个点对应的权值线段树根

// 自底向上：把自己的颜色建树，再把每个儿子的树合并进来，最后统计根上的 sum
void dfs(int u,int fa)
{
    root[u]=seg.insert(0,1,n,col[u],1);
    for(int e=head[u];e;e=nxt[e])
    {
        int v=to[e];
        if(v==fa)continue;
        dfs(v,u);
        root[u]=seg.merge(root[u],root[v]);
    }
    ans[u]=seg.count_distinct(root[u]);
}

// 暴力：对每个点搜一遍子树，用桶统计颜色
int vis[N];
int bhead[N],bto[N<<1],bnxt[N<<1],btot;
void badd(int u,int v){bto[++btot]=v,bnxt[btot]=bhead[u],bhead[u]=btot;}

int main()
{
    srand(20240513);

    // 1. 小样例：链 1-2-3-4-5，颜色 1 2 1 3 2
    //    子树颜色集：{1,2,3}=3、{2,1,3}=3、{1,3,2}=3、{3,2}=2、{2}=1
    n=5;
    int ini[]={0,1,2,1,3,2};
    for(int i=1;i<=n;i++)col[i]=ini[i];
    tot_edge=0;
    for(int i=1;i<=n;i++)head[i]=0;
    for(int i=1;i<n;i++)add_edge(i,i+1),add_edge(i+1,i);
    seg.init();
    dfs(1,0);
    printf("小样例(链): ans[1..5] =");
    for(int i=1;i<=n;i++)printf(" %d",ans[i]);
    printf("  (应为 3 3 3 2 1)\n");

    // 2. 对拍：随机树 + 随机颜色，与暴力 dfs 比较
    bool ok=true;
    for(int T=1;T<=30&&ok;T++)
    {
        n=rand()%60+1;
        int C=rand()%5+1;
        for(int i=1;i<=n;i++)col[i]=rand()%C+1;
        tot_edge=0,btot=0;
        for(int i=1;i<=n;i++)head[i]=0,bhead[i]=0;
        for(int i=2;i<=n;i++)      // 随机父结点，保证是棵树
        {
            int fa=rand()%(i-1)+1;
            add_edge(fa,i),add_edge(i,fa);
            badd(fa,i),badd(i,fa);
        }
        seg.init();
        dfs(1,0);
        // 暴力：每个点向下搜子树
        for(int r=1;r<=n;r++)
        {
            for(int i=1;i<=C;i++)vis[i]=0;
            int cnt=0;
            vector<int> st;st.push_back(r);
            vector<int> par(n+1,0);
            while(!st.empty())
            {
                int u=st.back();st.pop_back();
                if(!vis[col[u]])vis[col[u]]=1,cnt++;
                for(int e=bhead[u];e;e=bnxt[e])
                {
                    int v=bto[e];
                    if(v==par[u])continue;
                    par[v]=u;
                    st.push_back(v);
                }
            }
            if(cnt!=ans[r])
            {
                printf("第 %d 轮错: 点 %d got=%d want=%d\n",T,r,ans[r],cnt);
                ok=false;
                break;
            }
        }
        printf("线段树合并第 %d 轮 %s (结点数=%d)\n",T,ok?"passed":"FAILED",seg.tot);
    }

    // 3. 菊花图：根挂 n-1 个叶子，每个叶子一种颜色 -> 根答案是 n-1
    n=8;
    for(int i=1;i<=n;i++)col[i]=i;
    tot_edge=0;
    for(int i=1;i<=n;i++)head[i]=0;
    for(int i=2;i<=n;i++)add_edge(1,i),add_edge(i,1);
    seg.init();
    dfs(1,0);
    printf("菊花图: ans[1]=%d (应=%d) ans[2]=%d (应=1)\n",ans[1],n-1,ans[2]);

    // 4. 规模测试：n=100000 的随机树，全是 1 种颜色 -> 所有答案都是 1
    n=100000;
    for(int i=1;i<=n;i++)col[i]=1;
    tot_edge=0;
    for(int i=1;i<=n;i++)head[i]=0;
    for(int i=2;i<=n;i++)
    {
        int fa=(int)(rand()%(i-1))+1;
        add_edge(fa,i),add_edge(i,fa);
    }
    seg.init();
    dfs(1,0);
    int bad=0;
    for(int i=1;i<=n;i++)if(ans[i]!=1)bad++;
    printf("规模: n=100000 单色 -> 错 %d 个, 结点数 %d\n",bad,seg.tot);
    printf("结果: %s\n",(ok&&bad==0)?"OK":"FAILED");
    return 0;
}
