// 斜率优化DP 的测试与对拍代码
// 模板本体：06-动态规划/斜率优化DP.cpp
#include "../../06-动态规划/斜率优化DP.cpp"

void load_case(int len,ll gap,const ll *aa)
{
    ::n=len;::L=gap;for(int i=1;i<=len;i++)::a[i]=aa[i];
}

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

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
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        ll v[]={0,0,0,2,0,3};
        assert((load_case(5,2,v),slope_dp())==brute_slope(5,2,v));
        (load_case(5,2,v),build_ab());
        for(int i=1;i<=5;i++)for(int j=0;j<i;j++)
            assert((__int128)A[j]*s[i]+B[j]+C[i]==(__int128)f[j]+(s[i]-s[j]-2)*(s[i]-s[j]-2));
    }

    srand(20240615);
    printf("==== 固定样例 ====\n");
    // 玩具装箱简化版：c={2,3,1}, L=1
    // 分段 (2)|(3)|(1) 得 (2-1)^2+(3-1)^2+(1-1)^2=5
    // 分段 (2)|(3,1) 得 (2-1)^2+(4-1)^2=10，一次装完 (6-1)^2=25，最优 5
    // 注意：真实 P3195 的式子是 (s[i]-s[j]+i-j-1-L)^2，多一项 (i-j)，
    // 把 i 并进 X 里就变成同样形状，这里用简化式演示
    ll c1[4]={0,2,3,1};
    printf("斜率优化 n=3 c=[2 3 1] L=1 : %lld (期望 5)\n",(load_case(3,1,c1),slope_dp()));
    // 乘开形式自检：A/B/C 拼出来的答案要和上面一致
    load_case(3,1,c1);slope_dp();
    (load_case(3,1,c1),build_ab());
    ll chk=B[1]+A[1]*s[3]+C[3];// 直接用 j=1 这一项验证 A/B/C 的符号
    assert(chk==10); // f[1]=1，再加 (6-2-1)^2=9
    printf("A[j],B[j] 写法 j=1 : %lld (期望 10)\n",chk);

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=500;tt++)
    {
        int n=rndint(1,30);
        ll L=rndint(0,5);
        for(int i=1;i<=n;i++)a[i]=rndint(1,4);// c 全正，保证 s 单调
        ll cur=(load_case(n,L,a),slope_dp()),ref=brute_slope(n,L,a);
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
            load_case(n,L,a);slope_dp();
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
