#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int INF=0x3f3f3f3f;
int n,k,L,R;
int a[N],f[N],res[N];
int q[N];// 单调队列，存下标（也可以用 deque<int>，见 P1886 写法）
int mx[N],mn[N],bmx[N],bmn[N];// 对拍用的窗口结果
