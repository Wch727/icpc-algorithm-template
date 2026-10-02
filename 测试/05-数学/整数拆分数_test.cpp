#include "../../05-数学/整数拆分数.cpp"
int main(){for(long long mod:{1LL,12LL,1000000007LL}){int n=200;vector<long long> d(n+1);d[0]=1%mod;for(int x=1;x<=n;x++)for(int i=x;i<=n;i++)d[i]=(d[i]+d[i-x])%mod;assert(partition_numbers(n,mod)==d);}assert(partition_numbers(5,100)[5]==7);}
