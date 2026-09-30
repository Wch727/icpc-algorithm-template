#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const int NEG=-0x3f3f3f3f;
int n,m,root,dia;
int h[N],score[N],par[N],vis[N];// 欢乐值 学分 父亲 是否当过儿子
vector<int> son[N],adj[N];
int dp[N][2];// dp[x][0] x 不来 dp[x][1] x 来
int f[N][N];// f[x][j] 以 x 为根的子树里选 j 门课
int d1[N],d2[N];// 最长/次长向下链
