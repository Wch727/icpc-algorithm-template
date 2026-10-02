// 字典树 Trie：插入 / 查询是否出现 / 出现次数 / 前缀计数
// 结点数 N 按「所有串长度之和」开，别只按串个数开
// 设 f[j] 为某 j 个已插入串能共享的最长前缀：节点深 h、经过数 cnt 增 1 时，只更新 f[cnt]。
// 更小 j 的答案此前已>=h；每次插入只访问该串路径，总 O(字符串总长度)，不必区间 chmax。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;

// 输入仅允许字母数字；其他字符需修改 id。Trie 实例须零初始化（例如全局 t）。
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
    int tot=0;            // 已用结点数，根固定为 0，所以从 1 开始编号
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
        pre[0]++;
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
