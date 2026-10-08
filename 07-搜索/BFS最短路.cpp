#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=405;

// 四方向偏移表（下标 0 空着，从 1 开始用）
int dx4[]={0,1,-1,0,0};
int dy4[]={0,0,0,1,-1};

// 马走日，八方向
int dx8[]={0,-2,-2,-1,-1,1,1,2,2};
int dy8[]={0,-1,1,-2,2,-2,2,-1,1};

// ---------- 一、网格 BFS 求最少步数 ----------

int n,m;
int dis[N][N];//-1 表示不可达
bool block[N][N];//障碍格

// 单源 BFS，O(nm)
void bfs_grid(int sx,int sy)
{
    memset(dis,-1,sizeof(dis));
    queue<pair<int,int> > q;
    if(block[sx][sy])return;
    dis[sx][sy]=0;
    q.push(make_pair(sx,sy));
    while(!q.empty())
    {
        int x=q.front().first,y=q.front().second;
        q.pop();
        for(int i=1;i<=4;i++)
        {
            int xx=x+dx4[i],yy=y+dy4[i];
            if(xx<1||xx>n||yy<1||yy>m)continue;//边界
            if(block[xx][yy])continue;//障碍
            if(dis[xx][yy]!=-1)continue;//已访问
            dis[xx][yy]=dis[x][y]+1;
            q.push(make_pair(xx,yy));
        }
    }
}

// ---------- 二、多源 BFS ----------

int md[N][N];
// 多个起点同时入队，求每个点到最近起点的距离，O(nm)
void bfs_multi(vector<pair<int,int> > src)
{
    memset(md,-1,sizeof(md));
    queue<pair<int,int> > q;
    for(int i=0;i<(int)src.size();i++)
    {
        int x=src[i].first,y=src[i].second;
        md[x][y]=0;
        q.push(src[i]);
    }
    while(!q.empty())
    {
        int x=q.front().first,y=q.front().second;
        q.pop();
        for(int i=1;i<=8;i++)//马步
        {
            int xx=x+dx8[i],yy=y+dy8[i];
            if(xx<1||xx>n||yy<1||yy>m)continue;
            if(md[xx][yy]!=-1)continue;
            md[xx][yy]=md[x][y]+1;
            q.push(make_pair(xx,yy));
        }
    }
}

// ---------- 三、泛洪 / 连通块 ----------

int g[N][N];
// 从 (x,y) 出发把同一连通块染色；调用外面留一圈 0 保证边界的连通性
void flood(int x,int y,int c)
{
    assert(c!=0);
    if(g[x][y]!=0)return;
    queue<pair<int,int> > q;
    g[x][y]=c;
    q.push(make_pair(x,y));
    while(!q.empty())
    {
        int xx=q.front().first,yy=q.front().second;
        q.pop();
        for(int i=1;i<=4;i++)
        {
            int a=xx+dx4[i],b=yy+dy4[i];
            if(a<0||a>n+1||b<0||b>m+1)continue;//外面留了一圈
            if(g[a][b])continue;//非 0 都是非空
            g[a][b]=c;
            q.push(make_pair(a,b));
        }
    }
}

// ---------- 四、带时间限制的 BFS（流星雨）----------

const int INF=0x3f3f3f3f;
int tim[N][N],dt[N][N];
// tim[i][j] = 该格被砸毁的时刻（INF 表示永不），求最早能到达「永不被砸」的格子的时刻
// 站在格子上要求到达时刻 t 满足 t < tim；注意被砸的格子在时刻 tim 就不能站了
// 起点 (0,0) 若在时刻 0 就被砸则无解
int bfs_meteor()
{
    memset(dt,-1,sizeof(dt));
    if(tim[0][0]<=0)return -1;
    queue<pair<int,int> > q;
    dt[0][0]=0;
    q.push(make_pair(0,0));
    while(!q.empty())
    {
        int x=q.front().first,y=q.front().second;
        q.pop();
        if(tim[x][y]==INF)return dt[x][y];//永久安全的格子，BFS 首次到即最早
        for(int i=1;i<=4;i++)
        {
            int xx=x+dx4[i],yy=y+dy4[i];
            if(xx<0||xx>N-1||yy<0||yy>N-1)continue;
            if(dt[xx][yy]!=-1)continue;
            if(dt[x][y]+1>=tim[xx][yy])continue;//到达时已被砸
            dt[xx][yy]=dt[x][y]+1;
            q.push(make_pair(xx,yy));
        }
    }
    return -1;
}

// ---------- 五、八数码状态哈希 ----------

// 把 3*3 压成一个 36 位整数：第 i 格（0..8）放在右起第 (8-i) 个 4 bit，
// 也就是「按行读成十六进制」的样子，目标态 123456780 编码后就是 0x123456780
ll encode(int b[3][3])
{
    ll s=0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)s|=(ll)b[i][j]<<(4*(8-(i*3+j)));
    return s;
}

void decode(ll s,int b[3][3])
{
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)b[i][j]=(s>>(4*(8-(i*3+j))))&15;
}

// 八数码里空格向上下左右移动：pdx 是行号变化，pdy 是列号变化
// 行号增大是往下，所以「下」是 +1；这里写成 {0,-1,1,0,0} 表示 上、下、左、右
const int pdx[]={0,-1,1,0,0};
const int pdy[]={0,0,0,-1,1};

// 交换第 p 格与第 q 格（p,q 是 0..8 的格子编号），O(1)
// 用 decode/encode 保证索引口径和上面完全一致，不容易写错
ll flip(ll s,int p,int q)
{
    int b[3][3];
    decode(s,b);
    swap(b[p/3][p%3],b[q/3][q%3]);
    return encode(b);
}

// 八数码最少步数（标准 123456780 为目标），无解返回 -1，O(状态数*4)
int bfs_puzzle(int st[3][3])
{
    int goal[3][3]= {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    const ll T=encode(goal);//目标态编码 = 0x123456780
    ll S=encode(st);
    if(S==T)return 0;
    map<ll,int> dis;
    queue<pair<ll,int> > q;//(状态, 空格位置)
    int z=0;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(!st[i][j])z=i*3+j;
    dis[S]=0;
    q.push(make_pair(S,z));
    while(!q.empty())
    {
        ll u=q.front().first;
        int p=q.front().second;
        q.pop();
        int zx=p/3,zy=p%3;
        for(int i=1;i<=4;i++)
        {
            int xx=zx+pdx[i],yy=zy+pdy[i];
            if(xx<0||xx>2||yy<0||yy>2)continue;
            ll v=flip(u,zx*3+zy,xx*3+yy);
            if(dis.count(v))continue;
            dis[v]=dis[u]+1;
            if(v==T)return dis[v];
            q.push(make_pair(v,xx*3+yy));
        }
    }
    return -1;
}
