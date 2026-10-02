#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 矩阵语义：计数用 (+,*)；可达性用 (OR,AND)；最优值用 (max,+) 或 (min,+)。
// 空段单位矩阵：对角为乘法单位，其他为加法零（布尔 0、max-plus -INF、min-plus +INF）。
// 列向量先过 A 再过 B 为 B*A；不能只把普通矩阵的模数改掉。INF 项不得直接相加。
// 扫参数时，仿射式相等的条件只可能恒真/恒假/在唯一 k 成立；在 k 和 k+1 都安排更新。
// 轮廓线仅保留相对标号时，平移已处理前缀 delta 会改变目标 delta*前缀供需差，需结算。
const int N=2005;
const ll NEG=-4e18;
int n,q;
ll w[N];// 点权
vector<int> adj[N];

// ===== 转移矩阵 =====
// g0[u]=轻儿子的 max(f0[v],f1[v]) 之和，g1[u]=w[u]+轻儿子的 f0[v] 之和
// f0[u]=g0[u]+max(f0[重儿子],f1[重儿子])，f1[u]=g1[u]+f0[重儿子]
// 写成矩阵： (f0[u];f1[u]) = [g0 g0; g1 -inf] * (f0[son];f1[son])
// 答案 = max(f0[根],f1[根])
struct Matrix
{
    ll a[2][2];

    Matrix operator*(const Matrix &o)const
    {
        Matrix c;
        for(int i=0;i<2;i++)for(int j=0;j<2;j++)
        {
            c.a[i][j]=NEG;
            for(int k=0;k<2;k++)if(a[i][k]!=NEG&&o.a[k][j]!=NEG)
                c.a[i][j]=max(c.a[i][j],a[i][k]+o.a[k][j]);
        }
        return c;
    }
};

int fa[N],dep[N],sz[N],son[N],head[N],pos_of[N],dfn[N],tim;
ll g0[N],g1[N];
int nchain;
vector<int> chnode[N];// 每条链上的点，顺序：链头 -> 链尾
vector<int> chid[N];// 点 u 在链内的下标（链头是 0）
vector<Matrix> tr[N];

Matrix mt_of(int u)// 点 u 在重链上的转移矩阵
{
    Matrix m;
    m.a[0][0]=g0[u],m.a[0][1]=g0[u];
    m.a[1][0]=g1[u],m.a[1][1]=NEG;
    return m;
}

// 线段树：叶子下标 0 放链尾（最深），下标越大越靠近链头
// 列向量转移要求浅 * 深，倒序叶子的右段矩阵乘左段矩阵
void build_seg(int c,int p,int l,int r)
{
    int len=chnode[c].size();
    if(l==r){tr[c][p]=mt_of(chnode[c][len-1-l]);return;}
    int mid=(l+r)>>1;
    build_seg(c,p<<1,l,mid);
    build_seg(c,p<<1|1,mid+1,r);
    tr[c][p]=tr[c][p<<1|1]*tr[c][p<<1];
}

void modify(int c,int p,int l,int r,int x,Matrix v)
{
    if(l==r){tr[c][p]=v;return;}
    int mid=(l+r)>>1;
    if(x<=mid)modify(c,p<<1,l,mid,x,v);
    else modify(c,p<<1|1,mid+1,r,x,v);
    tr[c][p]=tr[c][p<<1|1]*tr[c][p<<1];
}

Matrix query(int c,int p,int l,int r,int ql,int qr)
{
    if(ql<=l&&r<=qr)return tr[c][p];
    int mid=(l+r)>>1;
    if(qr<=mid)return query(c,p<<1,l,mid,ql,qr);
    if(ql>mid)return query(c,p<<1|1,mid+1,r,ql,qr);
    return query(c,p<<1|1,mid+1,r,ql,qr)*query(c,p<<1,l,mid,ql,qr);
}

// 取点 u 的 f0,f1：查它所在链从 u 到链尾的一段
// 叶子下标 0 是链尾，所以链上 [chid[u], len-1] 对应叶子区间 [0, len-1-chid[u]]
void get_f(int u,ll &f0,ll &f1)
{
    int c=head[u],len=chnode[c].size();
    Matrix m=query(c,1,0,len-1,0,len-1-chid[u][0]);
    f0=m.a[0][0],f1=m.a[1][0];
}

// 第一次 dfs：重儿子、子树大小
void dfs1(int u,int p)
{
    fa[u]=p,dep[u]=dep[p]+1,sz[u]=1,son[u]=0;
    for(int v:adj[u])
    {
        if(v==p)continue;
        dfs1(v,u);
        sz[u]+=sz[v];
        if(sz[v]>sz[son[u]])son[u]=v;
    }
}

// 第二次 dfs：剖分重链，链内按链头->链尾顺序放进 chnode
void dfs2(int u,int h)
{
    head[u]=h;
    dfn[u]=++tim;// 记录链头的访问顺序，用来决定建树顺序
    chid[u].push_back(chnode[h].size());
    chnode[h].push_back(u);
    for(int v:adj[u])
        if(v!=fa[u]&&v!=son[u])dfs2(v,v);
    if(son[u])dfs2(son[u],h);
}

// 预处理：算 g0,g1 并建线段树
void build_all()
{
    dfs1(1,0);
    for(int i=1;i<=n;i++)chnode[i].clear(),chid[i].clear();
    tim=0;
    dfs2(1,1);
    // 按链头被访问的先后顺序倒着处理：保证轻儿子所在链已经算完
    vector<int> corder;
    for(int i=1;i<=n;i++)
        if(chnode[i].size())corder.push_back(i);
    sort(corder.begin(),corder.end(),[&](int x,int y){return dfn[x]>dfn[y];});
    for(int c:corder)
    {
        int L=chnode[c].size();
        for(int x=L-1;x>=0;x--)// 链内从链尾（最深）往链头算
        {
            int u=chnode[c][x];
            g0[u]=0,g1[u]=w[u];
            for(int v:adj[u])
                if(v!=fa[u]&&v!=son[u])
                {
                    ll v0,v1;
                    get_f(v,v0,v1);
                    g0[u]+=max(v0,v1),g1[u]+=v0;
                }
        }
        tr[c].assign(L*4+4,Matrix());
        build_seg(c,1,0,L-1);
    }
    nchain=corder.size();
    for(int u=1;u<=n;u++)pos_of[u]=chnode[head[u]].size()-1-chid[u][0];
}

// 点权修改：沿着重链往上跳，每跳一条链就把这条链的链头 dp 结果并进父结点的轻儿子贡献
void update(int u,ll x)
{
    g1[u]+=x-w[u],w[u]=x;
    while(u)
    {
        int c=head[u],len=chnode[c].size();
        ll old0,old1,new0,new1;
        get_f(c,old0,old1);
        modify(c,1,0,len-1,pos_of[u],mt_of(u));
        get_f(c,new0,new1);
        u=fa[c];
        if(u)
        {
            g0[u]+=max(new0,new1)-max(old0,old1);
            g1[u]+=new0-old0;
        }
    }
}
