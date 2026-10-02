// O(n²) 两类 Stirling 三角：第一类无符号 c(n,k) 排列恰有 k 个环；第二类 S(n,k) 集合分成 k 个非空块。
// 若只求一行，可滚动压成 O(n) 空间；有符号第一类乘 (-1)^(n-k)。模 mod>0。
#include<bits/stdc++.h>
using namespace std;
vector<vector<long long>> stirling(int n,long long mod,bool first_kind)
{
    assert(n>=0&&mod>0);vector<vector<long long>> s(n+1,vector<long long>(n+1));s[0][0]=1%mod;
    for(int i=1;i<=n;i++)for(int k=1;k<=i;k++)s[i][k]=(s[i-1][k-1]+(__int128)(first_kind?i-1:k)*s[i-1][k])%mod;
    return s;
}
