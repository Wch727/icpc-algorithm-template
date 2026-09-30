// 动态DP 的测试与对拍代码
// 模板本体：06-动态规划/动态DP.cpp
#include "../../06-动态规划/动态DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

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

ll intree(int u,int p,int mode)// mode=1 表示 u 选，0 表示不选
{
    ll res=mode?w[u]:0;
    for(int v:adj[u])
    {
        if(v==p)continue;
        if(mode)res+=intree(v,u,0);
        else res+=max(intree(v,u,0),intree(v,u,1));
    }
    return res;
}

ll brute_ans()
{
    // 独立暴力枚举点集，不复用树形转移
    ll ans=0;
    for(int mask=0;mask<(1<<n);mask++)
    {
        ll sum=0;
        bool ok=true;
        for(int u=1;u<=n;u++)if(mask>>(u-1)&1)
        {
            sum+=w[u];
            for(int v:adj[u])if(mask>>(v-1)&1)ok=false;
        }
        if(ok)ans=max(ans,sum);
    }
    return ans;
}

int main()
{
    srand(20240625);
    printf("==== 固定样例 ====\n");
    // 原期望 10 写错：相邻的点 2、3 不能同选，枚举子集得 6
    n=4;
    for(int i=1;i<=n;i++)adj[i].clear();
    int e1[3][2]={{1,2},{2,3},{3,4}};
    for(int i=0;i<3;i++)
    {
        int u=e1[i][0],v=e1[i][1];
        adj[u].push_back(v),adj[v].push_back(u);
    }
    w[1]=1,w[2]=5,w[3]=5,w[4]=1;
    build_all();
    ll f0,f1;get_f(1,f0,f1);
    printf("链 1-2-3-4 权 1 5 5 1 : %lld (期望 6)\n",max(f0,f1));
    update(2,100);
    get_f(1,f0,f1);
    printf("把点 2 权改成 100 后 : %lld (期望 101 = 100+1)\n",max(f0,f1));

    printf("==== 随机对拍（含随机修改）====\n");
    int bad=0;
    for(int tt=1;tt<=60;tt++)
    {
        n=rndint(1,10);
        for(int i=1;i<=n;i++)adj[i].clear();
        for(int i=1;i<=n;i++)w[i]=rndint(-10,20);
        for(int v=2;v<=n;v++)
        {
            int u=rndint(1,v-1);
            adj[u].push_back(v),adj[v].push_back(u);
        }
        build_all();
        for(int step=1;step<=20;step++)
        {
            int u=rndint(1,n);
            ll x=rndint(-10,20);
            update(u,x);
            get_f(1,f0,f1);
            ll cur=max(f0,f1),ref=brute_ans();
            if(cur!=ref)
            {
                bad++;
                printf("FAILED! 轮%d 第%d次修改 n=%d u=%d x=%lld ref=%lld cur=%lld\n",tt,step,n,u,x,ref,cur);
                break;
            }
        }
        if(bad)break;
    }
    if(!bad)printf("stress OK (60 组随机树 x 每组 20 次随机点权修改，共 1200 次全部与暴力一致)\n");
    return 0;
}

/*
动态 dp 用法（带修改的最大权独立集）：
1. 先 dfs1 求重儿子，dfs2 剖重链，链内顺序按 链头->链尾
2. 线段树每条链一棵，叶子下标 0 是链尾；合并按 浅*深 的顺序做矩阵乘
3. 修改实际结点的矩阵，再把链头 dp 的变化传给其父结点，沿重链向上跳
4. 查询答案直接 get_f(1) 后取 max
也可换成全局线段树（所有链首尾相接存一个数组）把复杂度降到 O(log^2 n)
*/
