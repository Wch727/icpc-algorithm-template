#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=105;// 组数上限
const int K=1005;// 每组物品数上限
const int VMAX=20005;// 容量上限
const int KN=3;// 每个主件最多 2 个附件
int n,V,g;
int cnt[N],gw[N][K],gv[N][K];// 每组物品数 与 组内物品
int f[VMAX];// 一维滚动数组，f[j] 表示容量 j 的最大价值
int mw[N],mv[N],ac[N];// 主件重量 价值 附件数
int aw[N][KN],av[N][KN];// 附件重量 价值，1-indexed
