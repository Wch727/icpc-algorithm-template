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

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int main()
{
    srand(12345);

    // 基础自测：模式串 he/she/his/hers，文本 ushershe
    clear_trie();
    vector<string> base={"he","she","his","hers"};
    for(int i=0;i<(int)base.size();i++)id[i]=insert(base[i]);
    build_fail();
    string txt="ushershe";
    printf("text=%s  total=%lld\n",txt.c_str(),query(txt));
    for(int i=0;i<(int)base.size();i++)
    {
        int c=0;
        for(int p=0;p+(int)base[i].length()<=(int)txt.length();p++)
            if(txt.substr(p,base[i].length())==base[i])c++;
        printf("  pattern %-5s appear=%d\n",base[i].c_str(),c);
    }

    int bad=0;
    for(int rd=1;rd<=1500;rd++)
    {
        clear_trie();
        int k=rand()%5+1;
        vector<string> pat;
        for(int i=1;i<=k;i++)
        {
            string p=rand_str(rand()%4+1);
            pat.push_back(p);
            id[i]=insert(p);//记录每个模式串的终止结点
        }
        build_fail();
        string tx=rand_str(rand()%12+1);
        ll got=query(tx),want=0;
        // 暴力：枚举每个模式串在文本中的每次出现
        for(int i=0;i<k;i++)
        {
            int pl=pat[i].length();
            for(int p=0;p+pl<=(int)tx.length();p++)
                if(tx.substr(p,pl)==pat[i])want++;
        }
        if(got!=want)
        {
            bad++;
            if(bad<=3)printf("mismatch text=%s got=%lld want=%lld\n",tx.c_str(),got,want);
        }
    }
    printf("random AC automaton bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
