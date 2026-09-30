#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=505;// 断环成链后要开 2n
const int INF=0x3f3f3f3f;
const int NEG=-0x3f3f3f3f;
int n;
int a[N],sum[N];// a 存石子/项链，sum 是前缀和
int gmin[N][N],gmax[N][N];// 环形石子合并
int emax[N][N];// 能量项链
int pmax[N][N],pmin[N][N];// 多边形游戏：有负数乘法，必须同时存 max 和 min
int val[N];// 多边形顶点
char op[N];// 多边形边上的运算符，op[i] 连 val[i] 和 val[i+1]
int bv[N];// 暴力用的临时环
