// 手写堆：大根堆 / 小根堆（上浮 push + 下沉 pop）+ 堆排序 + O(n) 建堆
// 比较器约定：Cmp(a,b) 为真表示 a 该排在 b 前面（更靠近堆顶）
//   小根堆 CmpMin：Cmp(a,b) = a<b，堆顶最小
//   大根堆 CmpMax：Cmp(a,b) = a>b，堆顶最大
// 上浮和下沉只用一条规则：儿子排在父亲前面（Cmp(儿子,父亲) 为真）就换
// 大数组一律放全局，别开在栈上（默认栈只有 1MB）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,a[N],b[N];

struct CmpMin{bool operator()(int x,int y){return x<y;}};   // 小的靠前
struct CmpMax{bool operator()(int x,int y){return x>y;}};   // 大的靠前

template<typename T,typename Cmp=CmpMin,int NMAX=N>
struct Heap{
    T h[NMAX];      // 下标从 1 开始，h[0] 不用
    int sz;
    Cmp cmp;
    Heap(){sz=0;}
    void clear(){sz=0;}
    bool empty(){return sz==0;}
    int size(){return sz;}
    T top(){return h[1];}
    void up(int p)                     // 上浮，O(log n)
    {
        while(p>1)
        {
            int fa=p>>1;
            if(cmp(h[p],h[fa]))swap(h[p],h[fa]),p=fa;   // 儿子该排在父亲前面
            else break;
        }
    }
    void down(int p)                   // 下沉，O(log n)
    {
        while((p<<1)<=sz)
        {
            int c=p<<1;
            if(c<sz&&cmp(h[c+1],h[c]))c++;              // 两个儿子里更靠前的那个
            if(cmp(h[c],h[p]))swap(h[c],h[p]),p=c;      // 儿子该排在前面
            else break;
        }
    }
    void push(T x){h[++sz]=x,up(sz);}
    void pop(){h[1]=h[sz--],down(1);}   // 堆空时调用是 UB，先判 empty
    void build(T *src,int len)          // O(n) 建堆
    {
        sz=len;
        for(int i=1;i<=len;i++)h[i]=src[i];
        for(int i=sz>>1;i>=1;i--)down(i);
    }
    bool valid()                        // 自测用：校验堆性质
    {
        for(int i=1;i<=sz;i++)
            for(int j=i<<1;j<=(i<<1|1)&&j<=sz;j++)
                if(cmp(h[j],h[i]))return false;   // 儿子比父亲靠前就违反堆序
        return true;
    }
};

Heap<int,CmpMin> mn;                   // 小根堆
Heap<int,CmpMax> mx;                   // 大根堆
Heap<int,CmpMax> hs;                   // 堆排序专用的大根堆（全局，避免爆栈）

// 堆排序：升序，先建大根堆，再把堆顶换到末尾、缩小堆
template<typename T>
void heap_sort(T *src,int len)
{
    hs.clear();
    hs.build(src,len);
    for(int i=len;i>=2;i--)
    {
        swap(hs.h[1],hs.h[i]);
        hs.sz--,hs.down(1);
    }
    for(int i=1;i<=len;i++)src[i]=hs.h[i];
}
