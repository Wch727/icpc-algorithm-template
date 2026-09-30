#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=205;
const int M=25;
const ll INF=4e18;
int n,K;
ll a[N],s[N];
ll w[N][N];// w[l][r]=(a[l]+...+a[r])^2
ll fd[N][N],bk[N][N];// 分治写法 / 暴力写法的 dp 表
int opt[N][N];// 决策点表
int dq[N*N],hd,tl;// 单调队列写法用的队列
