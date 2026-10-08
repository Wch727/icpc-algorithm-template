// 匹配文本前缀到状态 u：它的 fail 树祖先均为当前位置结束的匹配模式。
// 反过来模式结点 v 的出现次数/位置和，可统计 fail 子树内被访问状态；DFS 序转 BIT 区间。
// 多查询若对文本前缀长度单调，可共建一棵 AC，再整体二分按 mid 扫文本。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m,root=1,tot=1;
string s;

int trie[N][26];// Trie 儿子
int nxt[N];// fail 指针
int cnt[N];// 该结点被多少个模式串覆盖
int id[N];// 每个模式串的终止结点

// 插入模式串，返回终止结点编号
int insert(const string &str)
{
    int p=root;
    for(int i=0;i<(int)str.length();i++)
    {
        int k=str[i]-'a';
        if(!trie[p][k])trie[p][k]=++tot;
        p=trie[p][k];
    }
    cnt[p]++;//终止结点覆盖数 +1，重复模式串也累计
    return p;
}

// 清空 Trie，多组数据用；O(tot)
void clear_trie()
{
    for(int i=0;i<=tot;i++)
    {
        cnt[i]=0,nxt[i]=0;
        for(int j=0;j<26;j++)trie[i][j]=0;
    }
    tot=1;
}

// 建 fail 指针（BFS），根的儿子 fail 指向根；O(结点数*26)
void build_fail()
{
    queue<int> q;
    for(int j=0;j<26;j++)
    {
        if(trie[root][j])nxt[trie[root][j]]=root,q.push(trie[root][j]);
        else trie[root][j]=root;//根处失配回到根
    }
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        cnt[u]+=cnt[nxt[u]];//继承 fail 的覆盖数，统计时不必沿 fail 链跳
        for(int j=0;j<26;j++)
        {
            int v=trie[u][j];
            if(v)nxt[v]=trie[nxt[u]][j],q.push(v);
            else trie[u][j]=trie[nxt[u]][j];
        }
    }
}

// 文本串在自动机上跑一遍，返回所有模式串出现次数之和（含重复计入）
// 复杂度 O(|文本|)
ll query(const string &txt)
{
    int p=root;
    ll ans=0;
    for(int i=0;i<(int)txt.length();i++)
    {
        p=trie[p][txt[i]-'a'];
        ans+=cnt[p];
    }
    return ans;
}

// 普通 Trie 应用（独立于上面的 AC）：f[j] 为选恰 j 个已插入串的最大 LCP 长度。
// g[h] 为深度 h 的节点最大经过次数，f[j]>=h 等价于 g[h]>=j，是同一单调阶梯的转置。
// 节点 cnt 从 c-1 变 c，只需令 f[c]=max(f[c],h)；g 用于解释，不必实际存储。
// 小写非空串，重复串保留重数；插入后返回 sum(f[j] xor j)，j=1..已插入串数。
struct PrefixLCP
{
    vector<array<int,26>> tr{array<int,26>{}};
    vector<int> cnt{0}, f{0};
    ll sum=0;
    ll insert(const string &s)
    {
        int r=f.size();f.push_back(0);sum+=r;// 新增 j=r 的基准项 0 xor r
        int p=0,h=0;
        for(char ch:s)
        {
            int x=ch-'a';++h;
            if(!tr[p][x])
            {
                tr[p][x]=tr.size();
                tr.push_back({});cnt.push_back(0);
            }
            p=tr[p][x];int c=++cnt[p];
            if(h > f[c])
            {
                sum-= f[c] ^ c;
                f[c]= h;
                sum+= f[c] ^ c;
            }
        }
        return sum;
    }
};
