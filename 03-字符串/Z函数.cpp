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
