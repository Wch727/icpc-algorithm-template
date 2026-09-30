#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=110;
const int K=105;// 每行合法状态数上限(m<=10 时约 60 个)
const int NS=1<<15;// TSP 状态数上限
const int NEG=-0x3f3f3f3f;
int n,m,top;
int mp[N];// 地形压缩，1 表示山地不能放
int S[1<<10],cnt[1<<10];// 合法状态 和它里面 1 的个数
int dp[2][K][K];// 滚动两行，dp[i][j][k]：第 i 行状态 j、第 i-1 行状态 k
double px[20],py[20];// 吃奶酪的坐标，0 号是原点
double f[NS][16];// f[状态][当前点]，走完集合的最小距离
