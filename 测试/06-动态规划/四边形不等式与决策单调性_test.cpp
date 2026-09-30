// 四边形不等式与决策单调性 的测试与对拍代码
// 模板本体：06-动态规划/四边形不等式与决策单调性.cpp
#include "../../06-动态规划/四边形不等式与决策单调性.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// ===== 四边形不等式（QI）判定 =====
// 区间代价 w(l,r) 满足 QI：对任意 a<=b<=c<=d 有
//   w(a,c)+w(b,d) <= w(a,d)+w(b,c)
// 结论（充分条件）：若 w 满足 QI 且 w 关于区间包含单调（w(a,d)>=w(b,c)），
// 则 dp[i]=min_{j<i} dp[j]+w(j+1,i) 的决策点 opt[i] 单调不减；
// 区间 dp f[l][r]=min_{l<=k<r} f[l][k]+f[k+1][r]+w(l,r) 有 opt[l][r-1]<=opt[l][r]<=opt[l+1][r]
// 常见满足 QI 的代价：(sum)^2、区间长度、区间最值；不满足时不能乱用决策单调性
namespace QI
{
    ll c[N][N];

    // O(n^4)，暴力检查 QI 是否成立，返回违反次数
    int check(int n)
    {
        int bad=0;
        for(int aa=1;aa<=n;aa++)
            for(int b=aa;b<=n;b++)
                for(int cc=b;cc<=n;cc++)
                    for(int dd=cc;dd<=n;dd++)
                        if(c[aa][cc]+c[b][dd]>c[aa][dd]+c[b][cc])bad++;
        return bad;
    }

    // O(n^3)，检查 dp[j]=min_{i<j} cost[i][j] 的决策点是否单调不减
    int check_mono(int n)
    {
        int last=0;
        for(int j=2;j<=n;j++)
        {
            int bj=-1;ll bv=INF;
            for(int i=1;i<j;i++)
                if(c[i][j]<bv)bv=c[i][j],bj=i;
            if(bj<last)return 1;// 出现下降
            last=bj;
        }
        return 0;
    }
}

// O(n^2 K)，分层 dp：把前 i 个数分成 k 段，段代价是"段内和的平方"，求最小总代价
// f[k][i]=min_{j<i} f[k-1][j] + w(j+1,i)，其中 w(l,r)=(s[r]-s[l-1])^2
void brute_partition(int n,int K,ll w[][N],ll f[][N])
{
    for(int i=1;i<=n;i++)f[1][i]=w[1][i];
    for(int k=2;k<=K;k++)
        for(int i=k;i<=n;i++)
        {
            f[k][i]=INF;
            for(int j=k-1;j<i;j++)f[k][i]=min(f[k][i],f[k-1][j]+w[j+1][i]);
        }
}

// ===== 写法一：分治优化，O(n K log n) =====
// 已知 opt[k][i] 随 i 单调不减，用分治求 f[k][*]
// 每层枚举总量 O(n log n)：先算中点 mid，再按 mid 的决策点夹出左右两半的取值范围
void solve_layer(int k,int l,int r,int optl,int optr)
{
    if(l>r)return;
    int mid=(l+r)>>1;
    int bestj=-1;ll bv=INF;
    int hi=min(mid-1,optr);
    for(int j=optl;j<=hi;j++)
    {
        ll v=fd[k-1][j]+w[j+1][mid];
        if(v<bv)bv=v,bestj=j;
    }
    fd[k][mid]=bv,opt[k][mid]=bestj;
    solve_layer(k,l,mid-1,optl,bestj);
    solve_layer(k,mid+1,r,bestj,optr);
}

void dnc_partition(int n,int K,ll w[][N],ll f[][N],int opt[][N])
{
    for(int i=1;i<=n;i++)f[1][i]=w[1][i],opt[1][i]=0;
    for(int k=2;k<=K;k++)solve_layer(k,k,n,k-1,n-1);
}

// ===== 写法二：单调队列 + 二分找分界点，均摊 O(n log n) =====
// 队列里放若干"决策候选人"，相邻两人的胜负分界点 pos 递增
// 加入新决策 j 时二分出它从哪个位置开始优于队尾，若整段都优于队尾就弹掉队尾
// 这里演示单层：f[i]=min_{1<=j<i} fd[1][j]+w(j+1,i)
ll costd(int j,int i)
{
    return fd[1][j]+w[j+1][i];
}

void mq_layer(int n,ll w[][N])
{
    hd=tl=0;
    for(int i=2;i<=n;i++)
    {
        while(hd+1<tl&&costd(dq[hd+1],i)<=costd(dq[hd],i))hd++;// 队首被下一个超过
        if(hd>=tl)dq[0]=i-1,tl=1,hd=0;// 队列空了，直接用 i-1 起步
        while(hd<tl)
        {
            int p=dq[tl-1];
            if(costd(i,p)<=costd(p,p))tl--;// 新决策在 p 处就更优，队尾整段作废
            else break;
        }
        if(hd>=tl)dq[0]=i,tl=1,hd=0;
        else
        {
            int lo=1,hi=n,be=-1,p=dq[tl-1];// 找第一个新决策更优的位置
            while(lo<=hi)
            {
                int mid=(lo+hi)>>1;
                if(costd(i,mid)<=costd(p,mid))be=mid,hi=mid-1;
                else lo=mid+1;
            }
            if(be!=-1)dq[tl++]=i;
        }
        int best=dq[hd];
        for(int t=hd;t<tl;t++)if(costd(dq[t],i)<costd(best,i))best=dq[t];// 保险：真取最小（队列已排序，可不写）
        fd[2][i]=costd(best,i);
    }
}

int main()
{
    srand(20240618);
    printf("==== 四边形不等式判定 ====\n");
    // 代价 w(l,r)=(sum)^2 满足 QI
    int qn=8;
    for(int l=1;l<=qn;l++)
    {
        ll sum=0;
        for(int r=l;r<=qn;r++)sum+=r,QI::c[l][r]=sum*sum;
    }
    printf("w(l,r)=(sum)^2   QI 违反次数 %d，决策是否非单调 %d (期望 0 0)\n",
        QI::check(qn),QI::check_mono(qn));
    // 代价 w(l,r)=((l+r)&1) 不满足 QI，决策点就会不单调
    for(int l=1;l<=qn;l++)
        for(int r=l;r<=qn;r++)QI::c[l][r]=((l+r)&1);
    printf("w(l,r)=((l+r)&1) QI 违反次数 %d，决策是否非单调 %d (期望 >0 1)\n",
        QI::check(qn),QI::check_mono(qn));

    printf("==== 固定样例 ====\n");
    n=5,K=2;
    for(int i=1;i<=n;i++)a[i]=i;
    for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
    for(int l=1;l<=n;l++)
        for(int r=l;r<=n;r++)w[l][r]=(s[r]-s[l-1])*(s[r]-s[l-1]);
    dnc_partition(n,K,w,fd,opt);
    brute_partition(n,K,w,bk);
    printf("n=5 [1..5] 分 2 段 : 分治 %lld 暴力 %lld (期望 45 45)\n",fd[2][5],bk[2][5]);

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=300;tt++)
    {
        n=rndint(1,40),K=rndint(1,min(n,5));
        for(int i=1;i<=n;i++)a[i]=rndint(0,8);
        for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
        for(int l=1;l<=n;l++)
            for(int r=l;r<=n;r++)w[l][r]=(s[r]-s[l-1])*(s[r]-s[l-1]);
        brute_partition(n,K,w,bk);
        dnc_partition(n,K,w,fd,opt);
        int wa=0;
        for(int k=1;k<=K;k++)
            for(int i=k;i<=n;i++)
                if(fd[k][i]!=bk[k][i])wa=1;
        if(wa){bad++;printf("WA! 分治写法 轮%d n=%d K=%d\n",tt,n,K);break;}
        for(int k=1;k<=K;k++)
            for(int i=k+1;i<=n;i++)
                if(opt[k][i]<opt[k][i-1]){bad++;wa=1;printf("WA! 决策点不单调 轮%d k=%d i=%d\n",tt,k,i);break;}
        if(wa)break;
    }
    // 单调队列写法：单层 dp f[2][i]=min_{j<i} f[1][j]+w(j+1,i)
    for(tt=1;tt<=300&&!bad;tt++)
    {
        n=rndint(2,30);
        for(int i=1;i<=n;i++)a[i]=rndint(0,8);
        for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
        for(int l=1;l<=n;l++)
            for(int r=l;r<=n;r++)w[l][r]=(s[r]-s[l-1])*(s[r]-s[l-1]);
        for(int i=1;i<=n;i++)fd[1][i]=w[1][i];
        mq_layer(n,w);
        for(int i=2;i<=n;i++)
        {
            ll ref=INF;
            for(int j=1;j<i;j++)ref=min(ref,fd[1][j]+w[j+1][i]);
            if(fd[2][i]!=ref){bad++;printf("WA! 单调队列写法 轮%d i=%d ref=%lld cur=%lld\n",tt,i,ref,fd[2][i]);break;}
        }
    }
    // 反例演示：不满足 QI 的代价表会让分治优化出错
    ll ww[M][N];
    int found=0,nn=0;
    for(tt=1;tt<=3000&&!found;tt++)
    {
        nn=rndint(6,12);
        for(int l=1;l<=nn;l++)
            for(int r=l;r<=nn;r++)ww[l][r]=rndint(0,20);
        brute_partition(nn,2,ww,bk);
        dnc_partition(nn,2,ww,fd,opt);
        for(int i=2;i<=nn;i++)
            if(fd[2][i]!=bk[2][i]){found=1;break;}
    }
    if(found)
    {
        printf("==== 反例：代价表不满足 QI 时，分治优化会算错 ====\n");
        for(int l=1;l<=nn;l++)
        {
            printf("w[%d][*] =",l);
            for(int r=l;r<=nn;r++)printf(" %lld",ww[l][r]);
            printf("\n");
        }
        printf("分治 f[2][%d]=%lld，暴力 f[2][%d]=%lld -> 用之前必须先验证 QI\n",
            nn,fd[2][nn],nn,bk[2][nn]);
    }
    if(!bad)printf("stress OK (分治写法 300 轮 + 单调队列写法 300 轮 全部通过)\n");
    return 0;
}
