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

int main()
{
    srand(20240513);

    hc.init(),ho.init();
    // 1. 小数据手测（含负数、0、重复值）
    int a[]={1,0,-1,-1,5,1000000007,-1000000007,3};
    for(int i=0;i<8;i++)
    {
        hc.insert(a[i]);
        ho.insert(a[i]);
    }
    printf("chain: 1=%d -1=%d 7=%d 1e9+7=%d -1e9-7=%d\n",
        (int)hc.find(1),(int)hc.find(-1),(int)hc.find(7),(int)hc.find(1000000007),(int)hc.find(-1000000007));
    printf("open : 1=%d -1=%d 7=%d 1e9+7=%d -1e9-7=%d\n",
        (int)ho.count(1),(int)ho.count(-1),(int)ho.count(7),(int)ho.count(1000000007),(int)ho.count(-1000000007));

    // 2. 随机对拍：两个哈希表都对照 set
    for(int T=1;T<=5;T++)
    {
        hc.init(),ho.init();
        set<int> st;
        n=5000;
        for(int i=1;i<=n;i++)
        {
            // 故意造大量重复和负数
            x=rand()%3000-1500;
            hc.insert(x),ho.insert(x),st.insert(x);
        }
        bool ok=true;
        for(int q=1;q<=5000;q++)
        {
            x=rand()%6000-3000;
            bool want=(st.count(x)>0);
            if(hc.find(x)!=want||ho.count(x)!=want){ok=false;break;}
        }
        printf("round %d %s\n",T,ok?"passed":"FAILED");
        if(!ok)break;
    }

    // 3. 简单应用：P4305 去重（按输入顺序输出第一次出现的数）
    // 这里直接把哈希表当判重器用
    hc.init();
    int b[]={1,2,1,3,2,4};
    for(int i=0;i<6;i++)
    {
        if(!hc.find(b[i]))
        {
            hc.insert(b[i]);
            printf("%d ",b[i]);
        }
    }
    printf("\n");
    return 0;
}
