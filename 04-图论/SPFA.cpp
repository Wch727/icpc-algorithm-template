#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=1e18;
int n,m,s;
struct Edge
{
    int to,w;
};
vector<Edge> adj[N];
ll dis[N];
int cnt[N];// cnt[u]：u 的入队次数，>=n 说明有负环
bool inq[N];

int spfa(int s)// 期望 O(k*m)，最坏 O(n*m)；返回 1 表示存在可达负环
{
    for(int i=1;i<=n;i++)dis[i]=INF,cnt[i]=0,inq[i]=0;
    queue<int> q;
    dis[s]=0,inq[s]=1,cnt[s]=1;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        inq[u]=0;
        for(int i=0;i<(int)adj[u].size();i++)
        {
            int v=adj[u][i].to,c=adj[u][i].w;
            if(dis[u]+c<dis[v])
            {
                dis[v]=dis[u]+c;
                if(!inq[v])
                {
                    q.push(v);
                    inq[v]=1;
                    cnt[v]++;
                    if(cnt[v]>=n)return 1;// 一个点入队 n 次必有负环
                }
            }
        }
    }
    return 0;
}

int main()
{
    // 自测 1：手造有向图 1->2(2) 1->3(5) 2->3(1) 2->4(4) 3->4(1) 4->5(3)
    n=5,m=6;
    int a[10]={0,1,1,2,2,3,4},b[10]={0,2,3,3,4,4,5},c[10]={0,2,5,1,4,1,3};
    for(int i=1;i<=m;i++)
    {
        Edge e;
        e.to=b[i],e.w=c[i];
        adj[a[i]].push_back(e);
    }
    int bad=spfa(1);
    printf("手造图 负环=%d dis=",bad);
    for(int i=1;i<=n;i++)printf("%lld ",dis[i]);
    printf("\n期望 负环=0 dis= 0 2 3 4 7\n");
    // 自测 2：单点负自环 2->2(-1)，必判出负环
    n=3,m=3;
    for(int i=1;i<=n;i++)adj[i].clear();
    int a2[5]={0,1,2,2},b2[5]={0,2,2,3},c2[5]={0,1,-1,1};
    for(int i=1;i<=m;i++)
    {
        Edge e;
        e.to=b2[i],e.w=c2[i];
        adj[a2[i]].push_back(e);
    }
    printf("负自环图 负环=%d（期望 1）\n",spfa(1));
    // 自测 3：随机图 + 随机权，与 Floyd 对拍（只测无负环的图）
    n=10;
    int ok=1;
    for(int t=1;t<=200&&ok;t++)
    {
        for(int i=1;i<=n;i++)adj[i].clear();
        int w[20][20];
        memset(w,0x3f,sizeof(w));
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(i!=j&&rand()%35==0)
                {
                    int cc=rand()%30-10;// 含负权，但无负环
                    w[i][j]=cc;
                    Edge e;
                    e.to=j,e.w=cc;
                    adj[i].push_back(e);
                }
        int fl[20][20];
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)fl[i][j]=(i==j?0:w[i][j]);
        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(fl[i][k]+fl[k][j]<fl[i][j])fl[i][j]=fl[i][k]+fl[k][j];
        for(int st=1;st<=n;st++)
        {
            if(spfa(st))// 出现负环就换一组数据
            {
                ok=0;
                break;
            }
            for(int i=1;i<=n;i++)
            {
                ll want=(fl[st][i]>=(int)1e8?INF:fl[st][i]);
                if(dis[i]!=want)
                {
                    printf("WA t=%d st=%d i=%d got=%lld want=%lld\n",t,st,i,dis[i],want);
                    return 0;
                }
            }
        }
    }
    printf("与 Floyd 对拍 200 组通过（含负权边）\n");
    return 0;
}
/* 注意：判负环要看「从源点可达」的负环，cnt[v]>=n 才返回 1 */
