#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;
int n;
double p;// 每步向目标前进的概率
double E[N];// 期望步数
double P[N];// 当前时刻各位置的概率
double A[N][N],bvec[N],sol[N];// 线性方程组用

// O(n^3)，高斯消元解线性方程组，用来和期望 dp 对拍
void gauss(int n,double A[][N],double b[],double x[])
{
    for(int i=0;i<n;i++)
    {
        int piv=i;
        for(int j=i;j<n;j++)
            if(fabs(A[j][i])>fabs(A[piv][i]))piv=j;
        if(fabs(A[piv][i])<1e-14)continue;
        for(int j=0;j<n;j++)swap(A[i][j],A[piv][j]);
        swap(b[i],b[piv]);
        double d=A[i][i];
        for(int j=i;j<n;j++)A[i][j]/=d;
        b[i]/=d;
        for(int j=0;j<n;j++)
            if(j!=i&&fabs(A[j][i])>1e-15)
            {
                double f=A[j][i];
                for(int k=i;k<n;k++)A[j][k]-=f*A[i][k];
                b[j]-=f*b[i];
            }
    }
    for(int i=0;i<n;i++)x[i]=b[i];
}

// ===== 期望 dp 逆推 =====
// 数轴上从 0 出发走到 n，每步以 p 前进、1-p 后退（在 0 处退不动）
// E[n]=0，E[i]=1+p*E[i+1]+(1-p)*E[i-1]，i=0 时后退为原地不动，最后一项为 (1-p)*E[0]
// 这个方程里 E[i] 左右都有，所以不能简单地从后往前推，要解三对角方程（p=0.5 时有公式 n(n+1)-i(i+1)）
// 有限反射区间上 p>0 时期望有限；p=0 且 n>0 时返回 -1
double exp_hit(int n,double p)
{
    if(n==0)return 0;
    if(p<=0)return -1;
    if(p>=1-1e-12)return (double)n;// 每步必进
    int sz=n;// 未知数 E[0..n-1]
    for(int i=0;i<sz;i++)
        for(int j=0;j<sz;j++)A[i][j]=0;
    for(int i=0;i<sz;i++)
    {
        A[i][i]=i==0?p:1;
        if(i+1<sz)A[i][i+1]-=p;// E[i] 里含 p*E[i+1]
        if(i-1>=0)A[i][i-1]-=(1-p);// E[i] 里含 (1-p)*E[i-1]
        bvec[i]=1;
    }
    gauss(sz,A,bvec,sol);
    return sol[0];
}

// 正推法：把"某个时刻还在游走的概率"记作 cost，期望 = sum_{t>=0} P(T>t)
// 这里 T 是首次到达 n 的时刻，P(T>t)=P(X_t<n)
// 下方正推函数均为截断近似：固定步数或当前存活概率小，不等于期望尾项误差已小。
// 剩余误差是 sum(t>=K)P(T>t)；若所有未终止状态的剩余期望<=B，才有尾项<=B*P(T>K)。
// 若终点可能永不到达，期望可为无穷，不能用有限截断结果冒充完整期望。
double exp_forward_walk(int n,double p)
{
    for(int i=0;i<=n;i++)P[i]=0;
    P[0]=1;
    double ans=0;
    int K=300000;
    for(int t=0;t<K;t++)
    {
        double cost=0;
        for(int i=0;i<n;i++)cost+=P[i];// 还没到 n
        ans+=cost;
        if(cost<1e-14)break;
        double np[N];
        for(int i=0;i<=n;i++)np[i]=0;
        for(int i=0;i<n;i++)
        {
            np[i+1]+=P[i]*p;// 前进
            if(i-1>=0)np[i-1]+=P[i]*(1-p);// 后退，0 处退不动所以不加
            else np[0]+=P[i]*(1-p);
        }
        np[n]+=P[n];
        for(int i=0;i<=n;i++)P[i]=np[i];
    }
    return ans;
}

// 正推法第二个例子：二维格子里走
// 每次 a 概率向右、b 概率向上、其余概率不动，走到 (n-1,m-1) 停，求期望步数
double exp_forward_grid(int n,int m,double a,double b)
{
    double st[15][15];
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)st[i][j]=0;
    st[0][0]=1;
    double ans=0;
    int K=200000;
    for(int t=0;t<K;t++)
    {
        double cost=0;
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                if(!(i==n-1&&j==m-1))cost+=st[i][j];// 此刻还没走到终点
        ans+=cost;
        if(cost<1e-14)break;
        double ns[15][15];
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)ns[i][j]=0;
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
            {
                if(i==n-1&&j==m-1){ns[i][j]+=st[i][j];continue;}
                double v=st[i][j];
                ns[i][j]+=v*(1-a-b);// 原地不动
                ns[min(i+1,n-1)][j]+=v*a;// 向右，到边界就停在终点方向
                ns[i][min(j+1,m-1)]+=v*b;// 向上
            }
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)st[i][j]=ns[i][j];
    }
    return ans;
}

// 顺带一个经典期望：n 种卡等概率抽，集齐 n 种的期望次数 = n*H_n
double coupon_expected(int n)
{
    double ans=0;
    for(int i=1;i<=n;i++)ans+=(double)n/i;
    return ans;
}

// 自环移项：E=c+p*E+sum(q[i]*E[i]) => E=(c+sum(q[i]*E[i]))/(1-p)。
// p<1、期望有限；后继互相依赖时仍需解方程组。抽 n 种物品、已有集合 S：
// E[S]=(n+sum(i不在S)E[S并{i}])/(n-|S|)，完成状态为 0，分母是“未抽过的种数”。
// 指示变量：E[sum X[i]]=sum E[X[i]]，无需独立；非负整数 T：E[T]=sum(t>=0)P(T>t)。
// 按概率 p[i] 独立抽元素并移到表头，稳态期望跨越数=sum(i!=j)p[i]*p[j]/(p[i]+p[j])。
// 两者概率都为 0 的项记 0；不要为整个排列建马尔可夫状态。
