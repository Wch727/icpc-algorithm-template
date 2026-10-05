#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=205,M=25;
const ll INF=4000000000000000000LL;
int n,K;
ll a[N],s[N],w[N][N],fd[N][N];
int opt[N][N],dq[N],beg[N],hd,tl;

// 非负 a，w[l][r]=(s[r]-s[l-1])²；分成 k 个非空连续段。
// fd[k][i]=min_{k-1<=j<i} fd[k-1][j]+w[j+1][i]。
// 要先证明决策单调性；不能仅凭样例或“区间代价”就套。
// 数值及加法须小于 INF；O(K n log n)。
void solve_layer(int k,int l,int r,int optl,int optr)
{
    if(l>r)return;
    int mid=(l+r)/2,best=optl;
    ll val=INF;
    for(int j=optl;j<=min(mid-1,optr);j++)
        if(fd[k-1][j]+w[j+1][mid]<val)
            val=fd[k-1][j]+w[j+1][mid],best=j;
    fd[k][mid]=val,opt[k][mid]=best;
    solve_layer(k,l,mid-1,optl,best);
    solve_layer(k,mid+1,r,best,optr);
}
void dnc_partition()
{
    for(int i=1;i<=n;i++)fd[1][i]=w[1][i],opt[1][i]=0;
    for(int k=2;k<=min(K,n);k++)solve_layer(k,k,n,k-1,n-1);
}

// 单层：fd[2][i]=min_{1<=j<i} fd[1][j]+w[j+1][i]。
// 前提：新决策胜过旧决策的位置形成后缀，最优决策单调不减。
// dq 存决策，beg 存它开始胜出的下标；每个决策进出一次，O(n log n)。
void mq_layer()
{
    if(n<2)return;
    auto value=[&](int j,int i){return fd[1][j]+w[j+1][i];};
    hd=0,tl=1,dq[0]=1,beg[0]=2;
    for(int i=2;i<=n;i++)
    {
        while(hd+1<tl&&beg[hd+1]<=i)++hd;
        fd[2][i]=value(dq[hd],i);
        if(i==n)break;
        while(hd<tl&&value(i,max(i+1,beg[tl-1]))<=value(dq[tl-1],max(i+1,beg[tl-1])))--tl;
        if(hd==tl){dq[tl]=i,beg[tl++]=i+1; continue;}
        int old=dq[tl-1];
        if(value(i,n)>value(old,n))continue;
        int l=max(i+1,beg[tl-1]),rr=n;
        while(l<rr)
        {
            int mid=(l+rr)/2;
            if(value(i,mid)<=value(old,mid))rr=mid;
            else l=mid+1;
        }
        dq[tl]=i,beg[tl++]=l;
    }
}
