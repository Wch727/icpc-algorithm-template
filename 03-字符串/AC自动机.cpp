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
