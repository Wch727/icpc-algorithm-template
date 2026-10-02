// 哈希表（整数键）：拉链法 + 开放寻址法
// 拉链法 平均 O(1)，天然支持负数；开放寻址法 平均 O(1)，装太满会退化
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100003;         // 容量；质数不能保证无冲突

// ---------------- 拉链法 ----------------
// h[k] 存链表头（结点下标），e[] 存值，nxt[] 存后继，idx 从 0 开始
struct HashChain{
    int h[N],e[N],nxt[N],idx;
    void init()
    {
        memset(h,-1,sizeof(h));   // -1 表示空
        idx=0;
    }
    int get_key(int x)
    {
        return (x%N+N)%N;         // 负数取模也要落在 [0,N)
    }
    void insert(int x)            // O(1) 平均
    {
        int k=get_key(x);
        e[idx]=x,nxt[idx]=h[k],h[k]=idx++;
    }
    bool find(int x)              // O(1) 平均
    {
        int k=get_key(x);
        for(int i=h[k];i!=-1;i=nxt[i])
            if(e[i]==x)return true;
        return false;
    }
};

// ---------------- 开放寻址法 ----------------
// h[k] 为 null 表示空位；线性探测，key 冲突就往后找
// 注意：表要开 2~3 倍，装载因子太高会退化成 O(n)
struct HashOpen{
    const int null=0x3f3f3f3f;    // 空标记，保证没有输入等于它
    int h[N];
    void init()
    {
        memset(h,0x3f,sizeof(h));
    }
    int get_key(int x)
    {
        return (x%N+N)%N;
    }
    // 已有位置或首个空位；表满且不存在时返回 -1。x 不能等于 null。
    int find(int x)
    {
        int k=get_key(x);
        int start=k;
        while(h[k]!=null&&h[k]!=x)
        {
            k++;
            if(k==N)k=0;          // 走到表尾绕回表头
            if(k==start)return -1;
        }
        return k;
    }
    bool insert(int x)            // 重复插入只留一份；满表返回 false
    {
        if(x==null)return false;
        int p=find(x);
        if(p<0)return false;
        h[p]=x;
        return true;
    }
    bool count(int x)
    {
        if(x==null)return false;
        int p=find(x);
        return p>=0&&h[p]==x;
    }
};

HashChain hc;
HashOpen ho;
int n,x;
