// 平衡树 Treap（带旋）：插入 / 删除 / 排名 / 第k小 / 前驱后继
// 每个结点随机一个优先值，小根堆性质 → 期望 O(log n)，最坏 O(n)
// 重复值按独立结点存（不去重），排名约定：get_rank(x) 返回「比 x 小的数个数」（0 起）
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
    void clear(){tot=0;}
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
        if(p==0){p=new_node(x);return;}
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
        else return;                       // 已有相同值（本模板不去重就改成 >= 往右放）
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
vector<int> bf;                            // 暴力容器（有序、去重），插入前用 binary_search 判重

bool bf_has(int x){return binary_search(bf.begin(),bf.end(),x);}
void bf_insert(int x)
{
    if(bf_has(x))return;
    bf.insert(lower_bound(bf.begin(),bf.end(),x),x);
}
void bf_erase(int x)
{
    vector<int>::iterator it=lower_bound(bf.begin(),bf.end(),x);
    if(it!=bf.end()&&*it==x)bf.erase(it);
}
int bf_rank(int x){return lower_bound(bf.begin(),bf.end(),x)-bf.begin();}
int bf_kth(int k){return (k>=1&&k<=(int)bf.size())?bf[k-1]:0;}
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

int main()
{
    // 1. 手测：插入 5 3 8 1 4，看第 k 小和前驱后继
    t.clear(),root=0;
    int ini[]={5,3,8,1,4};
    for(int i=0;i<5;i++)t.insert(root,ini[i]);
    printf("kth1=%d kth3=%d kth5=%d min=%d max=%d\n",t.kth(root,1),t.kth(root,3),t.kth(root,5),t.get_min(root),t.get_max(root));
    printf("rank(4)=%d pre(4)=%d next(4)=%d pre(1)=%d next(8)=%d\n",t.get_rank(root,4),t.get_pre(root,4),t.get_next(root,4),t.get_pre(root,1),t.get_next(root,8));
    printf("inorder:"),t.print_inorder(root),printf("\n");
    t.erase(root,5);
    printf("after erase 5: kth4=%d size=%d find(5)=%d\n",t.kth(root,4),t.sz[root],(int)t.find(root,5));

    // 2. 随机多轮对拍：插入 / 删除 / 五种查询 与暴力比较
    bool ok=true;
    for(int T=1;T<=15&&ok;T++)
    {
        t.clear(),root=0,bf.clear();
        n=1000;
        for(int i=1;i<=n;i++)
        {
            x=rand()%400;
            int do_erase=(rand()%3==0);         // 先决定动作，保证两边完全同步
            if(do_erase)
            {
                t.erase(root,x);
                bf_erase(x);
            }
            else
            {
                t.insert(root,x);
                bf_insert(x);
            }
            if(t.sz[root]!=(int)bf.size())
            {
                printf("第 %d 步 size 对不上：树=%d bf=%d\n",i,t.sz[root],(int)bf.size());
                ok=false;break;
            }
            if(rand()%4==0)
            {
                if(t.get_rank(root,x)!=bf_rank(x)){printf("第 %d 步 rank 错 x=%d\n",i,x);ok=false;break;}
                int k=rand()%((int)bf.size()+2)+1;
                if(t.kth(root,k)!=bf_kth(k)){printf("第 %d 步 kth 错 k=%d\n",i,k);ok=false;break;}
                if(t.get_pre(root,x)!=bf_pre(x)){printf("第 %d 步 pre 错 x=%d\n",i,x);ok=false;break;}
                if(t.get_next(root,x)!=bf_next(x)){printf("第 %d 步 next 错 x=%d\n",i,x);ok=false;break;}
                if(t.find(root,x)!=bf_has(x)){printf("第 %d 步 find 错 x=%d\n",i,x);ok=false;break;}
            }
        }
        // 收尾再整体比一遍：中序应当等于 bf
        if(ok)
        {
            for(int i=0;i<(int)bf.size()&&ok;i++)
                if(t.kth(root,i+1)!=bf[i])ok=false;
        }
        if(ok&&!bf.empty()&&(t.get_min(root)!=bf.front()||t.get_max(root)!=bf.back()))ok=false;
        printf("random round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 边界：空树 / 单结点 / 删到空
    t.clear(),root=0;
    printf("empty: rank=%d kth=%d pre=%d next=%d\n",t.get_rank(root,5),t.kth(root,1),t.get_pre(root,5),t.get_next(root,5));
    t.insert(root,7);
    printf("one: rank(7)=%d kth(1)=%d pre(7)=%d next(7)=%d\n",t.get_rank(root,7),t.kth(root,1),t.get_pre(root,7),t.get_next(root,7));
    t.erase(root,7);
    printf("erase to empty: size=%d kth(1)=%d\n",t.sz[root],t.kth(root,1));

    // 4. 大量有序插入（比 BST 抗退化）：1..100000 顺序插入后查第 1 小
    t.clear(),root=0;
    for(int i=1;i<=100000;i++)t.insert(root,i);
    printf("ordered insert: size=%d kth1=%d kth100000=%d\n",t.sz[root],t.kth(root,1),t.kth(root,100000));
    return 0;
}
