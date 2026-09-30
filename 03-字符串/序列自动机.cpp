#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
string s;

// 序列自动机：nxt[i][c] 表示位置 i 之后（不含 i）第一个字符 c 的下标，n+1 表示没有
// 从后往前递推，O(26n) 预处理
int nxt[N][26];

void build_seq(const string &t)
{
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

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%2);// 只用 a,b 制造大量重复
    return r;
}

int main()
{
    srand(12345);

    // 基础自测
    s="ababc";
    build_seq(s);
    printf("s=%s  is_subseq(abc)=%d  is_subseq(aac)=%d  is_subseq(cba)=%d\n",s.c_str(),(int)is_subseq("abc"),(int)is_subseq("aac"),(int)is_subseq("cba"));
    printf("diff_subseq(ababc)=%lld (含空串)\n",count_diff_subseq());

    int bad=0;
    for(int rd=1;rd<=1200;rd++)
    {
        int ls=rand()%12+1;
        s=rand_str(ls);
        build_seq(s);

        // 暴力 1：枚举 2^ls 个子序列放进 set
        set<string> st;
        for(int mask=0;mask<(1<<ls);mask++)
        {
            string cur="";
            for(int i=0;i<ls;i++)if(mask>>i&1)cur+=s[i];
            st.insert(cur);
        }
        if(count_diff_subseq()!=(ll)st.size())
        {
            bad++;
            if(bad<=3)printf("diff_subseq mismatch s=%s got=%lld want=%d\n",s.c_str(),count_diff_subseq(),(int)st.size());
        }

        // 暴力 2：随机小串判断子序列
        for(int q=1;q<=5;q++)
        {
            int lt=rand()%5+1;
            string t=rand_str(lt);
            bool want=false;
            for(int mask=0;mask<(1<<ls)&&!want;mask++)
            {
                string cur="";
                for(int i=0;i<ls;i++)if(mask>>i&1)cur+=s[i];
                if(cur==t)want=true;
            }
            if(is_subseq(t)!=want)
            {
                bad++;
                if(bad<=3)printf("subseq mismatch s=%s t=%s got=%d want=%d\n",s.c_str(),t.c_str(),(int)is_subseq(t),(int)want);
            }
        }
    }
    printf("random seq automaton bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
