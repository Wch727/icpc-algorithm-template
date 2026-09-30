#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
int n,m;
char a[N],b[N];
int f[N];// 一维滚动数组
int g[N][N];// LCS 二维表，还原方案用
int st[N];// 还原方案用的栈
