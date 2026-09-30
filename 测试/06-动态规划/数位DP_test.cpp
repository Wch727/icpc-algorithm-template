// 数位DP 的测试与对拍代码
// 模板本体：06-动态规划/数位DP.cpp
#include "../../06-动态规划/数位DP.cpp"

// 暴力：数字 d 在 1..x 里出现几次（0 不算任何数字）

int brute_cnt(ll x,int d)
{
    int s=0;
    for(ll i=1;i<=x;i++)
    {
        ll t=i;
        while(t>0)
        {
            if(t%10==d)s++;
            t/=10;
        }
    }
    return s;
}

// 暴力：区间内不含 4 的数的个数
int brute_no4(ll l,ll r)
{
    int s=0;
    for(ll i=l;i<=r;i++)
    {
        ll t=i;
        int ok=1;
        while(t>0)
        {
            if(t%10==4)ok=0;
            t/=10;
        }
        s+=ok;
    }
    return s;
}

// 暴力：区间内相邻数字差 >= 2 的数的个数
int brute_windy(ll l,ll r)
{
    int s=0;
    for(ll i=l;i<=r;i++)
    {
        ll t=i;
        int ok=1,last=-1;
        while(t>0)
        {
            int c=t%10;
            if(last>=0&&abs(c-last)<2)ok=0;
            last=c,t/=10;
        }
        s+=ok;
    }
    return s;
}

// 暴力：区间内数位和 <= K 的数的个数
int brute_sumk(ll l,ll r,int k)
{
    int s=0;
    for(ll i=l;i<=r;i++)
    {
        ll t=i;
        int sum=0;
        while(t>0)sum+=t%10,t/=10;
        if(sum<=k)s++;
    }
    return s;
}

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

int main()
{
    srand(20240609);
    printf("==== 固定样例 ====\n");
    printf("1..11 中 1 出现次数 : %lld (期望 4)\n",solve_cnt(11,1));
    printf("1..100 中不含 4 的个数 : %lld  暴力 %d\n",solve_no4(100)-solve_no4(0),brute_no4(1,100));
    printf("1..100 中 windy 数个数 : %lld  暴力 %d\n",solve_windy(100)-solve_windy(0),brute_windy(1,100));
    printf("1..100 中数位和<=5个数 : %lld  暴力 %d\n",solve_sumk(100,5)-solve_sumk(0,5),brute_sumk(1,100,5));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=300;tt++)
    {
        ll x=rndint(0,3000);
        int dd=rndint(0,9);
        if(solve_cnt(x,dd)!=brute_cnt(x,dd))
        {
            bad++,printf("WA! 数字次数 轮%d x=%lld d=%d dp=%lld brute=%d\n",tt,x,dd,solve_cnt(x,dd),brute_cnt(x,dd));
            break;
        }
        ll l=rndint(1,3000),r=rndint((int)l,3000);
        ll dp1=solve_no4(r)-solve_no4(l-1);
        if(dp1!=brute_no4(l,r)){bad++,printf("WA! 不含4 轮%d l=%lld r=%lld dp=%lld brute=%d\n",tt,l,r,dp1,brute_no4(l,r));break;}
        ll dp2=solve_windy(r)-solve_windy(l-1);
        if(dp2!=brute_windy(l,r)){bad++,printf("WA! windy 轮%d l=%lld r=%lld dp=%lld brute=%d\n",tt,l,r,dp2,brute_windy(l,r));break;}
        int k=rndint(1,25);
        ll dp3=solve_sumk(r,k)-solve_sumk(l-1,k);
        if(dp3!=brute_sumk(l,r,k)){bad++,printf("WA! 数位和 轮%d l=%lld r=%lld k=%d dp=%lld brute=%d\n",tt,l,r,k,dp3,brute_sumk(l,r,k));break;}
    }
    if(!bad)printf("stress OK (300 轮，数字出现次数/不含4/windy/数位和 全部通过)\n");
    printf("大范围抽查：1..1e18 中 windy 数个数 = %lld\n",solve_windy(1000000000000000000LL)-solve_windy(0));
    return 0;
}
