// 概率期望DP 的测试与对拍代码
// 模板本体：06-动态规划/概率期望DP.cpp
#include "../../06-动态规划/概率期望DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

double rndreal()// 生成 [0,1) 的随机实数
{
    return (double)rand()/RAND_MAX;
}

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

int main()
{
    srand(20240701);
    printf("==== 固定样例 ====\n");
    // 原期望写错：0 处后退停留，独立概率推进得到 20、11.9753、40.9375
    printf("逆推 p=0.50 n=4 : %.4f (期望 20.0000 = n*(n+1))\n",exp_hit(4,0.5));
    printf("逆推 p=0.60 n=4 : %.4f (期望 11.9753)\n",exp_hit(4,0.6));
    printf("正推 p=0.60 n=4 : %.4f (与上一行一致)\n",exp_forward_walk(4,0.6));
    printf("逆推 p=1.00 n=4 : %.4f (期望 4.0000)\n",exp_hit(4,1.0));
    printf("逆推 p=0.40 n=4 : %.4f (期望 40.9375)\n",exp_hit(4,0.4));
    printf("正推 2x2 网格 a=0.4 b=0.3 : %.6f\n",exp_forward_grid(2,2,0.4,0.3));
    printf("抽卡集齐 4 种 : %.4f (期望 8.3333 = 4*H4)\n",coupon_expected(4));

    printf("==== 随机对拍 ====\n");
    int bad=0;
    for(int tt=1;tt<=30;tt++)
    {
        int nn=rndint(2,8);
        double pp=0.40+0.55*rndreal();// 覆盖后退概率更大的分支
        pp=llround(pp*100)/100.0;
        if(pp<0.40)pp=0.40;
        if(pp>0.95)pp=0.95;
        double cur=exp_hit(nn,pp),ref=exp_forward_walk(nn,pp);
        if(fabs(cur-ref)>1e-6)
        {
            bad++;
            printf("FAILED! 逆推 vs 正推 轮%d n=%d p=%.4f 逆推=%.6f 正推=%.6f\n",tt,nn,pp,cur,ref);
            break;
        }
    }
    for(int tt=1;tt<=20&&!bad;tt++)// 正推网格：小概率且状态少时结果应当在合理范围
    {
        int nn=rndint(1,3),mm=rndint(1,3);
        double a=0.2+0.3*rndreal(),b=0.2+0.3*rndreal();
        double cur=exp_forward_grid(nn,mm,a,b);
        if(!(cur>=0)||cur!=cur||cur>1e7){bad++;printf("FAILED! 正推网格 轮%d 结果异常 %.6f\n",tt,cur);break;}
    }
    if(!bad)printf("stress OK (30 组逆推 vs 时间推进正推 + 20 组正推网格 全部通过)\n");
    return 0;
}

/*
概率期望 dp 两种方向：
1. 逆推：E[终态]=0，E[i]=1+sum_j P(i->j)*E[j]，从后往前算。
   注意方程里如果 E[i] 自己也出现（原地不动/后退），要移项解方程，
   模板里 p=0.5 时 E[i]=n(n+1)-i(i+1) 可以直接用，p!=0.5 用高斯消元或三对角递推。
2. 正推：先按时间推概率分布 P_t[i]，再用 E[T]=sum_{t>=0} P(T>t) 求期望；
   适合状态数小、步数上限明确的题（比如本题网格只有 9 个状态）。
- 期望的线性性：E[X+Y]=E[X]+E[Y]，把总期望拆成每条边/每种卡片的贡献往往更好写。
*/
