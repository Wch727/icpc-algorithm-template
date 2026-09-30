#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// ========== 一、迭代加深 IDDFS ==========
// 适合「答案深度浅，但每层分支巨大」的搜索：逐层放宽深度上限，
// 空间只有 O(深度)，且第一次找到答案时深度即为最优（每层代价相同的前提）。
// 单次 DFS 为 O(b^lim)，总复杂度 O(b^lim)，比 BFS 省状态存储。

int id_n;
int id_a[25];
bool id_vis[25];
int id_ans;

// 用互不相同的 a[i] 做加减，凑出 target，问最少用几个数
bool id_dfs(int dep,int lim,ll cur,ll target)
{
    if(cur==target)return true;
    if(dep==lim)return false;//到达深度上限，直接回溯
    for(int i=1;i<=id_n;i++)
    {
        if(id_vis[i])continue;
        id_vis[i]=true;
        if(id_dfs(dep+1,lim,cur+id_a[i],target))return true;
        if(id_dfs(dep+1,lim,cur-id_a[i],target))return true;
        id_vis[i]=false;
    }
    return false;
}

// 返回最少用几个数；无解返回 -1
int solve_iddfs(ll target)
{
    for(int lim=1;lim<=id_n;lim++)//上限从小到大
        if(id_dfs(0,lim,0,target))return lim;
    return -1;
}

// ========== 二、IDA*：knight swap（洛谷 P2324 同型）==========
// 估价函数 h = 不在目标位置的棋子数，每次移动最多修正 1 个，可采纳。
// f = g + h > maxdep 就剪枝。

const int KB=5;
int ks[KB][KB],ks_goal[KB][KB];
int ks_sx,ks_sy,cnt_ks,ks_lim,ks_sol;
int kdx[]={0,-2,-2,-1,-1,1,1,2,2};
int kdy[]={0,-1,1,-2,2,-2,2,-1,1};

int ks_h()//O(25)
{
    int c=0;
    for(int i=0;i<KB;i++)
        for(int j=0;j<KB;j++)
            if(ks[i][j]!=ks_goal[i][j])c++;
    return c;
}

long long ks_nodes;//统计访问结点数，用来看搜索规模

void ks_dfs(int x,int y,int g,int pre)
{
    ks_nodes++;
    int h=ks_h();
    if(h==0){ks_sol=g;return;}
    if(g+h>ks_lim)return;//IDA* 核心剪枝
    for(int i=1;i<=8;i++)
    {
        int xx=x+kdx[i],yy=y+kdy[i];
        if(xx<0||xx>=KB||yy<0||yy>=KB)continue;
        if(xx*KB+yy==pre)continue;//不立刻走回上一步
        swap(ks[x][y],ks[xx][yy]);
        ks_dfs(xx,yy,g+1,x*KB+y);
        swap(ks[x][y],ks[xx][yy]);
        if(ks_sol!=-1)return;
    }
}

// 返回最少步数，无解（超过 maxdepth）返回 -1
int ks_solve(int maxdepth)
{
    ks_sol=-1;
    for(int lim=0;lim<=maxdepth;lim++)
    {
        ks_lim=lim;
        ks_dfs(ks_sx,ks_sy,0,-1);
        if(ks_sol!=-1)return ks_sol;
    }
    return -1;
}

// 暴力 BFS 求真实最少步数，用于对拍（状态空间小的时候可以用）
int ks_bfs()
{
    string s0,t0;
    for(int i=0;i<KB;i++)
        for(int j=0;j<KB;j++)
        {
            s0.push_back((char)('0'+ks[i][j]+1));
            t0.push_back((char)('0'+ks_goal[i][j]+1));
        }
    if(s0==t0)return 0;
    map<string,int> dis;
    queue<string> q;
    dis[s0]=0;
    q.push(s0);
    while(!q.empty())
    {
        string u=q.front();
        q.pop();
        int p=u.find('0');
        int x=p/KB,y=p%KB;
        for(int i=1;i<=8;i++)
        {
            int xx=x+kdx[i],yy=y+kdy[i];
            if(xx<0||xx>=KB||yy<0||yy>=KB)continue;
            string v=u;
            swap(v[p],v[xx*KB+yy]);
            if(dis.count(v))continue;
            dis[v]=dis[u]+1;
            if(v==t0)return dis[v];
            q.push(v);
        }
    }
    return -1;
}

void ks_rand_start(mt19937 &rnd)
{
    int p[KB*KB];
    for(int i=0;i<KB*KB;i++)p[i]=i;
    for(int i=KB*KB-1;i>0;i--)swap(p[i],p[rnd()%(i+1)]);
    for(int i=0;i<KB;i++)
        for(int j=0;j<KB;j++)
        {
            int t=p[i*KB+j];
            ks[i][j]=t%3-1;//-1 空格，0 白，1 黑
            if(t%3==0)ks_sx=i,ks_sy=j;
        }
}

// ========== 三、IDA*：九连环型旋钮（洛谷 P5507 思路）==========
// n 个旋钮各 4 个状态，一次操作把旋钮 i 和它指向的旋钮 t[i] 都 +1（模 4）。
// 估价：每个旋钮到 1 的最少次数之和的一半上取整。
// 这个估价要求 t[i]!=i（每次操作改善两个不同旋钮），否则会高估、剪掉正解

int cr_n;
int cr_s[20],cr_t[20];
int cr_sol,cr_lim;
const int DIST1[4]={0,0,3,2};//状态 0/1/2/3 变到 1 的最少次数

int cr_h()
{
    int sum=0;
    for(int i=1;i<=cr_n;i++)sum+=DIST1[cr_s[i]];
    return (sum+1)/2;
}

void cr_dfs(int g,int pre)
{
    int h=cr_h();
    if(h==0){cr_sol=g;return;}
    if(g+h>cr_lim)return;
    for(int i=1;i<=cr_n;i++)
    {
        if(i==pre)continue;//撤销上一步无意义
        cr_s[i]=(cr_s[i]+1)&3;
        cr_s[cr_t[i]]=(cr_s[cr_t[i]]+1)&3;
        cr_dfs(g+1,i);
        cr_s[cr_t[i]]=(cr_s[cr_t[i]]+3)&3;
        cr_s[i]=(cr_s[i]+3)&3;
        if(cr_sol!=-1)return;
    }
}

int cr_solve(int maxdepth)
{
    cr_sol=-1;
    for(int lim=cr_h();lim<=maxdepth;lim++)//下界从 h 开始，别从 0 浪费
    {
        cr_lim=lim;
        cr_dfs(0,0);
        if(cr_sol!=-1)return cr_sol;
    }
    return -1;
}

// 朴素 BFS 暴力，用于对拍（状态数 4^n，n<=6 才用）
int cr_brute()
{
    int st=0,goal=0;
    for(int i=1;i<=cr_n;i++){st|=cr_s[i]<<(2*(i-1));goal|=1<<(2*(i-1));}
    map<int,int> dis;
    queue<int> q;
    dis[st]=0;
    q.push(st);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        if(u==goal)return dis[u];
        for(int i=1;i<=cr_n;i++)
        {
            int v=u;
            int a=(v>>(2*(i-1)))&3;
            v^=a<<(2*(i-1));
            v|=((a+1)&3)<<(2*(i-1));
            int b=(v>>(2*(cr_t[i]-1)))&3;
            v^=b<<(2*(cr_t[i]-1));
            v|=((b+1)&3)<<(2*(cr_t[i]-1));
            if(dis.count(v))continue;
            dis[v]=dis[u]+1;
            q.push(v);
        }
    }
    return -1;
}
