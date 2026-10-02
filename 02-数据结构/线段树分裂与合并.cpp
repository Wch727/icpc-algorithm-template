// 动态权值线段树，多重集频数非负；拆出 [L,R] 返回新根，原根按引用更新。
// split O(log σ) 边界节点，merge 摊还 O(总节点数)；所有操作是破坏性的，根不能共享节点。
// 节点用 vector 扩容；递归调用不得持有 vector 元素引用。值域 0..σ-1。
#include<bits/stdc++.h>
using namespace std;
struct SplitMergeSeg
{
    struct Node{int l=0,r=0;long long sum=0;};vector<Node> t={{}};int sigma;
    SplitMergeSeg(int sigma):sigma(sigma){assert(sigma>0);}
    int node(){t.push_back({});return t.size()-1;}
    void pull(int p){t[p].sum=t[t[p].l].sum+t[t[p].r].sum;}
    int add(int p,int l,int r,int x,int delta){if(!p)p=node();if(l==r){t[p].sum+=delta;assert(t[p].sum>=0);return p;}int m=(l+r)/2;if(x<=m){int q=add(t[p].l,l,m,x,delta);t[p].l=q;}else{int q=add(t[p].r,m+1,r,x,delta);t[p].r=q;}pull(p);return p;}
    int split(int &p,int l,int r,int L,int R)
    {
        if(!p||R<l||r<L)return 0;if(L<=l&&r<=R){int q=p;p=0;return q;}
        int q=node(),m=(l+r)/2,a=t[p].l,b=t[p].r;
        int x=split(a,l,m,L,R),y=split(b,m+1,r,L,R);
        t[p].l=a;t[p].r=b;t[q].l=x;t[q].r=y;pull(p);pull(q);return q;
    }
    int merge(int a,int b,int l,int r){if(!a||!b)return a?a:b;if(l==r)t[a].sum+=t[b].sum;else{int m=(l+r)/2;t[a].l=merge(t[a].l,t[b].l,l,m);t[a].r=merge(t[a].r,t[b].r,m+1,r);pull(a);}return a;}
};
