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
