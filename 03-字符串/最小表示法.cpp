#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;
int n,m;
string s;

// 最小表示法：求长度为 n 的串循环同构串中字典序最小的那个的起始下标
// 返回原串下标（0 起），比较 s[i+k] 与 s[j+k]，O(n)
int min_show(const string &t)
{
    int len=t.length();
    if(len==0)return 0;
    int i=0,j=1,k=0;// i,j 为两个候选起点，k 为已匹配长度
    while(i<len&&j<len&&k<len)
    {
        int a=t[(i+k)%len],b=t[(j+k)%len];
        if(a==b)k++;
        else
        {
            if(a>b)i=i+k+1;// i 处更差，跳到 k+1 之后
            else j=j+k+1;
            if(i==j)j++;
            k=0;
        }
    }
    return min(i,j);
}

// 返回最小表示的串本身
string min_str(const string &t)
{
    int p=min_show(t),len=t.length();
    string r="";
    for(int i=0;i<len;i++)r+=t[(p+i)%len];
    return r;
}

// 最大表示法：把比较方向反过来即可
int max_show(const string &t)
{
    int len=t.length();
    if(len==0)return 0;
    int i=0,j=1,k=0;
    while(i<len&&j<len&&k<len)
    {
        int a=t[(i+k)%len],b=t[(j+k)%len];
        if(a==b)k++;
        else
        {
            if(a<b)i=i+k+1;
            else j=j+k+1;
            if(i==j)j++;
            k=0;
        }
    }
    return min(i,j);
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
    printf("min_show(banana)=%d  min_str=%s\n",min_show("banana"),min_str("banana").c_str());
    printf("min_show(aaaa)=%d  min_show(bbaa)=%d  max_show(bbaa)=%d\n",min_show("aaaa"),min_show("bbaa"),max_show("bbaa"));

    int bad=0;
    for(int rd=1;rd<=2000;rd++)
    {
        int ls=rand()%12+1;
        string a=rand_str(ls);
        // 暴力：n 种循环移位取 min / max
        string wmin="",wmax="";
        int wpmin=0,wpmax=0;
        for(int k=0;k<ls;k++)
        {
            string cur=a.substr(k)+a.substr(0,k);
            if(k==0||cur<wmin)wmin=cur,wpmin=k;
            if(k==0||cur>wmax)wmax=cur,wpmax=k;
        }
        if(min_str(a)!=wmin||min_show(a)!=wpmin)
        {
            bad++;
            if(bad<=3)printf("min mismatch a=%s got=%s@%d want=%s@%d\n",a.c_str(),min_str(a).c_str(),min_show(a),wmin.c_str(),wpmin);
        }
        if(max_show(a)!=wpmax)
        {
            bad++;
            if(bad<=3)printf("max mismatch a=%s got=%d want=%d\n",a.c_str(),max_show(a),wpmax);
        }
    }
    printf("random min_show bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
