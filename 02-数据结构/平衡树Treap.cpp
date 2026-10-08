// 平衡树 Treap（带旋）：插入 / 删除 / 排名 / 第k小 / 前驱后继
// 每个结点随机一个优先值，小根堆性质 → 期望 O(log n)，最坏 O(n)
// 重复插入忽略（集合），排名约定：get_rank(x) 返回「比 x 小的数个数」（0 起）
// 关键坑：旋转必须先用 int &q=ls[p] 绑好引用，递归返回后 ls[p] 可能已经不是原来那个结点了
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
const int INF=0x3f3f3f3f;
int n,x;

// xorshift 伪随机，比 rand() 快
unsigned long long seed=20240513;

int rand_int()
{
    seed^=seed<<7;
    seed^=seed>>9;
    return (int)(seed%1000000007);
}

struct Treap{
    int val[N],pri[N],sz[N],ls[N],rs[N];   // 0 号是空结点
    int tot;
    void clear(){tot=sz[0]=ls[0]=rs[0]=0;}
    int new_node(int x)
    {
        tot++;
        val[tot]=x,pri[tot]=rand_int(),sz[tot]=1,ls[tot]=0,rs[tot]=0;
        return tot;
    }
    void push_up(int p){sz[p]=sz[ls[p]]+sz[rs[p]]+1;}
    // 左旋：p 的右儿子转上来
    void zig(int &p)
    {
        int q=rs[p];
        rs[p]=ls[q],ls[q]=p,p=q;
        push_up(ls[p]),push_up(p);
    }
    // 右旋：p 的左儿子转上来
    void zag(int &p)
    {
        int q=ls[p];
        ls[p]=rs[q],rs[q]=p,p=q;
        push_up(rs[p]),push_up(p);
    }
    void insert(int &p,int x)
    {
        if(p == 0)
        {
            p= new_node(x);
            return;
        }
        if(x<val[p])
        {
            int &q=ls[p];                  // 先绑引用，旋转后 ls[p] 会变
            insert(q,x);
            if(pri[q]<pri[p])zag(p);
        }
        else if(x>val[p])
        {
            int &q=rs[p];
            insert(q,x);
            if(pri[q]<pri[p])zig(p);
        }
        else return;                       // 已有相同值（多重集合应另加出现次数，不要只改比较号）
        push_up(p);
    }
    // 把 p 一路旋转到叶子再摘掉
    void erase(int &p,int x)
    {
        if(p==0)return;
        if(x<val[p])erase(ls[p],x);
        else if(x>val[p])erase(rs[p],x);
        else
        {
            if(ls[p]==0)p=rs[p];
            else if(rs[p]==0)p=ls[p];
            else if(pri[ls[p]]<pri[rs[p]])zag(p),erase(rs[p],x);
            else zig(p),erase(ls[p],x);
        }
        if(p)push_up(p);
    }
    bool find(int p,int x)
    {
        while(p)
        {
            if(x<val[p])p=ls[p];
            else if(x>val[p])p=rs[p];
            else return true;
        }
        return false;
    }
    // 比 x 小的数个数（0 起）
    int get_rank(int p,int x)
    {
        int cnt=0;
        while(p)
        {
            if(x<=val[p])p=ls[p];
            else cnt+=sz[ls[p]]+1,p=rs[p];
        }
        return cnt;
    }
    // 第 k 小（k 从 1 开始），不存在返回 0
    int kth(int p,int k)
    {
        if(k<1||k>sz[p])return 0;
        while(p)
        {
            if(k<=sz[ls[p]])p=ls[p];
            else if(k==sz[ls[p]]+1)return val[p];
            else k-=sz[ls[p]]+1,p=rs[p];
        }
        return 0;
    }
    int get_pre(int p,int x)               // 小于 x 的最大值
    {
        int ans=-INF;
        while(p)
        {
            if(val[p]<x)ans=val[p],p=rs[p];
            else p=ls[p];
        }
        return ans;
    }
    int get_next(int p,int x)              // 大于 x 的最小值
    {
        int ans=INF;
        while(p)
        {
            if(val[p]>x)ans=val[p],p=ls[p];
            else p=rs[p];
        }
        return ans;
    }
    int get_min(int p)
    {
        while(ls[p])p=ls[p];
        return val[p];
    }
    int get_max(int p)
    {
        while(rs[p])p=rs[p];
        return val[p];
    }
    void print_inorder(int p)              // 中序，调试用
    {
        if(!p)return;
        print_inorder(ls[p]);
        printf(" %d",val[p]);
        print_inorder(rs[p]);
    }
};

Treap t;
int root;
