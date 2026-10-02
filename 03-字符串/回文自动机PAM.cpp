#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m;
string s;

// 回文自动机：结点 0 是偶根 len=0，结点 1 是奇根 len=-1，奇根的 link 指向自己
// nxt 转移、link 后缀回文链、len 该回文串长度、siz 该回文串出现次数
int tot,last;
int len[N],link[N],siz[N],cnt[N+2],id[N+2];
int nxt[N][26];
char str[N];// 1-indexed 原串，str[0] 放用不到的哨兵 '#'
int pos[N];// 第一次出现的右端点；内容是 str[pos[u]-len[u]+1..pos[u]]

// 清空 PAM，多组数据用
void pam_init()
{
    tot=1,last=0;
    str[0]='#';
    len[0]=0,link[0]=1;
    len[1]=-1,link[1]=1;
    siz[0]=siz[1]=0;
    for(int j=0;j<26;j++)nxt[0][j]=nxt[1][j]=0;
}

// 从 p 出发沿 link 跳，找到能接上位置 i 的最长回文后缀
int get_fail(int p,int i)
{
    while(str[i-len[p]-1]!=str[i])p=link[p];
    return p;
}

// 在末尾加入第 i 个字符（1-indexed，str[i] 已赋值）
void pam_extend(int i)
{
    int c=str[i]-'a';
    int p=get_fail(last,i);
    if(!nxt[p][c])
    {
        int cur=++tot;
        len[cur]=len[p]+2,siz[cur]=1,pos[cur]=i;
        for(int j=0;j<26;j++)nxt[cur][j]=0;
        if(len[cur]==1)link[cur]=0;// 单字符回文的后缀链接是偶根
        else link[cur]=nxt[get_fail(link[p],i)][c];
        nxt[p][c]=cur;
    }
    else siz[nxt[p][c]]++;// 该回文串已经存在，出现次数 +1
    last=nxt[p][c];
}

// 按 len 基数排序，得到 len 单调不减的拓扑序 id[0..tot]，再沿 link 累加出现次数
// len 最小是奇根的 -1，统一 +1 后落在 0..tot+1，所以桶和 id 都开到 tot+1
// 所有字符加入后调用一次；累加后不要继续 extend/build。
void pam_build()
{
    int mx=tot+1;// 桶上界：len[i]+1 最大为 tot+1
    for(int i=0;i<=mx;i++)cnt[i]=0;
    for(int i=0;i<=tot;i++)cnt[len[i]+1]++;
    for(int i=1;i<=mx;i++)cnt[i]+=cnt[i-1];
    for(int i=tot;i>=0;i--)id[--cnt[len[i]+1]]=i;// 先减后放，保证 id[0] 也被填上
    for(int i=tot;i>=0;i--)
    {
        int u=id[i];
        if(u>=2)siz[link[u]]+=siz[u];// 只有真结点（非两个根）才沿 link 向下累加
    }
}

// 本质不同回文子串个数：去掉两个根
int count_pal(){return tot-1;}

// 最长回文子串长度：所有结点 len 的最大值
int longest_pal()
{
    int ans=0;
    for(int i=2;i<=tot;i++)ans=max(ans,len[i]);
    return ans;
}
