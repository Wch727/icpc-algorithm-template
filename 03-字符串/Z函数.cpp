#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;
int n,m;
string s,t;

int z[N];// z[0]=0；z[i] = s 与 s[i..] 的最长公共前缀长度

// 扩展 KMP / Z 数组，O(n)
// 维护 [l,r] 为当前已知最靠右的匹配段 s[l..r]==s[0..r-l]
void get_z(const string &str)
{
    int len=str.length();
    z[0]=0;
    int l=0,r=0;
    for(int i=1;i<len;i++)
    {
        if(i<=r)z[i]=min(r-i+1,z[i-l]);//在匹配段内直接复用
        else z[i]=0;//必须在 while 前清零，否则会复用上一次调用的旧值
        while(i+z[i]<len&&str[z[i]]==str[i+z[i]])z[i]++;
        if(i+z[i]-1>r)l=i,r=i+z[i]-1;
    }
}

// 扩展 KMP：ex[i] = t 与 s[i..] 的最长公共前缀长度（s 是文本，t 是模式）
void get_ex(const string &s,const string &t,int *ex)
{
    int n=s.length(),m=t.length();
    get_z(t);
    int l=0,r=0;
    for(int i=0;i<n;i++)
    {
        if(i<=r)ex[i]=min(r-i+1,z[i-l]);
        else ex[i]=0;//同样先清零再用
        while(i+ex[i]<n&&ex[i]<m&&s[i+ex[i]]==t[ex[i]])ex[i]++;
        if(i+ex[i]-1>r)l=i,r=i+ex[i]-1;
    }
}

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int brute_z(const string &str,int i)
{
    int len=str.length(),c=0;
    while(i+c<len&&str[c]==str[i+c])c++;
    return c;
}

int ex[N];

int main()
{
    srand(12345);

    s="aabaaab";
    get_z(s);
    printf("s=%s  z[1..%d]:",s.c_str(),(int)s.length()-1);
    for(int i=1;i<(int)s.length();i++)printf(" %d",z[i]);
    printf("\n");

    int bad=0;
    for(int rd=1;rd<=3000;rd++)
    {
        int len=rand()%16+1;
        string str=rand_str(len);
        get_z(str);
        for(int i=1;i<len;i++)if(z[i]!=brute_z(str,i))bad++;
        // 顺带对拍扩展 KMP
        string txt=rand_str(rand()%16+1),pat=rand_str(rand()%8+1);
        get_ex(txt,pat,ex);
        for(int i=0;i<(int)txt.length();i++)
        {
            int c=0;
            while(i+c<(int)txt.length()&&c<(int)pat.length()&&txt[i+c]==pat[c])c++;
            if(ex[i]!=c)bad++;
        }
    }
    printf("random z + exkmp bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
