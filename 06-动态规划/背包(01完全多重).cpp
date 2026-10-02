#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;// 物品数上限
const int M=100005;// 容量上限
const int INF=0x3f3f3f3f;
int n,V;
int w[N],v[N],c[N],typ[N];// 重量 价值 个数 类型(0:01 1:完全 2:多重)
int f[M];// 一维滚动数组，f[j] 表示容量 j 的最优价值
int q[M],qb[M];// 单调队列：存 (旧f[余数+j*w]-j*v) 和这个 j
int exact_ans;

// O(nV)，01 背包：容量不超过 V 的最大价值
int knap_01(int n,int V,int w[],int v[])
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int i=1;i<=n;i++)
        for(int j=V;j>=w[i];j--)// 倒序，保证每个物品最多用一次
            f[j]=max(f[j],f[j-w[i]]+v[i]);
    return f[V];
}

// O(nV)，01 背包恰好装满 V，装不满返回 -INF
int knap_01_exact(int n,int V,int w[],int v[])
{
    for(int j=0;j<=V;j++)f[j]=-INF;
    f[0]=0;// 只有容量 0 是合法起点，其余都不可达
    for(int i=1;i<=n;i++)
        for(int j=V;j>=w[i];j--)
            if(f[j-w[i]]!=-INF)f[j]=max(f[j],f[j-w[i]]+v[i]);
    return f[V];
}

// O(nV)，完全背包：容量正序，同一物品可重复选
int knap_complete(int n,int V,int w[],int v[])
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int i=1;i<=n;i++)
        for(int j=w[i];j<=V;j++)
            f[j]=max(f[j],f[j-w[i]]+v[i]);
    return f[V];
}

// O(V*Σlog(c[i]+1))，多重背包：二进制拆分把 c 个物品拆成 1,2,4,... 个 01 物品
int knap_multiple_binary(int n,int V,int w[],int v[],int c[])
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int i=1;i<=n;i++)
    {
        int k=1,cc=min(c[i],V/w[i]);
        while(cc>0)
        {
            int t=min(k,cc);
            for(int j=V;j>=t*w[i];j--)
                f[j]=max(f[j],f[j-t*w[i]]+t*v[i]);
            cc-=t,k<<=1;
        }
    }
    return f[V];
}

// O(nV)，多重背包：按余数分组 + 单调队列，窗口大小 lim+1
// 同余数下 j=r+k*w，f[j]=max(f[旧j']-k'*v)+k*v，k-k'<=c
int knap_multiple_deque(int n,int V,int w[],int v[],int c[])
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int i=1;i<=n;i++)
    {
        if(w[i]>V)continue;
        int lim=min(c[i],V/w[i]);// 实际最多能放几个
        for(int r=0;r<w[i]&&r<=V;r++)// 枚举余数类
        {
            int head=0,tail=0,k=0;
            for(int j=r;j<=V;j+=w[i],k++)
            {
                int cur=f[j]-k*v[i];// 先取旧值，再覆盖 f[j]
                while(head<tail&&qb[head]<k-lim)head++;// 队首超出 c 个
                while(head<tail&&q[tail-1]<=cur)tail--;// 队尾不优
                q[tail]=cur,qb[tail]=k,tail++;
                f[j]=q[head]+k*v[i];
            }
        }
    }
    return f[V];
}

// O(V*(n+Σ多重log(c[i]+1)))，混合背包：0=01 1=完全 2=多重，多重用二进制拆分
int knap_mixed(int n,int V,int w[],int v[],int c[],int typ[])
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int i=1;i<=n;i++)
    {
        if(typ[i]==0)
            for(int j=V;j>=w[i];j--)f[j]=max(f[j],f[j-w[i]]+v[i]);
        else if(typ[i]==1)
            for(int j=w[i];j<=V;j++)f[j]=max(f[j],f[j-w[i]]+v[i]);
        else
        {
            int k=1,cc=min(c[i],V/w[i]);
            while(cc>0)
            {
                int t=min(k,cc);
                for(int j=V;j>=t*w[i];j--)f[j]=max(f[j],f[j-t*w[i]]+t*v[i]);
                cc-=t,k<<=1;
            }
        }
    }
    return f[V];
}
