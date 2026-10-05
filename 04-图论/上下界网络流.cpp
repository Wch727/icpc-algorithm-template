// 适用：每条边必须达到最低流量的可行流、最大流、最小流。
// 编号：原点 1..n，边 1..m；超级源汇 n+1,n+2，必须均小于 N。
// 参数：eu/ev 是端点，0<=elow<=eup；有源汇时 s!=t。
// 关键：先固定下界，再用 eup-elow 建残量边；d 记录下界造成的收支差。
// 结论：超级源总流量等于 need 才可行；原边实际流量为 elow[i]+e[eidx[i]^1].cap。
// 易错：e 动态容纳原边、平衡边和 t->s 边及反边；INF 须大于所需总流量。
// 边界：最小流写法允许 base-dinic(t,s) 为负；题目若只接受非负流需另核对约定。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const ll INF=1e18;
int n,m,s,t,S,T;// S,T 是超级源汇
struct Edge{int to;ll cap;};
vector<Edge> e;
vector<int> adj[N];// 存 e 的下标，从 0 起；id^1 是反向边
int dep[N],cur[N];
ll d[N];// d[i]>0：i 还需要流入这么多；d[i]<0：i 需要流出这么多
int eu[N],ev[N],eidx[N];// eidx[i]：第 i 条上下界边对应的正向边编号
ll elow[N],eup[N];

// O(1)，加入 u->v 的剩余容量 c；端点可含超级源汇。
void add_edge(int u,int v,ll c)
{
    int id=e.size();
    e.push_back({v,c});
    e.push_back({u,0});
    adj[u].push_back(id);
    adj[v].push_back(id^1);
}

// O(n+m)，ss/tt 是本轮源汇；只给正残量边分层。
int bfs(int ss,int tt)// 分层，O(m)
{
    for(int i=1;i<=n+2;i++)dep[i]=-1;
    queue<int> q;
    dep[ss]=0;
    q.push(ss);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i:adj[u])
            if(e[i].cap>0&&dep[e[i].to]<0)
            {
                dep[e[i].to]=dep[u]+1;
                q.push(e[i].to);
            }
    }
    return dep[tt]>=0;
}

// 从 u 到 tt 推至多 flow；单次最坏 O(n*m)，当前弧避免同轮反复试死边。
ll dfs(int u,int tt,ll flow)// 沿分层图推流，当前弧优化
{
    if(u==tt)return flow;
    for(int &p=cur[u];p<(int)adj[u].size();p++)
    {
        int i=adj[u][p],v=e[i].to;
        if(e[i].cap>0&&dep[v]==dep[u]+1)
        {
            ll f=dfs(v,tt,min(flow,e[i].cap));
            if(f>0)
            {
                e[i].cap-=f;
                e[i^1].cap+=f;
                return f;
            }
        }
    }
    return 0;
}

// O(n^2*m)，返回 ss->tt 的新增流量，并修改残量容量。
ll dinic(int ss,int tt)// 最大流，O(n^2 m)
{
    ll ans=0;
    while(bfs(ss,tt))
    {
        for(int i=1;i<=n+2;i++)cur[i]=0;
        ll f;
        while((f=dfs(ss,tt,INF))>0)ans+=f;
    }
    return ans;
}

// O(n+m)，从 1..m 的输入边重建；eidx 指向正边，清空旧平衡量。
void build()// 按 eu/ev/elow/eup 重新建图：上下界边先默认流下界，剩下的容量建成普通边
{
    for(int i=1;i<=n+2;i++)adj[i].clear(),d[i]=0;
    e.clear();
    for(int i=1;i<=m;i++)
    {
        add_edge(eu[i],ev[i],eup[i]-elow[i]);
        eidx[i]=(int)e.size()-2;
        d[eu[i]]-=elow[i],d[ev[i]]+=elow[i];// 下界先当满流记账
    }
}

// type=0：无源汇可行流；type=1：有源汇上下界最大流；type=2：有源汇上下界最小流
// 返回流量大小，-1 表示无解
// O(n^2*m)，type=0/1/2 对应可行/最大/最小；返回 -1 也可能与负最小流混淆。
ll solve_lr(int type)
{
    build();
    int e_ts=0;
    if(type)add_edge(t,s,INF),e_ts=(int)e.size()-2;// 人为加 t->s 的无穷边，把有源汇变成无源汇
    S=n+1,T=n+2;
    ll need=0;
    for(int i=1;i<=n;i++)
    {
        if(d[i]>0)add_edge(S,i,d[i]),need+=d[i];// 缺流入的点由超级源补
        else if(d[i]<0)add_edge(i,T,-d[i]);// 多出来的流出丢给超级汇
    }
    if(dinic(S,T)!=need)return -1;// 超级源没流满 -> 下界无法同时满足
    if(type==0)return 0;
    ll base=e[e_ts^1].cap;// t->s 边上流过的量，就是当前 s->t 的流量
    for(int i=0;i<(int)e.size();i++)
        if(e[i].to==S||e[i].to==T)e[i].cap=0,e[i^1].cap=0;// 拆掉超级源汇的边
    e[e_ts].cap=0,e[e_ts^1].cap=0;// 拆掉人为边
    if(type==1)return base+dinic(s,t);// 还能继续增广就是最大流
    return base-dinic(t,s);// 能退掉的流量越多，剩下的就是最小流
}
