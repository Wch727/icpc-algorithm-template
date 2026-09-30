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
            h[i]=h[i-1]*base1+(ull)str[i-1];
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
            h1[i]=(h1[i-1]*base1+(ull)str[i-1])%mod1;
            h2[i]=(h2[i-1]*base2+(ull)str[i-1])%mod2;
        }
    }
    // 下标 1 开始，返回 [l,r] 的双哈希
    pair<ull,ull> get(int l,int r)
    {
        if(l>r)return {0,0};
        ull x=(h1[r]+mod1-h1[l-1]*p1[r-l+1]%mod1)%mod1;
        ull y=(h2[r]+mod2-h2[l-1]*p2[r-l+1]%mod2)%mod2;
        return {x,y};
    }
};

Hash1 H1;
Hash2 H2;

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%2);
    return r;
}

int main()
{
    srand(12345);

    // 基础自测：子串哈希与暴力比较
    s="abacababca";
    H1.build(s);
    H2.build(s);
    printf("s=%s\n",s.c_str());
    printf("get(3,6)=%llu  get(3,6)double=(%llu,%llu)\n",H1.get(3,6),H2.get(3,6).first,H2.get(3,6).second);

    // 多轮随机对拍：相等子串哈希必相等，不等子串哈希必不等
    int bad=0;
    for(int rd=1;rd<=2000;rd++)
    {
        int len=rand()%12+1;
        string str=rand_str(len);
        H1.build(str);
        H2.build(str);
        for(int l=1;l<=len;l++)
        for(int r=l;r<=len;r++)
        for(int x=1;x<=len;x++)
        for(int y=x;y<=len;y++)
        {
            bool same=(str.substr(l-1,r-l+1)==str.substr(x-1,y-x+1));
            if((H1.get(l,r)==H1.get(x,y))!=same)bad++;
            if((H2.get(l,r)==H2.get(x,y))!=same)bad++;
        }
    }
    printf("random sub-hash check bad=%d\n",bad);

    // 用法示例：P3370 不同字符串个数
    set<pair<ull,ull>> st;
    for(int i=1;i<=1000;i++)st.insert(H2.get(1,i%17+1));
    printf("distinct hash count=%d\n",(int)st.size());

    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
