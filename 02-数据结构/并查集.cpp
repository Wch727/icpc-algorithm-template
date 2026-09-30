#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 并查集：路径压缩 + 按大小合并，单次操作近似 O(alpha(n))
template<int NMAX=N>
struct DSU
{
    int fa[NMAX],sz[NMAX];
    void init(int n)//1..n 各自成一个集合
    {
        for(int i=1;i<=n;i++)fa[i]=i,sz[i]=1;
    }
    int find(int x)
    {
        return fa[x]==x?x:fa[x]=find(fa[x]);
    }
    bool same(int x,int y)
    {
        return find(x)==find(y);
    }
    void unionn(int x,int y)
    {
        int fx=find(x),fy=find(y);
        if(fx==fy)return;
        if(sz[fx]<sz[fy])swap(fx,fy);//小的树挂到大树下面
        fa[fy]=fx,sz[fx]+=sz[fy];
    }
    int size(int x)//x 所在集合大小
    {
        return sz[find(x)];
    }
};

int n;
DSU<305> d;
int id[305];//暴力：直接记每个点当前属于哪个连通块
