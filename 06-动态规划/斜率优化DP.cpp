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
// a>=0；前缀和、平方、f/y/k 都须放入 ll。
// 本题 s 单调不减 -> X 递增（加点单调），k 递增（询问单调），可以单调队列 O(n)
ll sq(ll v){return v*v;}

// 单调队列维护下凸壳，O(n) 斜率优化
// 叉积 cross(o,a,b)=(x[a]-x[o])*(y[b]-y[o])-(y[a]-y[o])*(x[b]-x[o])
// cross(o,a,b)<=0 表示 a 在 ob 连线之上，下凸壳要把 a 弹掉
ll slope_dp()
{
    s[0]=f[0]=0;
    for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
    int head=0,tail=0;
    x[0]=0,y[0]=0;// f[0]=0
    q[tail++]=0;
    for(int i=1;i<=n;i++)
    {
        ll k=2*(s[i]-L);
        while(head+1<tail&&(__int128)y[q[head+1]]-y[q[head]]<=(__int128)k*(x[q[head+1]]-x[q[head]]))head++;
        int j=q[head];
        f[i]=(ll)((__int128)y[j]-(__int128)k*x[j]+(__int128)(s[i]-L)*(s[i]-L));
        x[i]=s[i],y[i]=f[i]+sq(s[i]);
        if(tail>head&&x[q[tail-1]]==x[i])
        {
            if(y[q[tail-1]]<=y[i])continue;
            --tail;
        }
        while(head+1<tail)
        {
            int o=q[tail-2],p=q[tail-1];
            if((__int128)(x[p]-x[o])*((__int128)y[i]-y[o])-((__int128)y[p]-y[o])*(x[i]-x[o])<=0)tail--;
            else break;
        }
        q[tail++]=i;
    }
    return f[n];
}

// 把转移式乘开成 A[i]*B[j]+C[i] 的标准形式，方便套李超线段树/一般凸壳
// f[i]=min_j (f[j]+s[j]^2+2*L*s[j] - 2*s[i]*s[j]) + s[i]^2 + L^2 - 2*L*s[i]
void build_ab()
{
    s[0]=f[0]=0;
    for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
    for(int j=0;j<=n;j++)A[j]=-2*s[j],B[j]=f[j]+s[j]*s[j]+2*L*s[j];// 斜率、截距
    for(int i=1;i<=n;i++)C[i]=s[i]*s[i]+L*L-2*L*s[i];// 与 j 无关的常数
}
