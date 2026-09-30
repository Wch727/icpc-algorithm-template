#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
int n,m;
string s;

// 后缀自动机：O(n) 建图，nxt 转移、link 后缀链接、len 该状态最长子串长度
// siz[i] 为状态 i 的 endpos 集合大小（即该状态代表子串的出现次数）
int tot,last;
int len[N],link[N],siz[N],cnt[N],id[N];
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

// 按 len 基数排序，得到拓扑序；顺便求每个状态的出现次数
void sam_build()
{
    for(int i=1;i<=tot;i++)cnt[i]=0;
    for(int i=1;i<=tot;i++)cnt[len[i]]++;
    for(int i=1;i<=n;i++)cnt[i]+=cnt[i-1];
    for(int i=tot;i>=1;i--)id[cnt[len[i]]--]=i;
    for(int i=tot;i>=2;i--)siz[link[id[i]]]+=siz[id[i]];// 沿 link 累加 endpos
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

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%2);// 只用 a,b 制造大量重复
    return r;
}

int main()
{
    srand(12345);

    // 基础自测：手算例子 ababa
    sam_init();
    string base="ababa";
    n=base.length();
    for(int i=0;i<n;i++)sam_extend(base[i]-'a');
    sam_build();
    printf("s=%s  states=%d  diff_substr=%lld\n",base.c_str(),tot,count_diff_substr());
    printf("occ of \"ab\"=%d  occ of \"ba\"=%d\n",siz[nxt[nxt[1][0]][1]],siz[nxt[nxt[1][1]][0]]);
    printf("lcs(ababa, babab)=%d\n",longest_common("babab"));

    int bad=0;
    for(int rd=1;rd<=800;rd++)
    {
        int ls=rand()%12+1,lt=rand()%8+1;
        string a=rand_str(ls),b=rand_str(lt);
        sam_init();
        n=ls;
        for(int i=0;i<ls;i++)sam_extend(a[i]-'a');
        sam_build();

        // 暴力 1：所有子串丢进 set
        set<string> st;
        for(int i=0;i<ls;i++)
        {
            string cur="";
            for(int j=i;j<ls;j++)cur+=a[j],st.insert(cur);
        }
        if(count_diff_substr()!=(ll)st.size()){bad++;if(bad<=3)printf("diff_substr mismatch a=%s\n",a.c_str());}

        // 暴力 2：每个子串的出现次数
        for(int i=0;i<ls;i++)
        {
            string cur="";
            for(int j=i;j<ls;j++)
            {
                cur+=a[j];
                int p=1;
                for(int k=0;k<(int)cur.length();k++)p=nxt[p][cur[k]-'a'];
                int want=0;
                for(int k=0;k+(int)cur.length()<=ls;k++)if(a.substr(k,cur.length())==cur)want++;
                if(siz[p]!=want){bad++;if(bad<=3)printf("occ mismatch a=%s sub=%s got=%d want=%d\n",a.c_str(),cur.c_str(),siz[p],want);}
            }
        }

        // 暴力 3：最长公共子串
        int want=0;
        for(int i=0;i<ls;i++)
            for(int j=i;j<ls;j++)
            {
                string cur=a.substr(i,j-i+1);
                if(b.find(cur)!=string::npos)want=max(want,(int)cur.length());
            }
        if(longest_common(b)!=want){bad++;if(bad<=3)printf("lcs mismatch a=%s b=%s got=%d want=%d\n",a.c_str(),b.c_str(),longest_common(b),want);}
    }
    printf("random SAM bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
