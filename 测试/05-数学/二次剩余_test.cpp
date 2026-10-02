// 二次剩余 的测试与对拍代码
// 模板本体：05-数学/二次剩余.cpp
#include "../../05-数学/二次剩余.cpp"

ll cipolla_naive(ll a,ll p)
{
    a=(a%p<0?a%p+p:a%p);
    for(ll x=0;x<p;x++)
        if((lll)x*x%p==a)return x;
    return -1;
}

int count_qr_naive(ll p)
{
    int c=0;
    for(ll x=1;x<p;x++)
    {
        bool ok=false;
        for(ll y=0;y<p;y++)
            if((lll)y*y%p==x%p){ok=true;break;}
        if(ok)c++;
    }
    return c;
}

int main()
{
    int bad=0;
    // 1) p=3..300 全枚举：有解性与暴力一致，无解时都返回 -1
    for(ll p=3;p<=300;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        for(ll a=0;a<p;a++)
        {
            ll x=cipolla(a,p);
            ll y=cipolla_naive(a,p);
            // 原期望写错：模板返回任意一根，暴力返回最小根
            if((x==-1)!=(y==-1))bad++;
            if(x!=-1&&(lll)x*x%p!=a)bad++;// 平方代回
        }
    }
    // 2) 勒让德符号与暴力判定对拍
    for(ll p=3;p<=200;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        for(ll a=0;a<p;a++)
        {
            ll x=cipolla_naive(a,p);
            int want=(a==0)?0:(x==-1?-1:1);
            if(legendre(a,p)!=want)bad++;
        }
    }
    // 3) 大素数：随机 a 与随机平方数，验证代回
    ll bigp[4]={1000000007LL,998244353LL,1000000009LL,19260817LL};
    mt19937_64 rnd(20251010);
    for(int t=0;t<4;t++)
    {
        ll p=bigp[t];
        for(int k=1;k<=50;k++)
        {
            ll a=rnd()%(p-1)+1;
            ll x=cipolla(a,p);
            if(x!=-1&&(lll)x*x%p!=a)bad++;
            if(x!=-1&&legendre(a,p)!=1)bad++;
            // 构造一定是二次剩余的 b=y^2，必须求出解
            ll y=rnd()%p;
            ll b=(lll)y*y%p;
            ll z=cipolla(b,p);
            if(z==-1||(lll)z*z%p!=b)bad++;
        }
        // p≡1 (mod 4) 与 p≡3 (mod 4) 都要覆盖
        if(p%4==1)printf("p = %lld 满足 p%%4==1，走 Cipolla 随机分支\n",p);
    }
    // 4) 特例：1 的两解是 1 和 p-1；-1 在 p≡1 (mod 4) 时是二次剩余
    for(int t=0;t<4;t++)
    {
        ll p=bigp[t];
        ll r=cipolla(1,p);
        if(r!=1&&r!=p-1)bad++;
        int lg=legendre(p-1,p);
        if(lg!=(p%4==1?1:-1))bad++;
    }
    // 5) 二次剩余个数应为 (p-1)/2
    for(ll p=3;p<=60;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        if(count_qr_naive(p)!=(p-1)/2)bad++;
    }
    printf("cipolla(4,7)  = %lld (expect 2)\n",cipolla(4,7));
    printf("cipolla(3,7)  = %lld (expect -1, 无解)\n",cipolla(3,7));
    ll r2=cipolla(2,7),r5=cipolla(5,1000000007LL);
    printf("cipolla(2,7)  = %lld, 平方 = %lld\n",r2,(ll)((lll)r2*r2%7));
    printf("cipolla(5,1000000007) = %lld, 平方 = %lld\n",r5,(ll)((lll)r5*r5%1000000007LL));
    printf("legendre(5,1000000007) = %d (原期望写错，5 是非二次剩余)\n",legendre(5,1000000007LL));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P5491 输入多组 a p，输出两解（最小在前）或 Hola!
// 样例：Cipolla 模板题给 4 7 -> 2 5
// 边界：a=0 返回 0；p=2 直接返回 a；p≡3 (mod 4) 用 (p+1)/4 次幂；无解返回 -1

/*
自测记录：
  1) p<=300 的全部奇素数、a 取遍 0..p-1，Cipolla 有解性与暴力一致且平方代回；
  2) 勒让德符号与暴力判定全表对拍；
  3) 4 个大素数（含 p%4==1 与 p%4==3）随机 a 与随机平方数验证；
  4) 1 与 -1 的经典结论、二次剩余个数 = (p-1)/2 校验。
*/
