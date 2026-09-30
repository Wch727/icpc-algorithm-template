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

int main()
{
    srand(20240513);

    // 1. 手测：按 P5076 的用法插入 1 4 2 5 3
    t.clear(),root=0;
    int ini[]={1,4,2,5,3};
    for(int i=0;i<5;i++)root=t.insert(root,ini[i]);
    printf("rank(3)=%d kth(1)=%d kth(5)=%d\n",t.get_rank(root,3),t.kth(root,1),t.kth(root,5));
    printf("pre(3)=%d next(3)=%d pre(1)=%d next(5)=%d\n",t.get_pre(root,3),t.get_next(root,3),t.get_pre(root,1),t.get_next(root,5));
    root=t.erase(root,3);
    printf("after erase 3: rank(4)=%d kth(4)=%d find(3)=%d size=%d\n",t.get_rank(root,4),t.kth(root,4),(int)t.find(root,3),t.sz[root]);

    // 2. 随机对拍：插入 / 删除 / 五种查询 与有序数组暴力比较
    bool ok=true;
    for(int T=1;T<=10&&ok;T++)
    {
        t.clear(),root=0,bf.clear();
        n=300;
        for(int i=1;i<=n;i++)
        {
            x=rand()%200;
            if(rand()%4==0)                 // 1/4 概率删除
            {
                root=t.erase(root,x);
                bf_erase(x);
            }
            else
            {
                root=t.insert(root,x);
                bf_insert(x);
            }
            if(rand()%3==0)
            {
                if(t.get_rank(root,x)!=bf_rank(x)){ok=false;break;}
                int k=rand()%((int)bf.size()+2)+1;
                if(t.kth(root,k)!=bf_kth(k)){ok=false;break;}
                if(t.get_pre(root,x)!=bf_pre(x)){ok=false;break;}
                if(t.get_next(root,x)!=bf_next(x)){ok=false;break;}
                if(t.find(root,x)!=(bool)binary_search(bf.begin(),bf.end(),x)){ok=false;break;}
            }
        }
        printf("random round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 3. 边界：空树 / 单结点 / 删除到空
    t.clear(),root=0,bf.clear();
    printf("empty: rank=%d kth=%d pre=%d next=%d\n",t.get_rank(root,5),t.kth(root,1),t.get_pre(root,5),t.get_next(root,5));
    root=t.insert(root,7);
    printf("one: rank(7)=%d rank(9)=%d kth(1)=%d pre(7)=%d next(7)=%d\n",t.get_rank(root,7),t.get_rank(root,9),t.kth(root,1),t.get_pre(root,7),t.get_next(root,7));
    root=t.erase(root,7);
    printf("erase to empty: size=%d kth(1)=%d\n",t.sz[root],t.kth(root,1));

    // 样例（P5076）：插入 1 4 2 5 3 后 rank(3)=1(即第 2 小)，kth(1)=1
    return 0;
}
