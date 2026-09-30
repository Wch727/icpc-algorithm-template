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
