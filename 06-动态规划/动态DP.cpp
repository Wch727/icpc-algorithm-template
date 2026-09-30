#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=2005;
const ll NEG=-4e18;
int n,q;
ll w[N];// 点权
vector<int> adj[N];

// ===== 转移矩阵 =====
// g0[u]=轻儿子的 max(f0[v],f1[v]) 之和，g1[u]=w[u]+轻儿子的 f0[v] 之和
// f0[u]=g0[u]+max(f0[重儿子],f1[重儿子])，f1[u]=g1[u]+f0[重儿子]
// 写成矩阵： (f0[u];f1[u]) = [g0 g0; g1 -inf] * (f0[son];f1[son])
// 答案 = max(f0[根],f1[根])
struct Matrix
{
    ll a[2][2];

    Matrix operator*(const Matrix &o)const
    {
        Matrix c;
        c.a[0][0]=max(a[0][0]+o.a[0][0],a[0][1]+o.a[1][0]);
        c.a[0][1]=max(a[0][0]+o.a[0][1],a[0][1]+o.a[1][1]);
        c.a[1][0]=max(a[1][0]+o.a[0][0],a[1][1]+o.a[1][0]);
        c.a[1][1]=max(a[1][0]+o.a[0][1],a[1][1]+o.a[1][1]);
        return c;
    }
};

int fa[N],dep[N],sz[N],son[N],head[N],pos_of[N],dfn[N],tim;
ll g0[N],g1[N];
int nchain;
vector<int> chnode[N];// 每条链上的点，顺序：链头 -> 链尾
vector<int> chid[N];// 点 u 在链内的下标（链头是 0）
vector<Matrix> tr[N];
