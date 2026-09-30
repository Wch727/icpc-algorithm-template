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

// O(V*log c)，多重背包：二进制拆分把 c 个物品拆成 1,2,4,... 个 01 物品
int knap_multiple_binary(int n,int V,int w[],int v[],int c[])
{
    for(int j=0;j<=V;j++)f[j]=0;
    for(int i=1;i<=n;i++)
    {
        int k=1,cc=c[i];
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

// O(nV)，混合背包：0=01 1=完全 2=多重，多重用二进制拆分
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
            int k=1,cc=c[i];
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

// 生成 [l,r] 的随机整数
int rndint(int l,int r)
{
    return l+rand()%(r-l+1);
}

// 暴力：第 i 个物品起、剩 rem 容量，按 typ 决定能拿几个
int brute(int i,int rem)
{
    if(i>n)return 0;
    int lim;
    if(typ[i]==0)lim=(w[i]<=rem)?1:0;// 01 也要判放不放得下
    else if(typ[i]==1)lim=rem/w[i];
    else lim=min(c[i],rem/w[i]);
    int best=brute(i+1,rem);
    for(int k=1;k<=lim;k++)best=max(best,brute(i+1,rem-k*w[i])+k*v[i]);
    return best;
}

// 暴力：01 背包恰好装满
void dfs_exact(int i,int rem,int val)
{
    if(i>n)
    {
        if(rem==0)exact_ans=max(exact_ans,val);
        return;
    }
    dfs_exact(i+1,rem,val);
    if(w[i]<=rem)dfs_exact(i+1,rem-w[i],val+v[i]);
}

// 出错的现场数据
void print_data()
{
    printf("n=%d V=%d\n",n,V);
    for(int i=1;i<=n;i++)printf("i=%d w=%d v=%d c=%d typ=%d\n",i,w[i],v[i],c[i],typ[i]);
}

int main()
{
    srand(20240601);
    printf("==== 固定样例 ====\n");
    int w1[5]={0,71,69,1},v1[5]={0,100,1,2};
    printf("01   P1048 T=70 : %d (期望 3)\n",knap_01(3,70,w1,v1));
    printf("完全 P1616 T=70 : %d (期望 140)\n",knap_complete(3,70,w1,v1));
    int w2[5]={0,3,4},v2[5]={0,4,5},c2[5]={0,2,3};
    printf("多重 二进制 V=10 : %d (期望 13)\n",knap_multiple_binary(2,10,w2,v2,c2));
    printf("多重 单调队列 V=10: %d (期望 13)\n",knap_multiple_deque(2,10,w2,v2,c2));
    int w3[5]={0,3,4,2},v3[5]={0,4,5,3},c3[5]={0,1,3,2},t3[5]={0,0,1,2};
    printf("混合 V=10 : %d (期望 13)\n",knap_mixed(3,10,w3,v3,c3,t3));
    printf("01 恰好装满 V=10 : %d (1061109567 表示装不满)\n",knap_01_exact(2,10,w2,v2));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0,ref,cur;
    for(tt=1;tt<=800;tt++)
    {
        n=rndint(1,7),V=rndint(1,25);
        for(int i=1;i<=n;i++)w[i]=rndint(1,10),v[i]=rndint(1,15),c[i]=rndint(1,4);
        for(int i=1;i<=n;i++)typ[i]=0;// 01
        ref=brute(1,V),cur=knap_01(n,V,w,v);
        if(ref!=cur){bad++;printf("WA! 01 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        for(int i=1;i<=n;i++)typ[i]=1;// 完全
        ref=brute(1,V),cur=knap_complete(n,V,w,v);
        if(ref!=cur){bad++;printf("WA! 完全 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        for(int i=1;i<=n;i++)typ[i]=2;// 多重
        ref=brute(1,V),cur=knap_multiple_binary(n,V,w,v,c);
        if(ref!=cur){bad++;printf("WA! 多重拆分 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        cur=knap_multiple_deque(n,V,w,v,c);
        if(ref!=cur){bad++;printf("WA! 多重单调队列 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        for(int i=1;i<=n;i++)typ[i]=rndint(0,2);// 混合
        ref=brute(1,V),cur=knap_mixed(n,V,w,v,c,typ);
        if(ref!=cur){bad++;printf("WA! 混合 轮%d ref=%d cur=%d\n",tt,ref,cur);print_data();break;}
        exact_ans=-INF;
        dfs_exact(1,V,0);
        cur=knap_01_exact(n,V,w,v);
        if(exact_ans!=cur){bad++;printf("WA! 恰好装满 轮%d ref=%d cur=%d\n",tt,exact_ans,cur);break;}
    }
    if(!bad)printf("stress OK (800 轮，01/完全/多重/混合/恰好装满 全部通过)\n");
    return 0;
}
