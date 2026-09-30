// 平衡树 Splay（伸展树）：插入 / 删除 / 排名 / 第k小 / 前驱后继 / 区间翻转
// 单次操作均摊 O(log n)，常数比 Treap 大但能支持区间操作（文艺平衡树）
// 本模板结点存「互不相同的值 + 出现次数 cnt」，所以同一棵 Splay 既能当普通平衡树，
// 也能当文艺平衡树用（此时把 val 换成原序列、cnt 全为 1，忽略 val 的 BST 性质）
// 约定：get_rank(x) 返回严格小于 x 的个数（0 起），kth 的 k 从 1 开始
// 坑 1：每次 splay/递归进入结点前必须 push_down，否则 rev 标记会破坏父子关系
// 坑 2：init(n) 会先放两个哨兵，文艺平衡树取区间 [l,r] 是 splay(l-1) 再 splay(r+1)，
//       第 l 个数对应「中序第 l+1 个结点」（1 号哨兵占掉了第 1 个位置）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
const int INF=0x3f3f3f3f;
int n,m,x;

struct Splay{
    int val[N],cnt[N],sz[N],fa[N],ch[N][2];
    bool rev[N];                           // 区间翻转标记
    int tot,root;
    int new_node(int x,int f)
    {
        tot++;
        val[tot]=x,cnt[tot]=1,sz[tot]=1,fa[tot]=f,rev[tot]=0;
        ch[tot][0]=ch[tot][1]=0;
        return tot;
    }
    void clear()
    {
        tot=0,root=0;
        ch[0][0]=ch[0][1]=fa[0]=sz[0]=cnt[0]=0,rev[0]=0;
    }
    int get(int p){return ch[fa[p]][1]==p;}            // p 是父结点的哪个儿子
    void push_up(int p){sz[p]=sz[ch[p][0]]+sz[ch[p][1]]+cnt[p];}
    void push_down(int p)
    {
        if(!p||!rev[p])return;
        swap(ch[p][0],ch[p][1]);               // 交换左右儿子 + 下传标记
        if(ch[p][0])rev[ch[p][0]]^=1;
        if(ch[p][1])rev[ch[p][1]]^=1;
        rev[p]=0;
    }
    // 单旋：p 往上走一层
    void rotate(int p)
    {
        int f=fa[p],g=fa[f],k=get(p),w=ch[p][k^1];
        push_down(f),push_down(p);             // 旋转前先把标记放下去
        ch[f][k]=w;
        if(w)fa[w]=f;
        ch[p][k^1]=f,fa[f]=p,fa[p]=g;
        if(g)ch[g][ch[g][1]==f]=p;
        push_up(f),push_up(p);
    }
    // 把 p 旋到 goal 的儿子位置（goal=0 即旋到根）
    void splay(int p,int goal=0)
    {
        while(fa[p]!=goal)
        {
            int f=fa[p],g=fa[f];
            if(g!=goal)
            {
                if(get(p)==get(f))rotate(f);
                else rotate(p);
            }
            rotate(p);
        }
        if(goal==0)root=p;
    }
    // 插入一个值 x（已存在则 cnt+1）
    void insert(int x)
    {
        if(root==0){root=new_node(x,0);return;}
        int p=root,f=0;
        while(p)
        {
            push_down(p);
            if(val[p]==x){cnt[p]++,push_up(p),splay(p);return;}
            f=p,p=ch[p][x>val[p]];
        }
        p=new_node(x,f),ch[f][x>val[f]]=p,splay(p);
    }
    // 删除一个 x（删掉一次出现，cnt 减到 0 才摘结点）
    void erase(int x)
    {
        int p=root;
        while(p)
        {
            push_down(p);
            if(val[p]==x)break;
            p=ch[p][x>val[p]];
        }
        if(!p)return;
        splay(p);
        if(cnt[p]>1){cnt[p]--,push_up(p);return;}
        push_down(p);
        int l=ch[p][0],r=ch[p][1];
        if(!l&&!r){root=0;return;}
        if(!l){fa[r]=0,root=r;return;}
        if(!r){fa[l]=0,root=l;return;}
        // 把左子树里最大的结点转到左子树根，右子树接上去
        int q=l;
        while(q)push_down(q),q=ch[q][1];
        q=l;
        while(ch[q][1])q=ch[q][1];
        splay(q,p);
        ch[q][1]=r,fa[r]=q,push_up(q);
        fa[q]=0,root=q;
    }
    // 定位值 x，返回结点编号（没有返回 0），不 splay，纯查找
    int find_node(int x)
    {
        int p=root;
        while(p)
        {
            push_down(p);
            if(val[p]==x)return p;
            p=ch[p][x>val[p]];
        }
        return 0;
    }
    // 严格小于 x 的数的个数（0 起）
    int get_rank(int x)
    {
        int p=root,ans=0;
        while(p)
        {
            push_down(p);
            if(x<=val[p])p=ch[p][0];
            else ans+=sz[ch[p][0]]+cnt[p],p=ch[p][1];
        }
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
            else if(k<=sz[ch[p][0]]+cnt[p])return val[p];
            else k-=sz[ch[p][0]]+cnt[p],p=ch[p][1];
        }
        return 0;
    }
    int get_pre(int x)                         // 严格小于 x 的最大值
    {
        int p=root,ans=-INF;
        while(p)
        {
            push_down(p);
            if(val[p]<x)ans=val[p],p=ch[p][1];
            else p=ch[p][0];
        }
        return ans;
    }
    int get_next(int x)                        // 严格大于 x 的最小值
    {
        int p=root,ans=INF;
        while(p)
        {
            push_down(p);
            if(val[p]>x)ans=val[p],p=ch[p][0];
            else p=ch[p][1];
        }
        return ans;
    }
    // ---------- 文艺平衡树部分 ----------
    // 建出 1..n 的序列，额外带两个哨兵，中序第 i 个数在位置 i+1
    void init(int n_)
    {
        clear();
        root=new_node(-INF,0),ch[root][1]=new_node(INF,root);    // 哨兵
        cnt[root]=cnt[ch[root][1]]=0;                            // 哨兵不计入元素个数
        push_up(root);
        splay(ch[root][1]);
        for(int i=1;i<=n_;i++)insert(i);                         // 值就是下标，居中序位置
    }
    // 找中序第 k 个结点（1 起，含哨兵），一路 push_down
    int get_kth_node(int k)
    {
        int p=root;
        while(p)
        {
            push_down(p);
            if(k<=sz[ch[p][0]])p=ch[p][0];
            else if(k<=sz[ch[p][0]]+cnt[p])return p;
            else k-=sz[ch[p][0]]+cnt[p],p=ch[p][1];
        }
        return 0;
    }
    // 区间翻转 [l,r]（对原序列下标，1 起）
    void reverse(int l,int r)
    {
        int a=get_kth_node(l),b=get_kth_node(r+2);               // 算上左哨兵，区间是 (l-1,r+1)
        splay(a),splay(b,a);
        int t=ch[b][0];
        if(t)rev[t]^=1;
    }
    // 中序输出真实元素（哨兵 cnt=0 会输出 -INF/INF），调试用
    void print(int p)
    {
        if(!p)return;
        push_down(p);
        print(ch[p][0]);
        if(cnt[p]>0)printf(" %d",val[p]);
        print(ch[p][1]);
    }
};

Splay sp;
int a[N],bf[N],bflen;

// ---------- 暴力对照 ----------
// bf 是升序数组（有重复），长度 bflen
int bf_rank(int x)
{
    int c=0;
    for(int i=1;i<=bflen;i++)if(bf[i]<x)c++;
    return c;
}
int bf_kth(int k){return (k>=1&&k<=bflen)?bf[k]:0;}
int bf_pre(int x)
{
    int ans=-INF;
    for(int i=1;i<=bflen;i++)if(bf[i]<x&&bf[i]>ans)ans=bf[i];
    return ans;
}
int bf_next(int x)
{
    int ans=INF;
    for(int i=1;i<=bflen;i++)if(bf[i]>x&&bf[i]<ans)ans=bf[i];
    return ans;
}
void bf_insert(int x)
{
    bflen++;
    int p=bflen;
    while(p>1&&bf[p-1]>x)bf[p]=bf[p-1],p--;
    bf[p]=x;
}
void bf_erase(int x)
{
    for(int i=1;i<=bflen;i++)
        if(bf[i]==x)
        {
            for(int j=i;j<bflen;j++)bf[j]=bf[j+1];
            bflen--;
            return;
        }
}

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

int main()
{
    srand(19260817);

    // 1. 文艺平衡树手测：1 2 3 4 5，翻转 [2,4] 应得 1 4 3 2 5
    sp.init(5);
    printf("初始序列:"),sp.print(sp.root),printf("\n");
    sp.reverse(2,4);
    printf("翻转[2,4]后:"),sp.print(sp.root),printf("\n");
    sp.reverse(1,5);
    printf("再翻转[1,5]后:"),sp.print(sp.root),printf("\n");
    sp.reverse(2,2);                            // 单点翻转应当没变化
    printf("翻转[2,2]后:"),sp.print(sp.root),printf("\n");
    sp.reverse(1,3),sp.reverse(1,3);            // 翻两次应当还原
    printf("翻转[1,3]两次后:"),sp.print(sp.root),printf("\n");

    // 2. 普通平衡树手测
    sp.clear();
    int ini[]={5,3,8,1,4,3};
    for(int i=0;i<6;i++)sp.insert(ini[i]);
    printf("hand: size=%d rank(4)=%d kth(1)=%d kth(6)=%d pre(5)=%d next(5)=%d\n",
        sp.sz[sp.root],sp.get_rank(4),sp.kth(1),sp.kth(6),sp.get_pre(5),sp.get_next(5));
    sp.erase(3);
    printf("after erase 3: size=%d rank(3)=%d kth(2)=%d\n",sp.sz[sp.root],sp.get_rank(3),sp.kth(2));

    // 3. 与暴力对拍：插入/删除/五种查询全比
    bool ok=true;
    for(int T=1;T<=12&&ok;T++)
    {
        sp.clear();
        bflen=0;
        n=rnd(1,40);
        for(int i=1;i<=n;i++)
        {
            x=rnd(-50,50);
            sp.insert(x);
            bf_insert(x);
        }
        for(int q=1;q<=300&&ok;q++)
        {
            int op=rnd(1,3);
            x=rnd(-60,60);
            if(op==1)sp.insert(x),bf_insert(x);
            else if(op==2)sp.erase(x),bf_erase(x);
            if(sp.sz[sp.root]!=bflen)
            {
                printf("第 %d 轮第 %d 步 size 错: splay=%d bf=%d\n",T,q,sp.sz[sp.root],bflen);
                ok=false;break;
            }
            int r1=sp.get_rank(x),r2=bf_rank(x);
            if(r1!=r2){printf("第 %d 轮第 %d 步 rank 错 x=%d got=%d want=%d\n",T,q,x,r1,r2);ok=false;break;}
            int k=rnd(1,bflen+1),g1=(bflen?sp.kth(k):0),g2=(k<=bflen?bf_kth(k):0);
            if(g1!=g2){printf("第 %d 轮第 %d 步 kth 错 k=%d got=%d want=%d\n",T,q,k,g1,g2);ok=false;break;}
            if(sp.get_pre(x)!=bf_pre(x)){printf("第 %d 轮第 %d 步 pre 错 x=%d\n",T,q,x);ok=false;break;}
            if(sp.get_next(x)!=bf_next(x)){printf("第 %d 轮第 %d 步 next 错 x=%d\n",T,q,x);ok=false;break;}
        }
        // 收尾：中序必须与暴力数组完全一致（顺带检查 rev 全被清掉、父子关系没坏）
        if(ok)
        {
            static int out[4005];
            int len=0;
            // 迭代中序，防止深树爆栈
            static int st[4005];
            int top=0,p=sp.root;
            while(top||p)
            {
                while(p)sp.push_down(p),st[++top]=p,p=sp.ch[p][0];
                p=st[top--];
                if(sp.cnt[p]>0)out[++len]=sp.val[p];
                p=sp.ch[p][1];
            }
            if(len!=bflen)ok=false;
            for(int i=1;i<=len&&ok;i++)if(out[i]!=bf[i])ok=false;
        }
        // 只看一部分值时也要对
        if(ok)
        {
            int p=sp.root;
            while(sp.ch[p][0])p=sp.ch[p][0];
            if(bflen&&sp.val[p]!=bf[1])ok=false;
        }
        printf("splay round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 4. 文艺平衡树与暴力对拍：随机区间翻转 + 单点查询
    for(int T=1;T<=10&&ok;T++)
    {
        n=rnd(1,50);
        for(int i=1;i<=n;i++)a[i]=i;
        sp.init(n);
        for(int q=1;q<=120&&ok;q++)
        {
            int l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            int op=rnd(1,3);
            if(op<=2)
            {
                sp.reverse(l,r);
                reverse(a+l,a+r+1);
            }
            else
            {
                int p=rnd(1,n);
                int got=sp.val[sp.get_kth_node(p+1)];         // 哨兵占了第 1 个位置
                if(got!=a[p]){printf("第 %d 轮文艺平衡树第 %d 步错: 位置 %d got=%d want=%d\n",T,q,p,got,a[p]);ok=false;}
            }
        }
        // 整体输出一遍与数组比
        if(ok)
        {
            static int out[4005];
            int len=0,st[4005],top=0,p=sp.root;
            while(top||p)
            {
                while(p)sp.push_down(p),st[++top]=p,p=sp.ch[p][0];
                p=st[top--];
                if(sp.cnt[p]>0)out[++len]=sp.val[p];
                p=sp.ch[p][1];
            }
            for(int i=1;i<=len&&ok;i++)if(out[i]!=a[i])ok=false;
            if(len!=n)ok=false;
        }
        printf("flip round %d %s\n",T,ok?"passed":"FAILED");
    }

    // 5. 顺序插入 1..100000（考退化）
    sp.clear();
    for(int i=1;i<=100000;i++)sp.insert(i);
    printf("big: size=%d kth1=%d kth100000=%d rank(50000)=%d\n",sp.sz[sp.root],sp.kth(1),sp.kth(100000),sp.get_rank(50000));

    // 6. 大规模文艺平衡树：n=100000，翻转 10000 次后抽查几个位置
    n=100000;
    for(int i=1;i<=n;i++)a[i]=i;
    sp.init(n);
    for(int i=1;i<=10000;i++)
    {
        int l=rnd(1,n),r=rnd(1,n);
        if(l>r)swap(l,r);
        sp.reverse(l,r);
        reverse(a+l,a+r+1);
    }
    int bad=0;
    for(int i=1;i<=200;i++)
    {
        int p=rnd(1,n);
        if(sp.val[sp.get_kth_node(p+1)]!=a[p])bad++;
    }
    printf("big flip: 抽查 200 个位置，错 %d 个\n",bad);
    if(bad)ok=false;

    printf("Splay 全部自测: %s\n",ok?"OK":"FAILED");
    return 0;
}
