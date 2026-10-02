#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m;
string s;

// 后缀自动机：O(n) 建图，nxt 转移、link 后缀链接、len 该状态最长子串长度
// siz[i] 为状态 i 的 endpos 集合大小（即该状态代表子串的出现次数）
int tot,last;
int len[N],link[N],siz[N],cnt[N+2],id[N+2];
int nxt[N][26];

// 清空 SAM，多组数据用；根固定为 1
void sam_init()
{
    tot=1,last=1;
    len[1]=0,link[1]=0,siz[1]=0;
    for(int j=0;j<26;j++)nxt[1][j]=0;
}

// 插入一个字符 c（'a'~'z'）
void sam_extend(int c)
{
    int cur=++tot;
    len[cur]=len[last]+1,link[cur]=0,siz[cur]=1;
    for(int j=0;j<26;j++)nxt[cur][j]=0;
    int p=last;
    while(p&&!nxt[p][c])nxt[p][c]=cur,p=link[p];
    if(!p)link[cur]=1;
    else
    {
        int q=nxt[p][c];
        if(len[p]+1==len[q])link[cur]=q;
        else
        {
            int clone=++tot;// 分裂出的克隆点，siz 记为 0，不算新出现
            len[clone]=len[p]+1,link[clone]=link[q],siz[clone]=0;
            for(int j=0;j<26;j++)nxt[clone][j]=nxt[q][j];
            while(p&&nxt[p][c]==q)nxt[p][c]=clone,p=link[p];
            link[q]=link[cur]=clone;
        }
    }
    last=cur;
}

// 按 len 基数排序，得到 len 单调不减的拓扑序 id[0..tot-1]；顺便求每个状态的出现次数
// 所有字符加入后调用一次；siz 累加后不要继续 extend/build。
void sam_build()
{
    int mx=tot+1;
    for(int i=0;i<=mx;i++)cnt[i]=0;
    for(int i=1;i<=tot;i++)cnt[len[i]+1]++;
    for(int i=1;i<=mx;i++)cnt[i]+=cnt[i-1];
    for(int i=tot;i>=1;i--)id[--cnt[len[i]+1]]=i;// 先减后放，id 下标落在 0..tot-1
    for(int i=tot-1;i>=0;i--)siz[link[id[i]]]+=siz[id[i]];// 沿 link 累加 endpos
}

// 本质不同子串个数：每个状态贡献 len[i]-len[link[i]]
ll count_diff_substr()
{
    ll ans=0;
    for(int i=2;i<=tot;i++)ans+=len[i]-len[link[i]];
    return ans;
}

// 求 s 与 t 的最长公共子串长度：t 在 s 的 SAM 上跑，失配时沿 link 跳
int longest_common(const string &t)
{
    int p=1,l=0,ans=0;
    for(int i=0;i<(int)t.length();i++)
    {
        int c=t[i]-'a';
        while(p&&!nxt[p][c])p=link[p],l=len[p];// 跳 link，当前匹配长度退到 len[p]
        if(nxt[p][c])p=nxt[p][c],l++;
        else p=1,l=0;
        ans=max(ans,l);
    }
    return ans;
}
