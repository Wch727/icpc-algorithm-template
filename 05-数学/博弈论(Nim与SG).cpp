#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;

int n;
int a[N];// 每堆石子数

// Nim 博弈：异或和非 0 先手必胜，O(n)
// 取石子游戏，每步可从任意一堆取任意多个
bool nim_win(int n,int a[])
{
    int s=0;
    for(int i=1;i<=n;i++)s^=a[i];
    return s!=0;
}

// O(n)，Nim 的必胜第一步：返回 (堆编号, 取后剩余)，无必胜步返回 0
// 原理：找最高位，把某堆改成 a[i]^(s) 使异或和为 0
pair<int,int> nim_first(int n,int a[])
{
    int s=0;
    for(int i=1;i<=n;i++)s^=a[i];
    if(!s)return make_pair(0,0);
    int hb=31-__builtin_clz(s);// s 的最高位
    for(int i=1;i<=n;i++)
        if(a[i]>>hb&1)
        {
            int t=a[i]^s;
            if(t<a[i])return make_pair(i,t);
        }
    return make_pair(0,0);
}

// SG 函数（记忆化），位置为「剩余石子数」，可取 1..k 个
// 单堆的 SG 值，O(n*k)；vis 必须是每次调用独立的，不能共用 static 数组
int sg[N];
int get_sg(int x,int k)
{
    if(sg[x]>=0)return sg[x];
    bool vis[N];
    for(int i=0;i<=k;i++)vis[i]=false;
    for(int i=1;i<=k&&i<=x;i++)vis[get_sg(x-i,k)]=true;
    int mex=0;
    while(vis[mex])mex++;
    return sg[x]=mex;
}

// 清空 SG 表，调用前初始化（sg[0]=0，其余 -1）
void sg_init()
{
    memset(sg,-1,sizeof(sg));
    sg[0]=0;
}

// 通用 SG：对给定状态转移求 mex，状态转移由调用者提供
// 这里演示「一堆 n 个石子，可取 1..k 个」的整体胜负判定
// 多堆时把各堆 SG 异或即可，O(n*k)

// 暴力博弈：每步从任意一堆取 1..k 个（k=0 表示不限制，即普通 Nim）
// 返回当前局面先手是否必胜；memo 用按 k 分开的记忆化
map<pair<int,vector<int>>,bool> memo;
