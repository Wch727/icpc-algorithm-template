#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
const int N=1005;

// 双向广搜：起点和终点同时 BFS，交替扩展「待扩展结点少」的那一侧。
// 第一次汇合时两边的距离之和就是最短路，扩展量约为单向的 2*b^(d/2)。
// 要求边权全为 1（或者等权），否则汇合时的答案不一定最优。

int dx4[]={0,1,-1,0,0};
int dy4[]={0,0,0,1,-1};

// ---------- 一、网格迷宫上的双向 BFS ----------

int n,m;
char mp[N][N];//'#' 是墙，'.' 是空地
int df[N][N],db[N][N];//正向/反向到该格的距离

// 返回最少步数，不可达返回 -1，O(nm)
// df 是正向距离，db 是反向距离；注意汇合时要扫一整层邻居（不能只看新点的距离），
// 并且要等「两侧已扩展层数之和 >= 目前最好答案」才能停，否则会退出太早。
int bibfs_grid(int sx,int sy,int ex,int ey)
{
    if(mp[sx][sy]=='#'||mp[ex][ey]=='#')return -1;
    if(sx==ex&&sy==ey)return 0;
    memset(df,-1,sizeof(df));
    memset(db,-1,sizeof(db));
    queue<pair<int,int> > qf,qb;
    df[sx][sy]=0,qf.push(make_pair(sx,sy));
    db[ex][ey]=0,qb.push(make_pair(ex,ey));
    int depf=0,depb=0;//正向/反向「已经完整扩展过」的层数
    int best=0x3f3f3f3f;
    while(!qf.empty()&&!qb.empty())
    {
        if(depf+depb>=best)break;//再走也不会更短
        // 每次都挑小的一侧扩展，这是双向 BFS 的关键
        if(qf.size()<=qb.size())
        {
            int x=qf.front().first,y=qf.front().second;
            qf.pop();
            depf=max(depf,df[x][y]);
            for(int i=1;i<=4;i++)
            {
                int xx=x+dx4[i],yy=y+dy4[i];
                if(xx<1||xx>n||yy<1||yy>m)continue;
                if(mp[xx][yy]=='#')continue;
                if(df[xx][yy]==-1)
                {
                    df[xx][yy]=df[x][y]+1;
                    qf.push(make_pair(xx,yy));
                }
                if(db[xx][yy]!=-1)best=min(best,df[xx][yy]+db[xx][yy]);//两侧在 (xx,yy) 汇合
            }
        }
        else
        {
            int x=qb.front().first,y=qb.front().second;
            qb.pop();
            depb=max(depb,db[x][y]);
            for(int i=1;i<=4;i++)
            {
                int xx=x+dx4[i],yy=y+dy4[i];
                if(xx<1||xx>n||yy<1||yy>m)continue;
                if(mp[xx][yy]=='#')continue;
                if(db[xx][yy]==-1)
                {
                    db[xx][yy]=db[x][y]+1;
                    qb.push(make_pair(xx,yy));
                }
                if(df[xx][yy]!=-1)best=min(best,df[xx][yy]+db[xx][yy]);
            }
        }
    }
    return best==0x3f3f3f3f?-1:best;
}

// ---------- 二、数字变换上的双向 BFS ----------

// 操作：把某一位数字 +1，逢 9 变 0（模 10）。
// 例：1234 -> 1235、1234 -> 1244、1999 -> 1990（每次只改一位）
// 位数少（<=6）时状态最多 10^6，用 ull 直接编码（每 4 bit 一位），
// 每位 4 bit，反向搜索用 -1，枚举有向操作的前驱。
int dig;
unordered_map<ull,int> disf,disb;

ull step_digit(ull s,int k,int delta=1)//第 k 位（从低位起 0 开始）+1，O(1)
{
    int v=(s>>(4*k))&15;
    v=(v+delta+10)%10;
    return (s&~(15ULL<<(4*k)))|((ull)v<<(4*k));
}

// 返回最少步数，不可达返回 -1，O(状态数*dig)
int bibfs_num(ull s,ull t)
{
    if(s==t)return 0;
    disf.clear(),disb.clear();
    queue<ull> qf,qb;
    disf[s]=0,qf.push(s);
    disb[t]=0,qb.push(t);
    int depf=0,depb=0,best=0x3f3f3f3f;
    while(!qf.empty()&&!qb.empty())
    {
        if(depf+depb>=best)break;
        if(qf.size()<=qb.size())
        {
            ull u=qf.front();qf.pop();
            depf=max(depf,disf[u]);
            for(int k=0;k<dig;k++)
            {
                ull v=step_digit(u,k);
                if(!disf.count(v))
                {
                    disf[v]=disf[u]+1;
                    qf.push(v);
                }
                if(disb.count(v))best=min(best,disf[v]+disb[v]);
            }
        }
        else
        {
            ull u=qb.front();qb.pop();
            depb=max(depb,disb[u]);
            for(int k=0;k<dig;k++)
            {
                ull v=step_digit(u,k,-1);
                if(!disb.count(v))
                {
                    disb[v]=disb[u]+1;
                    qb.push(v);
                }
                if(disf.count(v))best=min(best,disf[v]+disb[v]);
            }
        }
    }
    return best==0x3f3f3f3f?-1:best;
}
