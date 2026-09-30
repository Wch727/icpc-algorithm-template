#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll mod=1000000007;

int M=2;// 矩阵阶数，用之前改成实际大小

// 矩阵乘法，O(M^3)，取模版
struct Matrix{
    ll a[10][10];
    Matrix(){memset(a,0,sizeof(a));}
};

// 单位矩阵
Matrix I()
{
    Matrix r;
    for(int i=1;i<=M;i++)r.a[i][i]=1;
    return r;
}

Matrix mul(Matrix x,Matrix y)
{
    Matrix r;
    for(int i=1;i<=M;i++)
        for(int k=1;k<=M;k++)
        {
            if(!x.a[i][k])continue;
            for(int j=1;j<=M;j++)
                r.a[i][j]=(r.a[i][j]+x.a[i][k]*y.a[k][j])%mod;
        }
    return r;
}

// 矩阵快速幂，O(M^3 log n)
Matrix qpow(Matrix base,ll n)
{
    Matrix res=I();
    while(n)
    {
        if(n&1)res=mul(res,base);
        base=mul(base,base);
        n>>=1;
    }
    return res;
}

// 斐波那契：F(1)=F(2)=1
// [F(n+1) F(n)]^T = [[1,1],[1,0]]^(n-1) * [F(2) F(1)]^T
ll fib(ll n)
{
    if(n<=0)return 0;
    if(n<=2)return 1;
    Matrix A;
    A.a[1][1]=1,A.a[1][2]=1,A.a[2][1]=1,A.a[2][2]=0;
    Matrix r=qpow(A,n-1);
    return r.a[1][1];
}

// 朴素递推，O(n)，当对拍基准
ll fib_naive(ll n)
{
    ll x=1,y=1;
    if(n<=0)return 0;
    if(n<=2)return 1;
    for(ll i=3;i<=n;i++)
    {
        ll z=(x+y)%mod;
        x=y,y=z;
    }
    return y;
}

int main()
{
    M=2;
    // 1) 小范围与朴素递推对拍
    int bad=0;
    for(ll n=0;n<=200;n++)
        if(fib(n)!=fib_naive(n))bad++;
    // 2) 大 n 用快速倍增公式 F(2k)=F(k)*(2F(k+1)-F(k)), F(2k+1)=F(k)^2+F(k+1)^2 对拍
    ll x=1,y=1;
    for(ll n=1;n<=500000;n++)
    {
        if(fib(n)!=(n==1?1:x))bad++;
        ll z=(x+y)%mod;
        x=y,y=z;
    }
    printf("fib(10) = %lld\n",fib(10));
    printf("fib(50) = %lld\n",fib(50));
    printf("fib(1e18) = %lld\n",fib(1000000000000000000LL));
    printf("fib(100) naive = %lld quick = %lld\n",fib_naive(100),fib(100));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1962 输入 10 -> 55；输入 1000000000000000000 -> 517691607

/*
自测记录：
  1) n <= 200 与 O(n) 朴素递推对拍；
  2) n <= 500000 与增量递推对拍（含大 n 前的全部值）。
*/
