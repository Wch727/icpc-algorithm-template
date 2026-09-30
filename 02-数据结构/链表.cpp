// 数组模拟双向链表 / 静态链表
// 用 pre/nxt 两个数组当指针，插入删除 O(1)，按下标随机访问 O(1)
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m;

// 数组模拟双向链表：h 是头结点编号，t 是尾结点编号，0 表示空
// 结点编号就是 val（值为 i 的结点编号 i），删除时按编号删
struct Link{
    int val[N],pre[N],nxt[N];
    bool used[N];        // 结点是否还在链表里
    int h,t;
    void init()
    {
        for(int i=1;i<N;i++)used[i]=false;
        h=0,t=0;
    }
    // 空表时单独处理，避免维护哨兵
    void push_back(int id,int v)
    {
        val[id]=v,used[id]=true;
        pre[id]=t,nxt[id]=0;
        if(t)nxt[t]=id;
        else h=id;
        t=id;
    }
    int insert_right(int p,int id,int v)   // 在 p 右边插入，O(1)
    {
        val[id]=v,used[id]=true;
        nxt[id]=nxt[p],pre[id]=p;
        if(nxt[p])pre[nxt[p]]=id;
        nxt[p]=id;
        if(t==p)t=id;
        return id;
    }
    int insert_left(int p,int id,int v)    // 在 p 左边插入，O(1)
    {
        val[id]=v,used[id]=true;
        pre[id]=pre[p],nxt[id]=p;
        if(pre[p])nxt[pre[p]]=id;
        pre[p]=id;
        if(h==p)h=id;
        return id;
    }
    void erase(int id)                     // O(1)，已删过就跳过（P1160 会重复删）
    {
        if(!used[id])return;
        used[id]=false;
        if(pre[id])nxt[pre[id]]=nxt[id];
        else h=nxt[id];
        if(nxt[id])pre[nxt[id]]=pre[id];
        else t=pre[id];
    }
    bool empty(){return h==0;}
    int size()
    {
        int c=0;
        for(int p=h;p;p=nxt[p])c++;
        return c;
    }
    void print()                           // 从头到尾
    {
        for(int p=h;p;p=nxt[p])printf("%d ",val[p]);
        printf("\n");
    }

    void print_rev()                       // 从尾到头，顺便验证 pre 指针
    {
        for(int p=t;p;p=pre[p])printf("%d ",val[p]);
        printf("\n");
    }
};

Link L;
