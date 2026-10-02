#include "../../05-数学/合数模组合数exLucas.cpp"
int main(){for(int m=1;m<=120;m++){vector<vector<ll>> c(61,vector<ll>(61));for(int n=0;n<=60;n++){c[n][0]=c[n][n]=1%m;for(int k=1;k<n;k++)c[n][k]=(c[n-1][k-1]+c[n-1][k])%m;for(int k=0;k<=n;k++)assert(exlucas(n,k,m)==c[n][k]);}}assert(exlucas(1000000000000000000ULL,1,72)==1000000000000000000ULL%72);}
