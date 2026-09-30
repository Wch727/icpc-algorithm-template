// 二叉搜索树 BST（数组版，无旋）：插入 / 删除 / 排名 / 第k小 / 前驱后继，维护 size
// 约定：不允许重复键（第二次数相同值直接忽略），随机数据下期望 O(log n)，退化会变 O(n)
// 排名约定：get_rank(x) 返回「比 x 小的数个数」（0 起）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
const int INF=0x3f3f3f3f;
int n,op,x;

struct BST{
    int val[N],sz[N],ls[N],rs[N];   // 0 号结点是空结点
    int tot;
    void clear(){tot=0;}
    int new_node(int x)
    {
        tot++;
        val[tot]=x,sz[tot]=1,ls[tot]=0,rs[tot]=0;
        return tot;
    }
    void push_up(int p){sz[p]=sz[ls[p]]+sz[rs[p]]+1;}
    // 插入 x，重复值忽略；返回新的根
    int insert(int p,int x)
    {
        if(p==0)return new_node(x);
        if(x<val[p])ls[p]=insert(ls[p],x);
        else if(x>val[p])rs[p]=insert(rs[p],x);
        else return p;              // 已存在，不动
        push_up(p);
        return p;
    }
    // 删除 x，返回新的根；用左子树最大值替换（前驱顶替）
    int erase(int p,int x)
    {
        if(p==0)return 0;
        if(x<val[p])ls[p]=erase(ls[p],x);
        else if(x>val[p])rs[p]=erase(rs[p],x);
        else
        {
            if(ls[p]==0)return rs[p];
            if(rs[p]==0)return ls[p];
            int q=ls[p];
            while(rs[q])q=rs[q];    // 找前驱，把它换到 p
            val[p]=val[q];
            ls[p]=erase(ls[p],val[q]);
        }
        push_up(p);
        return p;
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
    // 比 x 小的数个数（0 起），O(树高)
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
    // 小于 x 的最大值，不存在返回 -INF
    int get_pre(int p,int x)
    {
        int ans=-INF;
        while(p)
        {
            if(val[p]<x)ans=val[p],p=rs[p];
            else p=ls[p];
        }
        return ans;
    }
    // 大于 x 的最小值，不存在返回 INF
    int get_next(int p,int x)
    {
        int ans=INF;
        while(p)
        {
            if(val[p]>x)ans=val[p],p=ls[p];
            else p=rs[p];
        }
        return ans;
    }
};

BST t;
int root;
vector<int> bf;                 // 暴力容器：始终保持有序

void bf_insert(int x)
{
    if(binary_search(bf.begin(),bf.end(),x))return;
    bf.insert(lower_bound(bf.begin(),bf.end(),x),x);
}
void bf_erase(int x)
{
    vector<int>::iterator it=lower_bound(bf.begin(),bf.end(),x);
    if(it!=bf.end()&&*it==x)bf.erase(it);
}
int bf_rank(int x)              // 比 x 小的个数
{
    return lower_bound(bf.begin(),bf.end(),x)-bf.begin();
}
int bf_kth(int k)
{
    if(k<1||k>(int)bf.size())return 0;
    return bf[k-1];
}
int bf_pre(int x)
{
    int pos=lower_bound(bf.begin(),bf.end(),x)-bf.begin()-1;
    return pos>=0?bf[pos]:-INF;
}
int bf_next(int x)
{
    int pos=upper_bound(bf.begin(),bf.end(),x)-bf.begin();
    return pos<(int)bf.size()?bf[pos]:INF;
}
