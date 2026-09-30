#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int INF=0x3f3f3f3f;

// 种类并查集(3n 拆点)：x 同类域 / x+n x 的猎物 / x+n*2 x 的天敌，n 个点要开 3n
// 单次操作近似 O(alpha(n))，只能表示"同类/吃"这种循环关系
template<int NMAX=N>
struct KindDSU
{
    int fa[NMAX*3],n;
    void init(int n_)
    {
        n=n_;
        for(int i=1;i<=n*3;i++)fa[i]=i;
    }
    int find(int x)
    {
        return fa[x]==x?x:fa[x]=find(fa[x]);
    }
    void unionn(int x,int y)
    {
        fa[find(x)]=find(y);
    }
    // 声明 x,y 同类，返回 false 表示与已有关系矛盾
    bool same(int x,int y)
    {
        if(find(x)==find(y+n)||find(x)==find(y+2*n))return false;//已经互相吃
        unionn(x,y),unionn(x+n,y+n),unionn(x+2*n,y+2*n);
        return true;
    }
    // 声明 x 吃 y，返回 false 表示矛盾
    bool eat(int x,int y)
    {
        if(find(x)==find(y)||find(x)==find(y+n))return false;//同类或已被 y 吃
        unionn(x+n,y),unionn(x,y+2*n),unionn(x+2*n,y+n);
        return true;
    }
};

// 边权并查集：d[x] = val[x]-val[fa[x]]，find 时先递归到根再累加权值
// merge(x,y,w) 表示 val[y]-val[x]=w；mod>0 时权值对 mod 取模(种类并查集用 mod=3，食物链里 2 表示 x 吃 y)
template<int NMAX=N>
struct WeightDSU
{
    int fa[NMAX],d[NMAX],mod;
    int norm(int x)
    {
        if(mod==0)return x;
        return ((x%mod)+mod)%mod;
    }
    void init(int n,int m=0)
    {
        mod=m;
        for(int i=1;i<=n;i++)fa[i]=i,d[i]=0;
    }
    int find(int x)
    {
        if(fa[x]==x)return x;
        int f=fa[x];
        fa[x]=find(f);
        d[x]=norm(d[x]+d[f]);//val[x]-val[根]
        return fa[x];
    }
    // 加入关系 val[y]-val[x]=w，返回 false 表示与已有关系矛盾
    bool merge(int x,int y,int w)
    {
        int fx=find(x),fy=find(y);
        if(fx==fy)return norm(d[y]-d[x])==norm(w);
        fa[fy]=fx;
        d[fy]=norm(d[x]+w-d[y]);//解出 val[fy]-val[fx]
        return true;
    }
    // 同集合返回 val[y]-val[x]，否则返回 INF
    int query(int x,int y)
    {
        if(find(x)!=find(y))return INF;
        return norm(d[y]-d[x]);
    }
};

int n,vmod;
int gid[305],val[305];//暴力：每个点的集合编号 + 在该集合内的"绝对"权值

int norm(int x)
{
    if(vmod==0)return x;
    return ((x%vmod)+vmod)%vmod;
}
