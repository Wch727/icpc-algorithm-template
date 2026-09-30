// 字典树 Trie：插入 / 查询是否出现 / 出现次数 / 前缀计数
// 结点数 N 按「所有串长度之和」开，别只按串个数开
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;

// 字符映射：小写 0~25，大写 26~51，数字 52~61（别的字符集改这里）
int id(char c)
{
    if(c>='a'&&c<='z')return c-'a';
    if(c>='A'&&c<='Z')return c-'A'+26;
    if(c>='0'&&c<='9')return c-'0'+52;
    return 0;
}

struct Trie{
    int tr[N][62];      // tr[p][c] 儿子编号，0 表示没有
    int word[N];        // 以 p 结尾的串个数
    int pre[N];         // 经过 p 的串个数（含以 p 结尾的）
    int tot;            // 已用结点数，根固定为 0，所以从 1 开始编号
    void clear()        // 清空全部：O(结点数*62)，只有一个测试点时可省
    {
        for(int i=0;i<=tot;i++)
        {
            word[i]=0,pre[i]=0;
            for(int j=0;j<62;j++)tr[i][j]=0;
        }
        tot=0;
    }
    void insert(const string &s)              // O(|s|)
    {
        int p=0,len=s.length();
        for(int i=0;i<len;i++)
        {
            int c=id(s[i]);
            if(!tr[p][c])tr[p][c]=++tot;
            p=tr[p][c];
            pre[p]++;
        }
        word[p]++;
    }
    bool exist(const string &s)               // 是否作为完整串出现过，O(|s|)
    {
        int p=0,len=s.length();
        for(int i=0;i<len;i++)
        {
            int c=id(s[i]);
            if(!tr[p][c])return false;
            p=tr[p][c];
        }
        return word[p]>0;
    }
    int count_word(const string &s)           // 完整串出现次数，O(|s|)
    {
        int p=0,len=s.length();
        for(int i=0;i<len;i++)
        {
            int c=id(s[i]);
            if(!tr[p][c])return 0;
            p=tr[p][c];
        }
        return word[p];
    }
    int count_pre(const string &s)            // 以 s 为前缀的串个数（含 s 本身），O(|s|)
    {
        int p=0,len=s.length();
        for(int i=0;i<len;i++)
        {
            int c=id(s[i]);
            if(!tr[p][c])return 0;
            p=tr[p][c];
        }
        return pre[p];
    }
};

Trie t;
string s;
int n;

// 暴力：把所有串存下来，逐个比较，用于对拍
bool brute_exist(const vector<string> &v,const string &s)
{
    for(int i=0;i<(int)v.size();i++)if(v[i]==s)return true;
    return false;
}
int brute_count_word(const vector<string> &v,const string &s)
{
    int c=0;
    for(int i=0;i<(int)v.size();i++)if(v[i]==s)c++;
    return c;
}
int brute_count_pre(const vector<string> &v,const string &s)
{
    int c=0;
    for(int i=0;i<(int)v.size();i++)
    {
        const string &x=v[i];
        if(x.length()>=s.length()&&x.compare(0,s.length(),s)==0)c++;
    }
    return c;
}

// 随机小写串，长度 1~4
string rand_str()
{
    int len=rand()%4+1;
    string res;
    for(int i=1;i<=len;i++)res+=(char)('a'+rand()%3);
    return res;
}

int main()
{
    srand(20240513);

    // 1. 小数据手测：a / ab / abc / ab / abd
    t.clear();
    t.insert("a"),t.insert("ab"),t.insert("abc"),t.insert("ab"),t.insert("abd");
    printf("exist(a)=%d exist(ab)=%d exist(abcd)=%d\n",(int)t.exist("a"),(int)t.exist("ab"),(int)t.exist("abcd"));
    printf("word(ab)=%d word(a)=%d word(abd)=%d\n",t.count_word("ab"),t.count_word("a"),t.count_word("abd"));
    printf("pre(a)=%d pre(ab)=%d pre(abc)=%d pre(abz)=%d\n",t.count_pre("a"),t.count_pre("ab"),t.count_pre("abc"),t.count_pre("abz"));

    // 2. 随机多轮对拍：插入 + 三种查询 与暴力比较
    bool ok=true;
    for(int T=1;T<=20&&ok;T++)
    {
        t.clear();
        vector<string> v;
        n=rand()%12+1;
        for(int i=1;i<=n;i++)
        {
            s=rand_str();
            t.insert(s),v.push_back(s);
        }
        for(int q=1;q<=30;q++)
        {
            s=rand_str();
            if(t.exist(s)!=brute_exist(v,s)){ok=false;break;}
            if(t.count_word(s)!=brute_count_word(v,s)){ok=false;break;}
            if(t.count_pre(s)!=brute_count_pre(v,s)){ok=false;break;}
        }
    }
    printf("random %s\n",ok?"all passed":"FAILED");

    // 3. 大写 + 数字也走一遍，验证字符映射
    t.clear();
    t.insert("A0"),t.insert("A0"),t.insert("A0b");
    printf("A0: exist=%d word=%d pre(A0)=%d\n",(int)t.exist("A0"),t.count_word("A0"),t.count_pre("A0"));

    // 样例：3 个串 ab aa abc，查询 ab / aa / abc / a
    // 输出：exist=1/1/1/0，pre(a)=3
    return 0;
}
