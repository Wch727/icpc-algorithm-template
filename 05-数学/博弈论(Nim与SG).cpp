// 适用：公平、有限、无平局的正常玩法，不能行动者输；独立子游戏 SG 值异或。
// 下标：堆 a[1..n]，石子数非负；SG 下标 x 是剩余石子数，0<=x<N。
// 参数：get_sg 的 k 是单次最多取数，0<=k<N；改变 k 前必须 sg_init。
// 关键：mex 取所有后继 SG 中未出现的最小非负整数，终态 sg[0]=0。
// 易错：Nim 任意取数结论不能直接套到限取游戏或最后取者输；__builtin_clz 不接受 0。
// 结论：Nim 异或和为 0 为必败态；首次求 SG 至多 O(x*k)，递归深度 O(x)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;

int n;
int a[N];// 每堆石子数

// Nim 博弈：异或和非 0 先手必胜，O(n)
// 取石子游戏，每步可从任意一堆取任意多个
// O(n)，判断普通 Nim 先手胜负；a 的有效下标是 1..n。
bool nim_win(int n,int a[])
{
    int s=0;
    for(int i=1;i<=n;i++)s^=a[i];
    return s!=0;
}

// O(n)，Nim 的必胜第一步：返回 (堆编号, 取后剩余)，无必胜步返回 0
// 原理：找最高位，把某堆改成 a[i]^(s) 使异或和为 0
// O(n)，返回堆编号及取后剩余数；返回 (0,0) 表示没有必胜步，不是取走数量。
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
// 首次 O(x*k)，命中缓存 O(1)；x 是石子数，k 固定，局部 vis 避免递归互相污染。
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
// O(N)，整张 SG 表置 -1 再设终态；更换游戏取数规则必须重置。
void sg_init()
{
    memset(sg,-1,sizeof(sg));
    sg[0]=0;
}
