#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=105;
const ll INF=1e15;
int n,m,root;// 模板内部用 0..n-1 编号，缩点后 n 会变小
int ok;// 1 表示最小树形图存在（边权可以为负，所以别拿 -1 当无解标志）
struct Edge
{
    int u,v;
    ll w;
};
Edge e[N*N];// 边表，每轮缩点后原地修改
ll in[N];// 每个点当前选中的最小入边权
int pre[N],idd[N],vis[N];

ll zhuliu()// 有向图最小树形图（朱刘/Edmonds），O(n*m)；结果存返回值，无解时 ok=0
{
    ll ans=0;
    ok=1;
    while(1)
    {
        for(int i=0;i<n;i++)in[i]=INF;
        for(int i=1;i<=m;i++)
            if(e[i].u!=e[i].v&&e[i].w<in[e[i].v])
                in[e[i].v]=e[i].w,pre[e[i].v]=e[i].u;// 自环不考虑
        for(int i=0;i<n;i++)
            if(i!=root&&in[i]==INF)
            {
                ok=0;// 有人没有入边 -> 根到不了它，无解
                return 0;
            }
        int cnt=0;
        for(int i=0;i<n;i++)idd[i]=-1,vis[i]=-1;
        in[root]=0;
        for(int i=0;i<n;i++)
        {
            ans+=in[i];
            int v=i;
            while(vis[v]!=i&&idd[v]==-1&&v!=root)
                vis[v]=i,v=pre[v];// 顺着入边往上爬，找环
            if(v!=root&&idd[v]==-1)// 找到一个没见过的新环，缩成一点
            {
                for(int u=pre[v];u!=v;u=pre[u])idd[u]=cnt;
                idd[v]=cnt++;
            }
        }
        if(!cnt)break;// 一个环都没有，已经是最小树形图
        for(int i=0;i<n;i++)if(idd[i]==-1)idd[i]=cnt++;// 不在环里的点各成一点
        for(int i=1;i<=m;i++)
        {
            int v=e[i].v;
            e[i].u=idd[e[i].u],e[i].v=idd[v];
            if(e[i].u!=e[i].v)e[i].w-=in[v];// 环内的边扣掉原本选的入边，抵消重复计算
        }
        n=cnt;
        root=idd[root];
    }
    return ans;
}
