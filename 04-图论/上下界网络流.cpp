// 适用：每条边必须达到最低流量的可行流、最大流、最小流。
// 编号：原点 1..n，边 1..m；超级源汇 n+1,n+2，必须均小于 N。
// 参数：eu/ev 是端点，0<=elow<=eup；有源汇时 s!=t。
// 关键：先固定下界，再用 eup-elow 建残量边；d 记录下界造成的收支差。
// 结论：超级源总流量等于 need 才可行；原边实际流量为 elow[i]+cap[eidx[i]^1]。
// 易错：数组需容纳原边、平衡边和 t->s 边及反边；INF 须大于所需总流量。
// 边界：最小流写法允许 base-dinic(t,s) 为负；题目若只接受非负流需另核对约定。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const ll INF=1e18;
int n,m,s,t,S,T;// S,T 是超级源汇
int head[N],to[N<<1],nxt[N<<1],num=1;
ll cap[N<<1];
int dep[N],cur[N];
ll d[N];// d[i]>0：i 还需要流入这么多；d[i]<0：i 需要流出这么多
int eu[N],ev[N],eidx[N];// eidx[i]：第 i 条上下界边对应的正向边编号
ll elow[N],eup[N];

// O(1)，加入 u->v 的剩余容量 c；端点可含超级源汇。
void add_edge(int u,int v,ll c)
{
    to[++num]=v,cap[num]=c,nxt[num]=head[u],head[u]=num;
    to[++num]=u,cap[num]=0,nxt[num]=head[v],head[v]=num;// 反向边容量 0
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
        for(int i=head[u];i;i=nxt[i])
            if(cap[i]>0&&dep[to[i]]<0)
            {
                dep[to[i]]=dep[u]+1;
                q.push(to[i]);
            }
    }
    return dep[tt]>=0;
}

// 从 u 到 tt 推至多 flow；单次最坏 O(n*m)，当前弧避免同轮反复试死边。
ll dfs(int u,int tt,ll flow)// 沿分层图推流，当前弧优化
{
    if(u==tt)return flow;
    for(int &i=cur[u];i;i=nxt[i])
    {
        int v=to[i];
        if(cap[i]>0&&dep[v]==dep[u]+1)
        {
            ll f=dfs(v,tt,min(flow,cap[i]));
            if(f>0)
            {
                cap[i]-=f;
                cap[i^1]+=f;
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
        for(int i=1;i<=n+2;i++)cur[i]=head[i];
        ll f;
        while((f=dfs(ss,tt,INF))>0)ans+=f;
    }
    return ans;
}

// O(n+m)，从 1..m 的输入边重建；eidx 指向正边，清空旧平衡量。
void build()// 按 eu/ev/elow/eup 重新建图：上下界边先默认流下界，剩下的容量建成普通边
{
    for(int i=1;i<=n+2;i++)head[i]=0,d[i]=0;
    num=1;
    for(int i=1;i<=m;i++)
    {
        add_edge(eu[i],ev[i],eup[i]-elow[i]);
        eidx[i]=num-1;
        d[eu[i]]-=elow[i],d[ev[i]]+=elow[i];// 下界先当满流记账
    }
}

// type=0：无源汇可行流；type=1：有源汇上下界最大流；type=2：有源汇上下界最小流
// 返回流量大小，-1 表示无解
// O(n^2*m)，type=0/1/2 对应可行/最大/最小；返回 -1 也可能与负最小流混淆。
ll solve_lr(int type)
{
    // O(n+m)，从 1..m 的输入边重建；eidx 指向正边，清空旧平衡量。
    build();
    int e_ts=0;
    if(type)add_edge(t,s,INF),e_ts=num-1;// 人为加 t->s 的无穷边，把有源汇变成无源汇
    S=n+1,T=n+2;
    ll need=0;
    for(int i=1;i<=n;i++)
    {
        if(d[i]>0)add_edge(S,i,d[i]),need+=d[i];// 缺流入的点由超级源补
        else if(d[i]<0)add_edge(i,T,-d[i]);// 多出来的流出丢给超级汇
    }
    if(dinic(S,T)!=need)return -1;// 超级源没流满 -> 下界无法同时满足
    if(type==0)return 0;
    ll base=cap[e_ts^1];// t->s 边上流过的量，就是当前 s->t 的流量
    for(int i=2;i<=num;i++)
        if(to[i]==S||to[i]==T)cap[i]=0,cap[i^1]=0;// 拆掉超级源汇的边
    cap[e_ts]=0,cap[e_ts^1]=0;// 拆掉人为边
    if(type==1)return base+dinic(s,t);// 还能继续增广就是最大流
    return base-dinic(t,s);// 能退掉的流量越多，剩下的就是最小流
}
