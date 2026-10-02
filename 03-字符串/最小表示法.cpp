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
        int a=(unsigned char)t[(i+k)%len],b=(unsigned char)t[(j+k)%len];
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
        int a=(unsigned char)t[(i+k)%len],b=(unsigned char)t[(j+k)%len];
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
