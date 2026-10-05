// 树上启发式合并（DSU on tree / 小并大）
// 例题：每棵子树内不同颜色数 color_cnt[u]、出现次数最多的颜色的出现次数 color_mx[u]
// 复杂度 O(n log n)（重儿子保留贡献，轻儿子暴力重算）
// 预处理 dfs 序、子树大小、重儿子都是迭代的；get_cnt 也用显式栈写，链状数据不爆栈
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n;
vector<int> adj[N];
int col[N],fa_[N],siz[N],heavy[N],in_[N],out_[N],rnk_[N],timer_;
int cnt[N],color_cnt[N],color_mx[N];// cnt：颜色当前出现次数
int cur_sum,cur_mx,touched,added[N];// touched/added：记录碰过的颜色，清空时只清这些

void add_edge(int u,int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

// 迭代求 dfs 序 / 子树大小 / 重儿子，O(n)
void get_order(int rt)
{
    int tl=0,order[N],stk[N];
    fa_[rt]=0,timer_=0;
    int tp=0;
    stk[++tp]=rt;
    while(tp)// 第一次：只求父亲和顺序
    {
        int u=stk[tp--];
        in_[u]=++timer_,rnk_[timer_]=u,order[++tl]=u;
        for(int v:adj[u])
        {
            if(v==fa_[u])continue;
            fa_[v]=u,stk[++tp]=v;
        }
    }
    for(int i=tl;i>=1;i--)// 逆序：子树大小与重儿子
    {
        int u=order[i];
        siz[u]=1,heavy[u]=0;
        for(int v:adj[u])
        {
            if(v==fa_[u])continue;
            siz[u]+=siz[v];
            if(siz[v]>siz[heavy[u]])heavy[u]=v;
        }
        out_[u]=in_[u]+siz[u]-1;// 子树是 dfs 序上一段区间
    }
}

void update(int u)// 把 u 这个点的颜色统计进来，O(1)
{
    int c=col[u];
    if(cnt[c]==0)cur_sum++;
    cnt[c]++;
    if(cnt[c]>cur_mx)cur_mx=cnt[c];
    if(cnt[c]==1)added[touched++]=c;
}

// 迭代版 DSU on tree（用显式栈模拟上面那个递归，长链也不爆栈）
// 状态机：0 先处理轻儿子(keep=0) → 1 处理重儿子(keep=1) → 2 暴力加轻儿子+自己 → 3 收尾
// 递归版见注释里的 get_cnt_rec，逻辑完全一样
void get_cnt(int root,int keep)
{
    int st[N],state[N],saved[N],tp=0;
    for(int i=0;i<touched;i++)cnt[added[i]]=0;
    touched=cur_sum=cur_mx=0;
    st[++tp]=root,state[tp]=0,saved[tp]=keep;
    while(tp)
    {
        int u=st[tp],s=state[tp];
        if(s==0)
        {
            state[tp]=1;
            for(int v:adj[u])
            {
                if(v==fa_[u]||v==heavy[u])continue;
                st[++tp]=v,state[tp]=0,saved[tp]=0;// 轻儿子：算完丢掉
            }
        }
        else if(s==1)
        {
            state[tp]=2;
            if(heavy[u])st[++tp]=heavy[u],state[tp]=0,saved[tp]=1;// 重儿子：贡献保留
        }
        else if(s==2)
        {
            state[tp]=3;
            for(int v:adj[u])
            {
                if(v==fa_[u]||v==heavy[u])continue;
                for(int j=in_[v];j<=out_[v];j++)update(rnk_[j]);// 轻儿子暴力加
            }
            update(u);
            color_cnt[u]=cur_sum,color_mx[u]=cur_mx;// 此刻桶里正好是子树 u
        }
        else
        {
            if(!saved[tp])
            {
                for(int i=0;i<touched;i++)cnt[added[i]]=0;// 只清碰过的颜色
                touched=0,cur_sum=0,cur_mx=0;
            }
            tp--;
        }
    }
}

/* 递归版（n 小时更好看懂，链状数据会爆栈，正式用上面的迭代版）
void get_cnt_rec(int u,int keep)
{
    for(int v:adj[u])
        if(v!=fa_[u]&&v!=heavy[u])get_cnt_rec(v,0);
    if(heavy[u])get_cnt_rec(heavy[u],1);
    for(int v:adj[u])
        if(v!=fa_[u]&&v!=heavy[u])
            for(int j=in_[v];j<=out_[v];j++)update(rnk_[j]);
    update(u);
    color_cnt[u]=cur_sum,color_mx[u]=cur_mx;
    if(!keep)
    {
        for(int i=0;i<touched;i++)cnt[added[i]]=0;
        touched=0,cur_sum=0,cur_mx=0;
    }
}
*/
