// 笛卡尔树：O(n) 单调栈建树
// 性质：中序遍历就是原数组顺序；每个结点是「小根堆」（默认最小值在根），
//       所以区间 [l,r] 的最小值就是 lca(l,r)，配合 O(1) LCA 就能做 RMQ
// 反过来：把「下标」当 BST 键、「值」当堆键，笛卡尔树唯一确定
// 单调栈过程：新点 x 从栈顶往下弹掉所有 val > a[x] 的点，
//             最后弹出来的那个点当 x 的左儿子（它右边的点都比它大，全在左边）
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
const int LOG=18;

int n,a[N];
int lc[N],rc[N],fa[N],stk[N];
int root;

// O(n) 建笛卡尔树（小根堆）；返回根；相等时取靠前的当根（用 > 判断）
int build_cart()
{
    int top=0,rt=0;
    for(int i=1;i<=n;i++)
    {
        int last=0;
        while(top&&a[stk[top]]>a[i])last=stk[top--];//弹掉比 a[i] 大的
        lc[i]=last;
        if(last)fa[last]=i;
        if(top)rc[stk[top]]=i,fa[i]=stk[top];
        else fa[i]=0,rt=i;//栈空了，i 就是当前的根
        stk[++top]=i;
    }
    return rt;
}

// 对拍的暴力：区间最小值
int brute_min(int l,int r)
{
    int z=a[l];
    for(int i=l+1;i<=r;i++)z=min(z,a[i]);
    return z;
}

int dep[N],up[N][LOG];

void dfs_dep(int u,int d)
{
    dep[u]=d,up[u][0]=fa[u];
    if(lc[u])dfs_dep(lc[u],d+1);
    if(rc[u])dfs_dep(rc[u],d+1);
}

// LCA（倍增），O(log n)
int lca(int u,int v)
{
    if(dep[u]<dep[v])swap(u,v);
    for(int k=LOG-1;k>=0;k--)if(dep[u]-(1<<k)>=dep[v])u=up[u][k];
    if(u==v)return u;
    for(int k=LOG-1;k>=0;k--)if(up[u][k]!=up[v][k])u=up[u][k],v=up[v][k];
    return up[u][0];
}

// 中序遍历（迭代写法，深树也不会爆栈），检查是不是 1..n
void inorder(int rt,int *out)
{
    int cur=rt,top=0,cnt=0;
    int s[N];
    while(cur||top)
    {
        while(cur)s[++top]=cur,cur=lc[cur];
        cur=s[top--];
        out[++cnt]=cur;
        cur=rc[cur];
    }
}

// 堆性质检查（迭代，避免爆栈）
bool check_heap(int rt)
{
    vector<int> q;
    q.push_back(rt);
    for(size_t i=0;i<q.size();i++)
    {
        int u=q[i];
        if(lc[u])
        {
            if(a[lc[u]]<a[u])return false;
            q.push_back(lc[u]);
        }
        if(rc[u])
        {
            if(a[rc[u]]<a[u])return false;
            q.push_back(rc[u]);
        }
    }
    return true;
}

int tmp[N];

int rnd(int l,int r)
{
    return rand()%(r-l+1)+l;
}

int main()
{
    srand(20240513);
    bool ok=true;

    // 1. 手算样例 a = 3 1 4 1 5 9 2 6
    {
        n=8;
        int ini[]={0,3,1,4,1,5,9,2,6};
        for(int i=1;i<=n;i++)a[i]=ini[i];
        root=build_cart();
        printf("小样例: root=%d(应=2, a[2]=1) lc[2]=%d rc[2]=%d\n",root,lc[2],rc[2]);
        inorder(root,tmp);
        for(int i=1;i<=n;i++)if(tmp[i]!=i)ok=false;
        printf("中序遍历:");
        for(int i=1;i<=n;i++)printf(" %d",tmp[i]);
        printf("  %s\n",ok?"(等于原下标序列)":"(错)");
        if(!check_heap(root))ok=false;
    }

    // 2. 对拍：随机数组建树，检查 BST/堆性质，再用 lca 做 RMQ
    for(int T=1;T<=30&&ok;T++)
    {
        n=rnd(1,70);
        for(int i=1;i<=n;i++)a[i]=rnd(-20,20);//有重复值
        root=build_cart();
        inorder(root,tmp);
        for(int i=1;i<=n;i++)if(tmp[i]!=i){printf("第 %d 轮 BST 性质错\n",T);ok=false;}
        if(!check_heap(root)){printf("第 %d 轮 堆性质错\n",T);ok=false;}
        for(int i=1;i<=n;i++)dep[i]=0;
        dfs_dep(root,1);
        for(int k=1;k<LOG;k++)
            for(int i=1;i<=n;i++)up[i][k]=up[up[i][k-1]][k-1];
        for(int q=1;q<=60&&ok;q++)
        {
            int l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            int got=a[lca(l,r)],want=brute_min(l,r);
            if(got!=want)
            {
                printf("第 %d 轮 RMQ 错: [%d,%d] got=%d want=%d\n",T,l,r,got,want);
                ok=false;
            }
        }
    }
    printf("笛卡尔树 随机对拍 %s\n",ok?"passed":"FAILED");

    // 3. 特殊数据：不降 / 不增 / 全相等
    {
        n=6;
        for(int i=1;i<=n;i++)a[i]=i;
        root=build_cart();
        inorder(root,tmp);
        bool f=true;
        for(int i=1;i<=n;i++)if(tmp[i]!=i)f=false;
        printf("不降序列: root=%d(应=1) chk=%d\n",root,(int)f);

        for(int i=1;i<=n;i++)a[i]=n-i+1;
        root=build_cart();
        inorder(root,tmp);
        f=true;
        for(int i=1;i<=n;i++)if(tmp[i]!=i)f=false;
        printf("不增序列: root=%d(应=%d) chk=%d\n",root,n,(int)f);

        for(int i=1;i<=n;i++)a[i]=7;
        root=build_cart();
        inorder(root,tmp);
        f=true;
        for(int i=1;i<=n;i++)if(tmp[i]!=i)f=false;
        printf("全相等: root=%d(应=1) chk=%d\n",root,(int)f);
    }

    // 4. 规模测试：n=100000 的随机排列
    {
        n=100000;
        for(int i=1;i<=n;i++)a[i]=i;
        for(int i=n;i>=2;i--)swap(a[i],a[rnd(1,i)]);//随机排列
        root=build_cart();
        inorder(root,tmp);
        int bad=0;
        for(int i=1;i<=n;i++)if(tmp[i]!=i)bad++;
        int h=check_heap(root)?1:0;
        for(int i=1;i<=n;i++)dep[i]=0;
        dfs_dep(root,1);
        for(int k=1;k<LOG;k++)
            for(int i=1;i<=n;i++)up[i][k]=up[up[i][k-1]][k-1];
        // 随机 RMQ 抽查（暴力也是 O(区间长)，抽查量不能太大）
        for(int q=1;q<=20000;q++)
        {
            int l=rnd(1,n),r=rnd(1,n);
            if(l>r)swap(l,r);
            if(a[lca(l,r)]!=brute_min(l,r))bad++;
        }
        printf("规模: n=100000 -> 中序错 %d 个, 堆性质 %d, 最大深度 %d\n",bad,h,
            *max_element(dep+1,dep+n+1));
    }

    printf("结果: %s\n",ok?"OK":"FAILED");
    return 0;
}
