// 适用：固定阶线性递推与状态转移重复执行，指数很大时用平方加速。
// 下标：Matrix.a[1..M][1..M]，数组大小 10，故 1<=M<=9；fib 必须使用 M=2。
// 参数：qpow 的 n 是非负指数，矩阵元素先归一到 [0,mod)。
// 关键：乘法不交换，res*base 的次序必须符合既定状态列向量/行向量约定。
// 结论：零次幂是单位矩阵；斐波那契初值 F(1)=F(2)=1，n<=0 返回 0。
// 复杂度：矩阵乘 O(M^3)，幂 O(M^3 log n)，空间 O(M^2)；不要把 M 开到数组容量之外。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const ll mod=1000000007;

int M=2;// 矩阵阶数，用之前改成实际大小

// 矩阵乘法，O(M^3)，取模版
struct Matrix{
    ll a[10][10];
    // O(1)，固定 10*10 存储清零；有效阶数来自全局 M。
    Matrix(){memset(a,0,sizeof(a));}
};

// 单位矩阵
// O(M)，构造对角为 1 的单位矩阵，配合零次幂。
Matrix I()
{
    Matrix r;
    for(int i=1;i<=M;i++)r.a[i][i]=1;
    return r;
}

// O(M^3)，x/y 同为 M 阶，返回 x*y；跳过零项节省无效乘加。
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
// O(M^3 log n)，base 为转移矩阵，n>=0；返回 base 的 n 次幂。
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
// O(log n)，需 M=2；使用 [[1,1],[1,0]]，返回第 n 项 mod mod。
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
// O(n)，线性递推作为小规模参考，不依赖 M，返回同一初值约定的第 n 项。
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
