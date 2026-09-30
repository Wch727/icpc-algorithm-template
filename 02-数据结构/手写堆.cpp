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

int main()
{
    srand(20240513);

    // 1. 小数据手测：小根堆堆顶最小、大根堆堆顶最大
    int ini[]={0,5,1,9,3,7};
    for(int i=1;i<=5;i++)mn.push(ini[i]),mx.push(ini[i]);
    printf("min-top=%d max-top=%d valid=%d%d\n",mn.top(),mx.top(),(int)mn.valid(),(int)mx.valid());

    // 2. 与 priority_queue 对拍
    bool ok=true;
    for(int T=1;T<=10&&ok;T++)
    {
        mn.clear(),mx.clear();
        priority_queue<int,vector<int>,greater<int> > q1;   // 小根堆
        priority_queue<int> q2;                             // 大根堆
        for(int i=1;i<=3000;i++)
        {
            int x=rand()%1000-500;
            mn.push(x),mx.push(x),q1.push(x),q2.push(x);
            if(mn.top()!=q1.top()||mx.top()!=q2.top()){ok=false;break;}
            if(!mn.valid()||!mx.valid()){ok=false;break;}
            if(i%3==0)
            {
                mn.pop(),mx.pop(),q1.pop(),q2.pop();
                if(mn.top()!=q1.top()||mx.top()!=q2.top()){ok=false;break;}
                if(!mn.valid()||!mx.valid()){ok=false;break;}
            }
        }
        printf("heap round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 堆排序对拍 sort
    for(int T=1;T<=10;T++)
    {
        n=rand()%200+1;
        for(int i=1;i<=n;i++)a[i]=rand()%100-50,b[i]=a[i];
        heap_sort(a,n);
        sort(b+1,b+n+1);
        bool same=true;
        for(int i=1;i<=n;i++)if(a[i]!=b[i]){same=false;break;}
        printf("sort round %d %s\n",T,same?"passed":"FAILED");
        if(!same)break;
    }

    // 4. O(n) 建堆后逐个弹出，应当升序（小根堆）
    int c[]={0,4,2,8,1,9,3};
    mn.build(c,6);
    printf("build-pop: ");
    while(!mn.empty())printf("%d ",mn.top()),mn.pop();
    printf("\n");

    // 5. 小根堆套 P3378 的用法：1 插入 x，2 输出最小值，3 删除最小值
    mn.clear();
    mn.push(3),printf("%d ",mn.top()),mn.push(1),printf("%d ",mn.top()),mn.pop(),printf("%d\n",mn.top());
    return 0;
}
