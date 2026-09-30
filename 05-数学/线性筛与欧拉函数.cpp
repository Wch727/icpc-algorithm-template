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
void get_prime(int n)
{
    cnt=0;
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

// O(sqrt n)，单个数的欧拉函数，对拍用
int phi_naive(int x)
{
    int r=x;
    for(int i=2;(ll)i*i<=x;i++)
        if(x%i==0)
        {
            r=r/i*(i-1);
            while(x%i==0)x/=i;
        }
    if(x>1)r=r/x*(x-1);
    return r;
}

// O(sqrt n)，约数个数，对拍用
int d_naive(int x)
{
    int r=0;
    for(int i=1;(ll)i*i<=x;i++)
        if(x%i==0)r+=(i*i==x?1:2);
    return r;
}

// O(sqrt n)，莫比乌斯函数，对拍用
int mu_naive(int x)
{
    if(x==1)return 1;
    int r=1;
    for(int i=2;(ll)i*i<=x;i++)
        if(x%i==0)
        {
            x/=i;
            if(x%i==0)return 0;
            r=-r;
        }
    if(x>1)r=-r;
    return r;
}
