// 至多一次失配的最长匹配：先求 LCP，未到串尾则跳过一个字符，再求一次 LCP；两次查询。
// LCP 上界截在两串剩余长度内；哈希有碰撞概率，需严格正确可用 SA+RMQ 的 LCP。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
const int N=1000005;
const ull base1=131;
const ull base2=13331;
int n,m;
string s;

// 单哈希：自然溢出，预处理 O(n)，子串哈希 O(1)
struct Hash1
{
    ull h[N],p[N];
    void build(const string &str)
    {
        int len=str.length();
        p[0]=1;
        h[0]=0;
        for(int i=1;i<=len;i++)
        {
            p[i]=p[i-1]*base1;
            h[i]=h[i-1]*base1+(unsigned char)str[i-1];
        }
    }
    // 下标 1 开始，[l,r] 的哈希值
    ull get(int l,int r)
    {
        if(l>r)return 0;
        return h[r]-h[l-1]*p[r-l+1];
    }
};

// 双哈希：取模 1e9+7 / 1e9+9，降低冲突概率
struct Hash2
{
    static const ull mod1=1000000007;
    static const ull mod2=1000000009;
    ull h1[N],h2[N],p1[N],p2[N];
    void build(const string &str)
    {
        int len=str.length();
        p1[0]=p2[0]=1;
        h1[0]=h2[0]=0;
        for(int i=1;i<=len;i++)
        {
            p1[i]=p1[i-1]*base1%mod1,p2[i]=p2[i-1]*base2%mod2;
            h1[i]=(h1[i-1]*base1+(unsigned char)str[i-1])%mod1;
            h2[i]=(h2[i-1]*base2+(unsigned char)str[i-1])%mod2;
        }
    }
    // 下标 1 开始，返回 [l,r] 的双哈希
    pair<ull,ull> get(int l,int r)
    {
        if(l > r)
            return {0, 0};
        ull x=(h1[r]+mod1-h1[l-1]*p1[r-l+1]%mod1)%mod1;
        ull y=(h2[r]+mod2-h2[l-1]*p2[r-l+1]%mod2)%mod2;
        return {x,y};
    }
};

Hash1 H1;
Hash2 H2;
