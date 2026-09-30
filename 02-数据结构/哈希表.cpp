// 哈希表（整数键）：拉链法 + 开放寻址法
// 拉链法 平均 O(1)，天然支持负数；开放寻址法 平均 O(1)，装太满会退化
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100003;         // 取质数，模冲突最少

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
    // 返回 x 所在位置；没找到时返回 x 应该插入的第一个空位
    int find(int x)
    {
        int k=get_key(x);
        while(h[k]!=null&&h[k]!=x)
        {
            k++;
            if(k==N)k=0;          // 走到表尾绕回表头
        }
        return k;
    }
    void insert(int x)            // 重复插入同一个数只留一份
    {
        h[find(x)]=x;
    }
    bool count(int x)
    {
        return h[find(x)]==x;
    }
};

HashChain hc;
HashOpen ho;
int n,x;
