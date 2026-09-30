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
    for(int i=1;i<=n;i++)lc[i]=rc[i]=fa[i]=0;
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
