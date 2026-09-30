#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
const int INF=0x3f3f3f3f;
const int NEG=-0x3f3f3f3f;
int n,m;
int a[N];
char s[N],t[N];
int f[N],g[N],dp[N][N],best[N];// f/g 是滚动的线性 dp 数组
int tri[15][15];// 数字三角形
int grid[15][15];// 网格路径（1 表示障碍）
