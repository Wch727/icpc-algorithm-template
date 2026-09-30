#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;
int n,m;
string s,t;

int nxt[N];

// 求模式串 p 的 nxt 数组，nxt[i] 表示 p[0..i-1] 的最长公共前后缀长度
// nxt[0]=-1 便于失配跳转；O(|p|)
void get_nxt(const string &p)
{
    int l=p.length();
    nxt[0]=-1;
    int i=0,j=-1;
    while(i<l)
    {
        if(j==-1||p[i]==p[j])//j=-1 表示从头重新比较
        {
            i++;
            j++;
            nxt[i]=j;
        }
        else j=nxt[j];
    }
}

// 在 s 中找 p 的所有出现次数（允许重叠）；必须先 get_nxt(p)
// 复杂度 O(|s|+|p|)
int kmp(const string &s,const string &p)
{
    int l1=s.length(),l2=p.length();
    int cnt=0,i=0,j=0;
    if(l2==0)return 0;
    while(i<l1)
    {
        if(j==l2)//匹配成功，继续找下一次
        {
            cnt++;
            j=nxt[j];
        }
        if(s[i]==p[j])
        {
            i++;
            j++;
        }
        else
        {
            if(j==0)i++;
            else j=nxt[j];
        }
    }
    if(j==l2)cnt++;
    return cnt;
}

// 最小循环节长度：n-nxt[n] 若能整除 n 就是循环节，否则整串自己当循环节
// 例 "abcab" 长度 5、n-nxt[n]=3 不整除，返回 5
int min_cycle(const string &p)
{
    int l=p.length();
    get_nxt(p);
    int c=l-nxt[l];
    if(l%c)return l;
    return c;
}

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int brute_kmp(const string &s,const string &p)
{
    int l1=s.length(),l2=p.length(),cnt=0;
    if(l2==0||l1<l2)return 0;
    for(int i=0;i+l2<=l1;i++)if(s.substr(i,l2)==p)cnt++;
    return cnt;
}

int main()
{
    srand(12345);

    // 基础自测
    s="abababa",t="aba";
    get_nxt(t);
    printf("t=%s  nxt[1..%d]:",t.c_str(),(int)t.length());
    for(int i=1;i<=(int)t.length();i++)printf(" %d",nxt[i]);
    printf("\n");
    printf("count(abababa,aba)=%d (brute=%d)\n",kmp(s,t),brute_kmp(s,t));
    printf("min_cycle(abcab)=%d\n",min_cycle("abcab"));

    int bad=0;
    // 多轮随机对拍：出现次数
    for(int rd=1;rd<=3000;rd++)
    {
        int l1=rand()%14+1,l2=rand()%6+1;
        string a=rand_str(l1),b=rand_str(l2);
        get_nxt(b);
        int x=kmp(a,b),y=brute_kmp(a,b);
        if(x!=y)
        {
            bad++;
            if(bad<=3)printf("mismatch s=%s p=%s got=%d want=%d\n",a.c_str(),b.c_str(),x,y);
        }
    }
    // 多轮随机对拍：最小循环节（c 整除 len 且 len%c 为循环节位数，且没有更小的）
    for(int rd=1;rd<=3000;rd++)
    {
        int len=rand()%14+1;
        string b=rand_str(len);
        int c=min_cycle(b);
        if(c<1||c>len||len%c!=0)bad++;
        else for(int i=0;i<len;i++)if(b[i]!=b[i%c])bad++;
        for(int k=1;k<c;k++)
        {
            if(len%k)continue;
            bool ok=true;
            for(int i=0;i<len;i++)if(b[i]!=b[i%k])ok=false;
            if(ok)bad++;//存在更小的完整循环节
        }
    }
    printf("random kmp + min_cycle bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
