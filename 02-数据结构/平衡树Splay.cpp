// 平衡树 Splay（伸展树）：插入 / 删除 / 排名 / 第k小 / 前驱后继 / 区间翻转
// 单次操作均摊 O(log n)，常数比 Treap 大，但能支持区间操作（文艺平衡树）
// 本模板每个结点存一个互不相同的值 + 出现次数 num，所以同一份代码既能当普通平衡树，
// 也能当文艺平衡树（此时把 val 看成原序列元素、num 全为 1，忽略 val 的 BST 性质）
// 约定：get_rank(x) 返回严格小于 x 的个数（0 起），kth 的 k 从 1 开始
// 文艺平衡树用法：init(n) 建出 1..n 并带两个哨兵，位置 i 对应中序第 i+1 个结点
// 坑 1：splay/查找/旋转前都要 push_down，否则 rev 标记会让左右儿子对不上
// 坑 2：序列模式哨兵也计入 sz，位置查询含哨兵；输出时跳过哨兵
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
const int INF=0x3f3f3f3f;
int n,m,x;

struct Splay{
    int val[N],num[N],sz[N],fa[N],ch[N][2];
    bool rev[N];
    int tot,root;
    void clear()
    {
        tot=0,root=0;
        for(int i=0;i<2;i++)ch[0][i]=0;
        fa[0]=sz[0]=num[0]=0,rev[0]=0,val[0]=0;
    }
    int new_node(int v,int f)
    {
        tot++;
        val[tot]=v,num[tot]=1,sz[tot]=1,fa[tot]=f,rev[tot]=0;
        ch[tot][0]=ch[tot][1]=0;
        return tot;
    }
    int get(int p){return ch[fa[p]][1]==p;}          // p 是父结点的右儿子吗
    void push_up(int p){sz[p]=sz[ch[p][0]]+sz[ch[p][1]]+num[p];}
    void push_down(int p)
    {
        if(!p||!rev[p])return;
        swap(ch[p][0],ch[p][1]);                     // 翻转：先换儿子，再下传标记
        if(ch[p][0])rev[ch[p][0]]^=1;
        if(ch[p][1])rev[ch[p][1]]^=1;
        rev[p]=0;
    }
    // 单旋：p 往上走一层
    void rotate(int p)
    {
        int f=fa[p],g=fa[f],k=get(p),w=ch[p][k^1];
        push_down(f),push_down(p);                   // 旋转前先把标记放下去
        ch[f][k]=w;
        if(w)fa[w]=f;
        ch[p][k^1]=f,fa[f]=p,fa[p]=g;
        if(g)ch[g][ch[g][1]==f]=p;
        push_up(f),push_up(p);
    }
    // 把 p 旋到 goal 的儿子处，goal=0 表示旋到根
    void splay(int p,int goal=0)
    {
        vector<int> path;
        for(int u=p;u;u=fa[u])path.push_back(u);
        for(int i=(int)path.size()-1;i>=0;i--)push_down(path[i]);
        while(fa[p]!=goal)
        {
            int f=fa[p],g=fa[f];
            if(g!=goal)
            {
                if(get(p)==get(f))rotate(f);         // 同向：先转父亲（zig-zig）
                else rotate(p);                      // 异向：先转自己（zig-zag）
            }
            rotate(p);
        }
        if(!goal)root=p;
    }
    // 插入一个值 v
    void insert(int v)
    {
        if(!root){root=new_node(v,0);return;}
        int p=root,f=0;
        while(p)
        {
            push_down(p);
            if(val[p]==v){num[p]++,push_up(p),splay(p);return;}
            f=p,p=ch[p][v>val[p]];
        }
        p=new_node(v,f),ch[f][v>val[f]]=p,splay(p);
    }
    // 删除一个值 v（删掉一次出现）
    void erase(int v)
    {
        int p=find_node(v);
        if(!p)return;                                // 不存在
        splay(p);
        if(num[p]>1){num[p]--,push_up(p);return;}
        int l=ch[p][0],r=ch[p][1];
        if(!l&&!r){root=0;return;}
        if(!l){fa[r]=0,ch[p][1]=0,root=r;return;}
        if(!r){fa[l]=0,ch[p][0]=0,root=l;return;}
        // 关键：左右子树的根要同时从 p 上摘下来（fa 和 ch 都清），
        // 否则 splay(q) 旋转时会顺着没摘干净的那条边把 r 又转进来，结点就丢了
        fa[l]=fa[r]=0,ch[p][0]=ch[p][1]=0;
        int q=l;
        while(ch[q][1])push_down(q),q=ch[q][1];
        splay(q);
        ch[q][1]=r,fa[r]=q,push_up(q);
    }
    // 找值为 v 的结点，没有返回 0

    int find_node(int v)
    {
        int p=root,last=0;
        while(p)
        {
            push_down(p); last=p;
            if(val[p]==v){splay(p); return p;}
            p=ch[p][v>val[p]];
        }
        if(last)splay(last);
        return 0;
    }

    // 严格小于 v 的个数（0 起）
    int get_rank(int v)
    {
        int p=root,last=0,ans=0;
        while(p)
        {
            push_down(p); last=p;
            if(v<=val[p])p=ch[p][0];
            else ans+=sz[ch[p][0]]+num[p],p=ch[p][1];
        }
        if(last)splay(last);
        return ans;
    }
    // 第 k 小（k 从 1 开始），不存在返回 0
    int kth(int k)
    {
        if(k<1||k>sz[root])return 0;
        int p=root;
        while(p)
        {
            push_down(p);
            if(k<=sz[ch[p][0]])p=ch[p][0];
            else if(k<=sz[ch[p][0]]+num[p]){int v=val[p]; splay(p); return v;}
            else k-=sz[ch[p][0]]+num[p],p=ch[p][1];
        }
        return 0;
    }
    int get_pre(int v)                               // 严格小于 v 的最大值
    {
        int p=root,last=0,ans=-INF;
        while(p)
        {
            push_down(p); last=p;
            if(val[p]<v)ans=val[p],p=ch[p][1];
            else p=ch[p][0];
        }
        if(last)splay(last);
        return ans;
    }
    int get_next(int v)                              // 严格大于 v 的最小值
    {
        int p=root,last=0,ans=INF;
        while(p)
        {
            push_down(p); last=p;
            if(val[p]>v)ans=val[p],p=ch[p][0];
            else p=ch[p][1];
        }
        if(last)splay(last);
        return ans;
    }
    // ---------- 以下是文艺平衡树 ----------
    // 直接按中序建一棵尽量平衡的树：区间 [l,r] 里放的是值 v..v+r-l
    int build(int l,int r,int v)
    {
        if(l>r)return 0;
        int mid=(l+r)>>1;
        int p=new_node(v+mid-l,0);
        ch[p][0]=build(l,mid-1,v);
        ch[p][1]=build(mid+1,r,v+mid-l+1);
        if(ch[p][0])fa[ch[p][0]]=p;
        if(ch[p][1])fa[ch[p][1]]=p;
        push_up(p);
        return p;
    }
    // 建出序列 1..n_，两个哨兵也占一个位置
    void init(int n_)
    {
        clear();
        root=build(0,n_+1,0);
        val[get_pos(1)]=-INF;
        val[get_pos(n_+2)]=INF;
    }
    // 中序第 k 个结点（1 起，含哨兵），一路 push_down
    int get_pos(int k)
    {
        int p=root;
        while(p)
        {
            push_down(p);
            if(k<=sz[ch[p][0]])p=ch[p][0];
            else if(k<=sz[ch[p][0]]+num[p])return p;
            else k-=sz[ch[p][0]]+num[p],p=ch[p][1];
        }
        return 0;
    }
    // 区间翻转 [l,r]（原序列下标，1 起）
    // 左哨兵占第 1 个中序位置，所以「l-1 位置」是第 l 个，「r+1 位置」是第 r+2 个；
    // 把这两个旋上来之后，夹在中间的整棵子树正好是 [l,r]
    void reverse(int l,int r)
    {
        int a=get_pos(l),b=get_pos(r+2);
        if(!a||!b)return;
        splay(a),splay(b,a);
        int t=ch[b][0];
        if(t)rev[t]^=1;
    }
    // 中序打印真实元素，调试用（结点记了出现次数，这里每个值只打一次）
    void print(int p)
    {
        if(!p)return;
        push_down(p);
        print(ch[p][0]);
        if(val[p]!=-INF&&val[p]!=INF)printf(" %d",val[p]);
        print(ch[p][1]);
    }
};

Splay sp;
