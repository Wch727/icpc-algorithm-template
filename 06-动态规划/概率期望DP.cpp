#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
int n;
double p;// 每步向目标前进的概率
double E[N];// 期望步数
double P[N];// 当前时刻各位置的概率
double A[N][N],bvec[N],sol[N];// 线性方程组用
