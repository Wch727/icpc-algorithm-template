// p(n)：把 n 拆成正整数之和，不计顺序；五边形数定理，O(n sqrt n)、O(n)。
// p(0)=1，递推使用 j(3j±1)/2，符号按 j 奇偶交替；mod>0，不要求素数。
#include<bits/stdc++.h>
using namespace std;
vector<long long> partition_numbers(int n,long long mod)
{
    assert(n>=0&&mod>0);vector<long long> p(n+1);p[0]=1%mod;
    for(int i=1;i<=n;i++)for(long long j=1;j*(3*j-1)/2<=i;j++)
    {
        int a=j*(3*j-1)/2,b=j*(3*j+1)/2;__int128 v=p[a<=i?i-a:0];
        if(b<=i)v+=p[i-b];if(!(j&1))v=-v;
        p[i]=(p[i]+v)%mod;if(p[i]<0)p[i]+=mod;
    }
    return p;
}
