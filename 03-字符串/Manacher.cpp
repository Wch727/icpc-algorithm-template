#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;
int n,m;
string s;

// 预处理成 "#a#b#a#" 形式，两端加互不相同且不出现在原串的哨兵，省掉边界判断
int len_cur;
char buf[N<<1];
int rr[N<<1];// rr[i]=以 i 为中心的回文半径，数值上等于该中心的最长回文长度

// 求最长回文子串长度 / 回文子串个数，O(n)
struct Manacher
{
    // 传入原始串，返回转化后长度 len_cur，rr[] 为结果
    int build(const string &str)
    {
        int len=0,nn=str.length();
        buf[0]='%';
        for(int j=0;j<nn;j++)
        {
            buf[++len]='#';
            buf[++len]=str[j];
        }
        buf[++len]='#';
        buf[++len]='!';
        // 反复调用时后面残留着上一次的 '!' 会让 while 一直比较下去，必须清干净
        buf[len+1]='\0',buf[len+2]='\0';
        len_cur=len;
        int r=0,c=0;
        for(int i=1;i<=len;i++)
        {
            rr[i]=0;//同样先清零，避免复用上一次调用的旧半径
            if(i<=r)rr[i]=min(rr[(c<<1)-i],r-i);//镜像复制，右端不超过 r
            while(buf[i+rr[i]+1]==buf[i-rr[i]-1])rr[i]++;
            if(rr[i]+i>r)r=i+rr[i],c=i;
        }
        return len;
    }
    // 最长回文子串长度
    int longest()
    {
        int ans=0;
        for(int i=1;i<=len_cur;i++)if(rr[i]>ans)ans=rr[i];
        return ans;
    }
    // 回文子串个数（按出现位置计，可重复）
    // 每个中心贡献 (rr[i]+1)/2 个：偶数中心 rr 为偶数，奇数中心 rr 为奇数
    ll count_pal()
    {
        ll ans=0;
        for(int i=1;i<=len_cur;i++)ans+=(rr[i]+1)>>1;
        return ans;
    }
};

Manacher M;

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int brute_longest(const string &str)
{
    int len=str.length(),ans=0;
    for(int i=0;i<len;i++)
    for(int j=i;j<len;j++)
    {
        bool ok=true;
        for(int x=i,y=j;x<y;x++,y--)if(str[x]!=str[y]){ok=false;break;}
        if(ok&&j-i+1>ans)ans=j-i+1;
    }
    return ans;
}

ll brute_count(const string &str)
{
    int len=str.length();
    ll ans=0;
    for(int i=0;i<len;i++)
    for(int j=i;j<len;j++)
    {
        bool ok=true;
        for(int x=i,y=j;x<y;x++,y--)if(str[x]!=str[y]){ok=false;break;}
        if(ok)ans++;
    }
    return ans;
}

int main()
{
    srand(12345);

    s="abacaba";
    M.build(s);
    printf("s=%s  longest=%d (brute=%d)  count=%lld (brute=%lld)\n",
        s.c_str(),M.longest(),brute_longest(s),M.count_pal(),brute_count(s));

    int bad=0;
    for(int rd=1;rd<=3000;rd++)
    {
        int len=rand()%13+1;
        string str=rand_str(len);
        M.build(str);
        int a=M.longest(),b=brute_longest(str);
        ll c=M.count_pal(),dd=brute_count(str);
        if(a!=b||c!=dd)
        {
            bad++;
            if(bad<=3)printf("mismatch s=%s longest %d/%d count %lld/%lld\n",str.c_str(),a,b,c,dd);
        }
    }
    printf("random manacher bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
