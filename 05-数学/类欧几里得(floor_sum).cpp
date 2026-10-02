#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

// Σ_{i=0}^{n-1} floor((a*i+b)/m)，n>=0、m>0，a/b 可负，返回值须放入 ll。
// C++ 负数除法向 0 截断，先改为向下取整；a*n+b 与累计用 __int128。
// 去掉整商后交换横纵轴，类似欧几里得；O(log m)，空间 O(1)。
// FS 记本函数，u=(f*t+x) mod m、v=(g*t+y) mod m，余数规范到 [0,m)。
// #满足 u<v (0<=t<n)=n-FS(n,m,g,y)+FS(n,m,f,x)+FS(n,m,g-f,y-x-1)。
// 因为 [u<v]=1+floor((v-u-1)/m)；第三项可能有负参数，f/g/x/y 的差也须防溢出。
ll floor_sum(ll n,ll m,ll a,ll b)
{
    assert(n>=0&&m>0);
    ll qa=a/m,qb=b/m;
    a%=m,b%=m;
    if(a<0)a+=m,--qa;
    if(b<0)b+=m,--qb;
    __int128 ans=(__int128)n*(n-1)/2*qa+(__int128)n*qb;
    while(1)
    {
        if(a>=m)ans+=(__int128)n*(n-1)/2*(a/m),a%=m;
        if(b>=m)ans+=(__int128)n*(b/m),b%=m;
        __int128 y=(__int128)a*n+b;
        if(y<m)break;
        n=y/m,b=y%m,swap(a,m);
    }
    assert(LLONG_MIN<=ans&&ans<=LLONG_MAX);
    return (ll)ans;
}
