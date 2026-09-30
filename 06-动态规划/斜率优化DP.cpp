#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll INF=4e18;
ll n,L;
ll a[N],s[N],f[N],x[N],y[N],A[N],B[N],C[N];
int q[N];// 下凸壳用的单调队列，存下标
// 经典转移（P3195 玩具装箱）：
//   f[i]=min_{j<i} f[j]+(s[i]-s[j]-L)^2 ，其中 s[i] 是 c 的前缀和、L 是常数
// 展开成只和 j 有关的线性函数：
//   令 X=s[j]、Y=f[j]+s[j]^2、k=2*(s[i]-L)、C=(s[i]-L)^2
//   则 f[i]=min_j (Y - k*X) + C ，即把直线 y=kx 从下方去切点集，取最小截距
// 本题 s 单调递增 -> X 递增（加点单调），k 递增（询问单调），可以单调队列 O(n)
ll sq(ll v)
{
    return v*v;
}
