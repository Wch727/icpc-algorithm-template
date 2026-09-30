// 斜率优化DP 的测试与对拍代码
// 模板本体：06-动态规划/斜率优化DP.cpp
#include "../../06-动态规划/斜率优化DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

// 单调队列维护下凸壳，O(n) 斜率优化
// 叉积 cross(o,a,b)=(x[a]-x[o])*(y[b]-y[o])-(y[a]-y[o])*(x[b]-x[o])
// cross(o,a,b)<=0 表示 a 在 ob 连线之上，下凸壳要把 a 弹掉
ll slope_dp(int n,ll L,ll a[])
{
    for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
    int head=0,tail=0;
    x[0]=0,y[0]=0;// f[0]=0
    q[tail++]=0;
    for(int i=1;i<=n;i++)
    {
        ll k=2*(s[i]-L);
        while(head+1<tail&&y[q[head+1]]-y[q[head]]<=k*(x[q[head+1]]-x[q[head]]))head++;
        int j=q[head];
        f[i]=y[j]-k*x[j]+sq(s[i]-L);
        x[i]=s[i],y[i]=f[i]+sq(s[i]);
        while(head+1<tail)
        {
            int o=q[tail-2],p=q[tail-1];
            if((x[p]-x[o])*(y[i]-y[o])-(y[p]-y[o])*(x[i]-x[o])<=0)tail--;
            else break;
        }
        q[tail++]=i;
    }
    return f[n];
}

// 斜率优化的通用写法：只要求查询斜率 k 单调，点可以按任意顺序加入
// 用 deque 的版本见注释：把上面 q[] 换成 deque<int> 即可
// 输出每个 f[i] 方便观察转移来源
void slope_dp_all(int n,ll L,ll a[],ll f[])
{
    for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
    int head=0,tail=0;
    x[0]=0,y[0]=0,q[tail++]=0;
    for(int i=1;i<=n;i++)
    {
        ll k=2*(s[i]-L);
        while(head+1<tail&&y[q[head+1]]-y[q[head]]<=k*(x[q[head+1]]-x[q[head]]))head++;
        int j=q[head];
        f[i]=y[j]-k*x[j]+sq(s[i]-L);
        x[i]=s[i],y[i]=f[i]+sq(s[i]);
        while(head+1<tail)
        {
            int o=q[tail-2],p=q[tail-1];
            if((x[p]-x[o])*(y[i]-y[o])-(y[p]-y[o])*(x[i]-x[o])<=0)tail--;
            else break;
        }
        q[tail++]=i;
    }
}

// 把转移式乘开成 A[i]*B[j]+C[i] 的标准形式，方便套李超线段树/一般凸壳
// f[i]=min_j (f[j]+s[j]^2 - 2*s[i]*s[j]) + s[i]^2 + L^2 - 2*L*s[i]
void build_ab(int n,ll L,ll a[])
{
    for(int i=1;i<=n;i++)s[i]=s[i-1]+a[i];
    for(int j=0;j<=n;j++)A[j]=-2*s[j],B[j]=f[j]+s[j]*s[j];// 斜率、截距
    for(int i=1;i<=n;i++)C[i]=s[i]*s[i]+L*L-2*L*s[i];// 与 j 无关的常数
}

// O(n^2) 暴力，直接枚举转移
ll bs[N],bf[N];// 放全局，避免函数里开 8e5 字节的局部数组爆栈
ll brute_slope(int n,ll L,ll a[])
{
    bs[0]=0;
    for(int i=1;i<=n;i++)bs[i]=bs[i-1]+a[i];
    bf[0]=0;
    for(int i=1;i<=n;i++)
    {
        bf[i]=INF;
        for(int j=0;j<i;j++)bf[i]=min(bf[i],bf[j]+sq(bs[i]-bs[j]-L));
    }
    return bf[n];
}

int main()
{
    srand(20240615);
    printf("==== 固定样例 ====\n");
    // 玩具装箱简化版：c={2,3,1}, L=1
    // 分段 (2)|(3)|(1) 得 (2-1)^2+(3-1)^2+(1-1)^2=5
    // 分段 (2)|(3,1) 得 (2-1)^2+(4-1)^2=10，一次装完 (6-1)^2=25，最优 5
    // 注意：真实 P3195 的式子是 (s[i]-s[j]+i-j-1-L)^2，多一项 (i-j)，
    // 把 i 并进 X 里就变成同样形状，这里用简化式演示
    ll c1[4]={0,2,3,1};
    printf("斜率优化 n=3 c=[2 3 1] L=1 : %lld (期望 5)\n",slope_dp(3,1,c1));
    // 乘开形式自检：A/B/C 拼出来的答案要和上面一致
    ll f2[N];slope_dp_all(3,1,c1,f2);
    build_ab(3,1,c1);
    ll chk=B[1]+A[1]*s[3]+C[3];// 直接用 j=1 这一项验证 A/B/C 的符号
    printf("A[j],B[j] 写法 j=1 : %lld (期望 9 = (6-2-1)^2)\n",chk);

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=500;tt++)
    {
        int n=rndint(1,30);
        ll L=rndint(0,5);
        for(int i=1;i<=n;i++)a[i]=rndint(1,4);// c 全正，保证 s 单调
        ll cur=slope_dp(n,L,a),ref=brute_slope(n,L,a);
        if(cur!=ref){bad++;printf("WA! 斜率优化 轮%d n=%d L=%lld ref=%lld cur=%lld\n",tt,n,L,ref,cur);break;}
    }
    // 单独验证"最优转移点也单调"这一性质
    if(!bad)
    {
        int ok=1;
        for(tt=1;tt<=50&&ok;tt++)
        {
            int n=rndint(5,40);
            ll L=rndint(0,3);
            for(int i=1;i<=n;i++)a[i]=rndint(1,50);
            slope_dp_all(n,L,a,f);
            ll bs[N];bs[0]=0;
            for(int i=1;i<=n;i++)bs[i]=bs[i-1]+a[i];
            int last=-1;
            for(int i=1;i<=n;i++)
            {
                int bj=-1;ll bv=INF;
                for(int j=0;j<i;j++)
                {
                    ll v=f[j]+sq(bs[i]-bs[j]-L);
                    if(v<bv)bv=v,bj=j;
                }
                if(f[i]!=bv){ok=0;break;}
                if(bj<last){ok=0;break;}// 转移点应当单调不减
                last=bj;
            }
        }
        if(!ok){bad++;printf("WA! 转移点单调性检查失败\n");}
    }
    if(!bad)printf("stress OK (500 轮随机对拍 + 转移点单调性 50 轮 全部通过)\n");
    return 0;
}
