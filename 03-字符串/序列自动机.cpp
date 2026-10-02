#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
string s;

// 小写字母、0 下标：nxt[i][c] 是从 i 开始（含 i）首个 c 的位置，s.size() 表示不存在。
// 从后往前递推，O(26n) 预处理
int nxt[N][26];

void build_seq(const string &t)
{
    s=t;
    int len=t.length();
    for(int c=0;c<26;c++)nxt[len][c]=len;// 末尾之后什么都没有，用 len 当哨兵
    for(int i=len-1;i>=0;i--)
    {
        for(int c=0;c<26;c++)nxt[i][c]=nxt[i+1][c];
        nxt[i][t[i]-'a']=i;
    }
}

// 判断 t 是否为 s 的子序列，O(|t|)；匹配失败会停在下标 len 处
bool is_subseq(const string &t)
{
    int p=0,len=s.length();
    for(int i=0;i<(int)t.length();i++)
    {
        int c=t[i]-'a';
        if(p>len||nxt[p][c]>=len)return false;
        p=nxt[p][c]+1;// 跳到下一个位置之后继续找
    }
    return true;
}

// 本质不同子序列个数（含空串），O(26n)
// dp[i] 表示前 i 个字符能形成的不同子序列个数，重复的用上一次出现位置减掉
// 注意：个数是指数级的，字符串稍长就会溢出，题目要求取模时给 dp 全程加 %mod 即可
ll count_diff_subseq()
{
    int len=s.length();
    int last[26];
    for(int c=0;c<26;c++)last[c]=-1;
    ll dp[N];
    dp[0]=1;// 空串
    for(int i=1;i<=len;i++)
    {
        int c=s[i-1]-'a';
        dp[i]=dp[i-1]*2;
        if(last[c]!=-1)dp[i]-=dp[last[c]];// 减去上一次以 c 结尾造成的重复
        last[c]=i-1;
    }
    return dp[len];
}
