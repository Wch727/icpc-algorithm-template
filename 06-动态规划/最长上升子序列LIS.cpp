#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int INF=0x3f3f3f3f;
int n;
int a[N],d[N],f[N],pre[N],tmp[N],mp[N],c[N];
int st[N];// 还原方案用的栈
int lcsf[15][15];
