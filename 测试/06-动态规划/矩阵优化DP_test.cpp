// 矩阵优化DP 的测试与对拍代码
// 模板本体：06-动态规划/矩阵优化DP.cpp
#include "../../06-动态规划/矩阵优化DP.cpp"
ll solve(const vector<ll> &arg_c,const vector<ll> &arg_f,ll n){c=arg_c;f=arg_f;return solve(n);}

int main()
{
    srand(19260817);
    bool ok=solve({1,1},{0,1},10)==55;
    for(int t=1;t<=100;t++)
    {
        int k=rand()%6+1,n=rand()%101;
        vector<ll> c(k),f(k),dp(max(k,n+1));
        for(int i=0;i<k;i++)c[i]=rand()%10,f[i]=dp[i]=rand()%100;
        for(int i=k;i<=n;i++)
            for(int j=0;j<k;j++)dp[i]=(dp[i]+c[j]*dp[i-j-1])%MOD;
        if(solve(c,f,n)!=dp[n])ok=false;
    }
    printf("矩阵优化DP %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
