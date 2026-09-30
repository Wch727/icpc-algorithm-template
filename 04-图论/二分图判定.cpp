#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
vector<int> adj[N];
int col[N];// 0 未染色，1 / 2 两种颜色（黑白）
int c1,c2;// 当前连通块两种颜色的点数

int dfs_color(int u,int c)// 深搜染色，返回 0 表示出现奇环（不是二分图）
{
    col[u]=c;
    if(c==1)c1++;
    else c2++;
    for(int i=0;i<(int)adj[u].size();i++)
    {
        int v=adj[u][i];
        if(!col[v])
        {
            if(!dfs_color(v,3-c))return 0;// 3-c 就是换成另一种颜色
        }
        else if(col[v]==c)return 0;// 相邻同色 -> 矛盾
    }
    return 1;
}

int bfs_color(int s)// 宽搜染色，写法与 dfs 等价
{
    queue<int> q;
    col[s]=1,c1=1,c2=0;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=0;i<(int)adj[u].size();i++)
        {
            int v=adj[u][i];
            if(!col[v])
            {
                col[v]=3-col[u];
                if(col[v]==1)c1++;
                else c2++;
                q.push(v);
            }
            else if(col[v]==col[u])return 0;
        }
    }
    return 1;
}
