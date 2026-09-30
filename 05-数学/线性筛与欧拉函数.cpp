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

int main()
{
    int M=30000;
    get_prime(M);
    int bad=0,pcnt=0;
    for(int i=1;i<=M;i++)
    {
        if(!vis[i]&&i>1)pcnt++;
        if(phi[i]!=phi_naive(i))bad++;
        if(mu[i]!=mu_naive(i))bad++;
        if(d[i]!=d_naive(i))bad++;
    }
    if(pcnt!=cnt)bad++;
    // 素数表本身查一遍
    for(int i=2;i<=M;i++)
    {
        bool p=true;
        for(int j=2;(ll)j*j<=i;j++)
            if(i%j==0){p=false;break;}
        if(p!=!vis[i])bad++;
    }
    // 再筛满一遍，用 1e6 的边界值验证
    get_prime(1000000);
    if(phi[1000000]!=phi_naive(1000000))bad++;
    if(d[1000000]!=d_naive(1000000))bad++;
    if(mu[1000000]!=mu_naive(1000000))bad++;
    if(phi[999983]!=999982)bad++;// 999983 是素数
    printf("prime count <= %d : %d\n",M,cnt);
    printf("phi(1..8) = ");
    for(int i=1;i<=8;i++)printf("%d ",phi[i]);
    printf("\nmu(1..8) = ");
    for(int i=1;i<=8;i++)printf("%d ",mu[i]);
    printf("\nd(1..8)  = ");
    for(int i=1;i<=8;i++)printf("%d ",d[i]);
    printf("\nphi(1e6) = %d, d(1e6) = %d, mu(1e6) = %d\n",phi[1000000],d[1000000],mu[1000000]);
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P3383 输入 100 5 后跟 1 2 3 4 5 -> 2 3 5 7 11
// 样例：P1865 输入 2 5 -> 1 3 得 2, 2 5 得 3
// 边界：phi[1]=mu[1]=d[1]=1；n<2 时筛出的素数表为空

/*
自测记录：
  1) 素数表与 O(sqrt n) 试除逐一比对；
  2) phi / mu / d 三个数组在 1..30000 上与 O(sqrt n) 暴力对拍。
*/
