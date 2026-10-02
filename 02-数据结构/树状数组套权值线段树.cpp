// 在线点修改、区间第 k 小：位置 0..n-1，值域离散化为 0..σ-1，k 从 1 开始。
// 所有可能修改值预先加入压缩表；BIT 套动态权值线段树，O(log n log σ) 每次。
// roots 中存频数；change(pos,old,new) 前 old 必须确实是当前位置值。内存 O((n+q)log n log σ) 上界。
#include<bits/stdc++.h>
using namespace std;
struct BITValueTree
{
    struct Node{int l=0,r=0,sum=0;};int n,sigma;vector<int> root;vector<Node> t={{}};
    BITValueTree(int n,int sigma):n(n),sigma(sigma),root(n+1){assert(sigma>0);}
    int add(int p,int l,int r,int x,int d)
    {
        if(!p){p=t.size();t.push_back({});}t[p].sum+=d;
        if(l<r){int m=(l+r)/2;if(x<=m){int z=add(t[p].l,l,m,x,d);t[p].l=z;}else{int z=add(t[p].r,m+1,r,x,d);t[p].r=z;}}
        return p;
    }
    void add(int pos,int value,int delta){assert(0<=pos&&pos<n&&0<=value&&value<sigma);for(int i=pos+1;i<=n;i+=i&-i)root[i]=add(root[i],0,sigma-1,value,delta);}
    void change(int pos,int old,int value){add(pos,old,-1);add(pos,value,1);}
    int kth(int l,int r,int k)const// 闭区间
    {
        assert(0<=l&&l<=r&&r<n&&1<=k&&k<=r-l+1);vector<int> a,b;
        for(int i=r+1;i;i-=i&-i)a.push_back(root[i]);for(int i=l;i;i-=i&-i)b.push_back(root[i]);
        int L=0,R=sigma-1;
        while(L<R)
        {int count=0,m=(L+R)/2;for(int p:a)count+=t[t[p].l].sum;for(int p:b)count-=t[t[p].l].sum;
            bool left=k<=count;if(!left)k-=count;for(int &p:a)p=left?t[p].l:t[p].r;for(int &p:b)p=left?t[p].l:t[p].r;if(left)R=m;else L=m+1;}
        return L;
    }
};
