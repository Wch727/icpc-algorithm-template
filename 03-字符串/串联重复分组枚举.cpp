// 枚举 tandem repeat（相邻两个相同长度块），以 {周期p,起点闭区间lo..hi} 压缩输出。
// 用 SA+LCP/RMQ 的 LCE 分组法覆盖 Main–Lorentz 的重复枚举用途，不枚举所有 O(n²) 个答案。
// 每个 (p,start) 恰出现一次，枚举 O(n log n)，本版 SA 排序构建 O(n log²n)，空间 O(n log n)。
// period 是这次两个块的长度，未必是字符串最小周期；输入任意 byte，不含隐式终止符。
#include<bits/stdc++.h>
using namespace std;
struct StringLCE
{
    int n;vector<int>rank,lg;vector<vector<int>>st;
    StringLCE(const string&s):n(s.size()),rank(n),lg(n+1)
    {
        vector<int>sa(n),next(n),lcp(n);iota(sa.begin(),sa.end(),0);for(int i=0;i<n;i++)rank[i]=(unsigned char)s[i];
        for(int len=1;len<n;len*=2){auto key=[&](int i){return pair(rank[i],i+len<n?rank[i+len]:-1);};sort(sa.begin(),sa.end(),[&](int i,int j){return key(i)<key(j);});next[sa[0]]=0;for(int i=1;i<n;i++)next[sa[i]]=next[sa[i-1]]+(key(sa[i])!=key(sa[i-1]));rank=next;if(rank[sa.back()]==n-1)break;}
        if(n==1)rank[0]=0;
        int h=0;for(int i=0;i<n;i++){int r=rank[i];if(!r){h=0;continue;}int j=sa[r-1];while(i+h<n&&j+h<n&&s[i+h]==s[j+h])h++;lcp[r]=h;if(h)h--;}
        for(int i=2;i<=n;i++)lg[i]=lg[i/2]+1;st.push_back(lcp);for(int k=1;(1<<k)<=n;k++){st.push_back(vector<int>(n));for(int i=0;i+(1<<k)<=n;i++)st[k][i]=min(st[k-1][i],st[k-1][i+(1<<(k-1))]);}
    }
    int query(int i,int j)const{if(i<0||j<0||i>=n||j>=n)return 0;if(i==j)return n-i;int a=rank[i],b=rank[j];if(a>b)swap(a,b);int k=lg[b-a];return min(st[k][a+1],st[k][b-(1<<k)+1]);}
};
string s;
vector<array<int,3>> tandem_repeats()
{
    int n=s.size();StringLCE forward(s);string rev=s;reverse(rev.begin(),rev.end());StringLCE backward(rev);vector<array<int,3>>ans;
    for(int p=1;p*2<=n;p++)for(int i=0;i+p<n;i+=p)
    {int left=i?backward.query(n-i,n-i-p):0,right=forward.query(i,i+p);int l=max({0,i-left,i-p+1}),r=min({i,i+right-p,n-2*p});if(l<=r)ans.push_back({p,l,r});}
    return ans;
}
