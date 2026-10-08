// 适用：无向多重图生成树计数；拉普拉斯任意 n-1 阶主子式相等。
// 下标：矩阵 a/g 使用 1..n，n<N；g[u][v] 是边的重数，须对称。
// 参数：det_mod 的 p 为素数，matrix_tree 使用全局 mod；无向自环不计。
// 关键：度数放对角、邻接重数取负，删去同一编号行列；换行必须翻转行列式符号。
// 易错：det_mod 原地消元，保留原矩阵要先复制；精确 det_naive 和返回值会受 ll 范围限制。
// 复杂度：模行列式 O(n^3+n log p)，空间 O(n^2)；暴力仅供极小图核对。
// 秩一更新：det(I+u*v^T)=1+v^T*u；A 可逆时 det(A+u*v^T)=det(A)*(1+v^T*A^-1*u)。
// 已知精确 |det|=D 只判正负：选不整除 D 的奇素数 p，模行列式比较 ±D；D=0 单独处理。
// 必须是素数才可复用模高斯消元；仅“奇模数”不足以保证主元可逆。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
const int N=105;
const int E=1005;// 边的上限

int n,m;
ll mod=1000000007;

// O(log p)，快速幂
// O(log n)，a 为底数、n>=0 为指数，p 为正模数，模乘先扩宽。
ll qpow(ll a,ll n,ll p)
{
    ll res=1%p;
    a%=p;
    while(n)
    {
        if(n&1)res=(ll)((lll)res*a%p);
        a=(ll)((lll)a*a%p);
        n>>=1;
    }
    return res;
}

// O(n^3)，模素数意义下高斯消元求行列式（不取模则要求 mod 为素数以便求逆）
// O(n^3+n log p)，a 是 n 阶方阵，p 为素数；找不到非零主元返回 0。
ll det_mod(ll a[N][N],int n,ll p)
{
    ll res=1;
    for(int c=1;c<=n;c++)
    {
        int pivot=-1;
        for(int i=c;i<=n;i++)
            if(a[i][c] % p)
            {
                pivot= i;
                break;
            }
        if(pivot==-1)return 0;// 有一列全 0，行列式为 0
        if(pivot!=c)
        {
            for(int j=1;j<=n;j++)swap(a[c][j],a[pivot][j]);
            res=(p-res)%p;// 交换两行，行列式变号
        }
        res=(ll)((lll)res*a[c][c])%p;
        ll inv=qpow(a[c][c],p-2,p);// 主元的逆元
        for(int i=c+1;i<=n;i++)
        {
            if(!a[i][c])continue;
            ll t=(ll)((lll)a[i][c]*inv%p);
            for(int j=c;j<=n;j++)a[i][j]=(a[i][j]-(lll)t*a[c][j])%p;
        }
    }
    return (res%p<0?res%p+p:res%p);
}

// O(n! )，按第一行展开求行列式，只用于小矩阵对拍
// O(n!) 递归展开，n>=1；临时矩阵在递归栈上，只用于小阶数。

// O(n^2 + n^3)，矩阵树定理：邻接矩阵直接建拉普拉斯矩阵，求任意 n-1 阶主子式
// 返回 n 个点的无向（允许重边、自环不加）生成树个数，边权全为 1
// O(n^3+n log mod)，g 为对称重数矩阵；返回模 mod 的生成树数，单点返回 1。
ll matrix_tree(int n,ll g[N][N])
{
    if(n==1)return 1;// 单点视为 1 棵树
    static ll lap[N][N],sub[N][N];
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)lap[i][j]=0;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            if(i!=j&&g[i][j])
            {
                lap[i][j]-=g[i][j];// 邻接矩阵取负
                lap[i][i]+=g[i][j];// 度数放在对角
            }
    for(int i=1;i<=n-1;i++)
        for(int j=1;j<=n-1;j++)sub[i][j]=((lap[i][j]%mod)+mod)%mod;// 去掉第 n 行第 n 列
    return det_mod(sub,n-1,mod);
}
