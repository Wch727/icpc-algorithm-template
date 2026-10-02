// 适用：批量求 phi/mu/约数个数，数据上界能放入数组时一次 O(n) 预处理。
// 下标：数值范围 1..n，1<=n<N；prime[1..cnt] 为升序素数，1 不是素数。
// 关键：合数只由最小质因子筛一次，遇 i%prime[j]==0 后必须 break。
// 状态：dmin[i] 为最小质因子指数；加同一因子时约数个数仅替换该指数贡献。
// 易错：重复 get_prime 前清 vis、prime 与相关表，单改 cnt 不足以重新筛。
// 结论：phi(1)=mu(1)=d(1)=1；mu=0 表示有平方因子，试除参考版是 O(sqrt x)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000006;

int n;
int prime[N],cnt;// prime[1..cnt] 存的素数
bool vis[N];// 合数标记
int phi[N];// 欧拉函数
int mu[N];// 莫比乌斯函数
int d[N];// 约数个数
int dmin[N];// 最小质因子在该数中的指数

// O(n)，线性筛同时求 phi / mu / d
// O(n)，n 为最大查询值；同因子与新因子两种递推不能混用。
void get_prime(int n)
{
    cnt=0;
    fill(vis,vis+n+1,false);
    vis[1]=true;
    phi[1]=1,mu[1]=1,d[1]=1,dmin[1]=0;
    for(int i=2;i<=n;i++)
    {
        if(!vis[i])
        {
            prime[++cnt]=i;
            phi[i]=i-1;// 素数的 phi
            mu[i]=-1;// 素数有 1 个质因子
            d[i]=2;// 1 和自身
            dmin[i]=1;
        }
        for(int j=1;j<=cnt&&(ll)i*prime[j]<=n;j++)
        {
            int t=i*prime[j];
            vis[t]=true;
            if(i%prime[j]==0)
            {
                // prime[j] 已在 i 里出现过
                phi[t]=phi[i]*prime[j];
                mu[t]=0;
                dmin[t]=dmin[i]+1;
                d[t]=d[i]/(dmin[i]+1)*(dmin[i]+2);
                break;// 每个合数只被最小质因子筛一次
            }
            phi[t]=phi[i]*(prime[j]-1);
            mu[t]=-mu[i];
            dmin[t]=1;
            d[t]=d[i]*2;
        }
    }
}
