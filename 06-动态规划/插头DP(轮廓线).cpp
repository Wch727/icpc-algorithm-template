#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=14;// 轮廓线长度（列数上限）
int n,m;// n 行 m 列，插头 dp 适合 n,m<=12
int vis[N][N];
// 状态编码：2 bit 一个插头，从低位到高位对应轮廓线上第 0,1,...,m 个位置
// 0 = 无插头，1 = 左括号 '(', 2 = 右括号 ')'
// 轮廓线上"还没接好的插头"一定是匹配好的括号序列，所以只需要记括号类型
// 第 j 个格子的左插头在位置 j，上插头在位置 j+1（j 从 0 起）
unordered_map<int,ll> f[2];// 滚动数组，按格转移

int getp(int st,int p)// 取第 p 个插头的类型
{
    return (st>>(p*2))&3;
}

int setp(int st,int p,int v)// 把第 p 个插头改成 v
{
    return (st&~(3<<(p*2)))|(v<<(p*2));
}

// 找第 p 个插头的配对括号位置，找不到返回 -1
int match_p(int st,int p,int m)
{
    int v=getp(st,p);
    if(v==0)return -1;
    int cnt=0;
    if(v==1)// '(' 向右找配对的 ')'
    {
        for(int i=p;i<=m;i++)
        {
            int c=getp(st,i);
            if(c==1)cnt++;
            else if(c==2)cnt--;
            if(cnt==0)return i;
        }
    }
    else// ')' 向左找配对的 '('
    {
        for(int i=p;i>=0;i--)
        {
            int c=getp(st,i);
            if(c==2)cnt++;
            else if(c==1)cnt--;
            if(cnt==0)return i;
        }
    }
    return -1;
}

int has_plug(int st,int m)// 轮廓线上还有没有未闭合的插头（只用到 0..m-1 位）
{
    for(int i=0;i<m;i++)
        if(getp(st,i))return 1;
    return 0;
}

// 求 n*m 网格的哈密顿回路条数（每个格子恰好走一次，最后回到起点）
// 逐格转移，四种情况：0 个插头 / 1 个插头(分左右括号) / 2 个插头(分三种括号组合)
// 第 j 位是左插头，第 j+1 位是上插头；输出下插头在 j，右插头在 j+1
// 复杂度 O(n*m*状态数)，本题模板适合 n,m<=12
ll plug_dp(int n,int m)
{
    if((n*m)%2==1)return 0;// 奇数个格子不可能有哈密顿回路
    f[0].clear(),f[1].clear();
    f[0][0]=1;
    int cur=0;
    for(int i=0;i<n;i++)
    {
        if(i>0)// 换行后下插头变为下一行的上插头
        {
            f[cur^1].clear();
            for(auto &pr:f[cur])
                if(!getp(pr.first,m))f[cur^1][pr.first<<2]+=pr.second;
            f[cur].clear();
            cur^=1;
        }
        for(int j=0;j<m;j++)
        {
            f[cur^1].clear();
            for(auto &pr:f[cur])
            {
                int st=pr.first;
                ll cnt=pr.second;
                int l=getp(st,j),u=getp(st,j+1);
                int base=setp(setp(st,j,0),j+1,0);
                if(!l&&!u)
                {
                    if(i+1<n&&j+1<m)
                        f[cur^1][setp(setp(base,j,1),j+1,2)]+=cnt;
                }
                else if(!l||!u)
                {
                    int v=l?l:u;
                    if(i+1<n)f[cur^1][setp(base,j,v)]+=cnt;
                    if(j+1<m)f[cur^1][setp(base,j+1,v)]+=cnt;
                }
                else if(l==1&&u==2)
                {
                    if(i==n-1&&j==m-1&&base==0)f[cur^1][0]+=cnt;
                }
                else if(l==2&&u==1)f[cur^1][base]+=cnt;
                else if(l==1&&u==1)
                {
                    int p=match_p(st,j+1,m);
                    if(p!=-1)f[cur^1][setp(base,p,1)]+=cnt;
                }
                else if(l==2&&u==2)
                {
                    int p=match_p(st,j,m);
                    if(p!=-1)f[cur^1][setp(base,p,2)]+=cnt;
                }
            }
            cur^=1;
        }
    }
    return f[cur][0];
}

// 铺砖轮廓线：第 j 位表示当前列已被之前的骨牌占用
ll domino_tiling(int n,int m)
{
    vector<ll> g(1<<m,0),h;
    g[0]=1;
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
        {
            h.assign(1<<m,0);
            for(int st=0;st<(1<<m);st++)
            {
                if(!g[st])continue;
                if(st>>j&1)h[st^(1<<j)]+=g[st];
                else
                {
                    if(j+1<m&&!(st>>(j+1)&1))h[st|(1<<(j+1))]+=g[st];
                    if(i+1<n)h[st|(1<<j)]+=g[st];
                }
            }
            g.swap(h);
        }
    return g[0];
}
