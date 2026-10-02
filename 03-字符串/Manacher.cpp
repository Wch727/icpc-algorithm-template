#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;
int n,m;
string s;

// 预处理成 "#a#b#a#" 形式，两端加互不相同且不出现在原串的哨兵，省掉边界判断
int len_cur;
int buf[N<<1];// 负数哨兵与任意字节互不相同
int rr[N<<1];// rr[i]=以 i 为中心的回文半径，数值上等于该中心的最长回文长度

// 求最长回文子串长度 / 回文子串个数，O(n)
struct Manacher
{
    // 传入原始串，返回转化后长度 len_cur，rr[] 为结果
    int build(const string &str)
    {
        int len=0,nn=str.length();
        buf[0]=-1;
        for(int j=0;j<nn;j++)
        {
            buf[++len]=-2;
            buf[++len]=(unsigned char)str[j];
        }
        buf[++len]=-2;
        buf[++len]=-3;
        len_cur=len;
        int r=0,c=0;
        rr[len]=0;
        for(int i=1;i<len;i++)
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
