#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
const int N=105;
const int E=1005;// 边的上限

int n,m;
ll mod=1000000007;

// O(log p)，快速幂
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
ll det_mod(ll a[N][N],int n,ll p)
{
    ll res=1;
    for(int c=1;c<=n;c++)
    {
        int pivot=-1;
        for(int i=c;i<=n;i++)
            if(a[i][c]%p){pivot=i;break;}
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
    return (res%p+p)%p;
}

// O(n! )，按第一行展开求行列式，只用于小矩阵对拍
ll det_naive(ll a[N][N],int n)
{
    if(n==1)return a[1][1];
    ll res=0;
    ll b[N][N];
    for(int j=1;j<=n;j++)
    {
        for(int i=2;i<=n;i++)
        {
            int c=0;
            for(int k=1;k<=n;k++)
            {
                if(k==j)continue;
                b[i-1][++c]=a[i][k];// 划掉第 1 行第 j 列
            }
        }
        ll sub=det_naive(b,n-1);
        if(j&1)res+=a[1][j]*sub;
        else res-=a[1][j]*sub;
    }
    return res;
}

// O(n^2 + n^3)，矩阵树定理：邻接矩阵直接建拉普拉斯矩阵，求任意 n-1 阶主子式
// 返回 n 个点的无向（允许重边、自环不加）生成树个数，边权全为 1
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

// O(2^m)，枚举边集暴力数生成树，对拍用
ll tree_naive(int n,ll g[N][N],int m,int eu[],int ev[])
{
    ll res=0;
    for(int mask=0;mask<(1<<m);mask++)
    {
        if(__builtin_popcount(mask)!=n-1)continue;// 生成树恰好 n-1 条边
        int fa[N];
        for(int i=1;i<=n;i++)fa[i]=i;
        function<int(int)> findd=[&](int x){return fa[x]==x?x:fa[x]=findd(fa[x]);};
        bool ok=true;
        for(int i=1;i<=m;i++)
            if(mask>>(i-1)&1)
            {
                int a=findd(eu[i]),b=findd(ev[i]);
                if(a==b){ok=false;break;}// 出现环
                fa[a]=b;
            }
        if(ok)res++;
    }
    return res;
}

// O(n^3)，用有理数形式的小矩阵行列式（返回可能为负），再取模，交叉验证用
ll det_naive_mod(ll a[N][N],int n,ll p)
{
    ll v=det_naive(a,n);
    return ((v%p)+p)%p;
}
