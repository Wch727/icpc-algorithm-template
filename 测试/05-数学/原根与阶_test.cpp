// 原根与阶 的测试与对拍代码
// 模板本体：05-数学/原根与阶.cpp
#include "../../05-数学/原根与阶.cpp"

ll order_naive(ll a,ll p)
{
    if(__gcd(a,p)!=1)return -1;
    ll cur=1%p;
    for(ll k=1;k<=p;k++)
    {
        cur=cur*a%p;
        if(cur==1)return k;
    }
    return -1;
}

bool is_root_naive(ll g,ll p)
{
    if(__gcd(g,p)!=1)return false;
    static int seen[N];
    for(int i=1;i<p;i++)seen[i]=0;
    ll cur=1%p;
    for(int k=1;k<p;k++)
    {
        cur=cur*g%p;
        if(seen[cur])return false;
        seen[cur]=1;
    }
    return true;
}

bool is_root_naive_general(ll g,ll n)
{
    if(n==1)return true;
    if(__gcd(g,n)!=1)return false;
    ll lim=phi_of(n);
    static int seen[N];
    for(int i=0;i<n;i++)seen[i]=0;
    ll cur=1%n;
    for(ll k=1;k<=lim;k++)
    {
        cur=cur*g%n;
        if(seen[cur])return false;
        seen[cur]=1;
    }
    return true;
}

int main()
{
    get_prime(1000000);
    int bad=0;
    // 1) 1..300 范围内所有互素 (a,p) 的阶与暴力对拍（p 含合数，考 get_order 的 phi(p) 分支）
    for(ll p=2;p<=300;p++)
        for(ll a=1;a<p;a++)
        {
            if(__gcd(a,p)!=1)continue;
            if(get_order(a,p)!=order_naive(a,p))bad++;
        }
    // 2) 1..200 的素数：原根判定与暴力对拍，并与最小原根一致
    for(ll p=2;p<=200;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(!isp)continue;
        ll g=get_root(p);
        if(!is_root(g,p))bad++;
        if(!is_root_naive(g,p))bad++;
        if(get_order(g,p)!=p-1)bad++;// 素数模数下原根的阶就是 p-1
        for(ll x=1;x<p;x++)
        {
            if(is_root(x,p)!=is_root_naive(x,p))bad++;
            if(is_root(x,p)&&x<g)bad++;// g 必须是最小的那个
        }
    }
    // 3) 大素数：验算 g^((p-1)/q)!=1（q 取 p-1 的每个素因子）且 g^(p-1)=1
    ll bigp[4]={1000000007LL,998244353LL,1000000009LL,19260817LL};
    for(int t=0;t<4;t++)
    {
        ll p=bigp[t];
        ll g=get_root(p);
        if(qpow(g,p-1,p)!=1)bad++;
        vector<ll> f=factor(p-1);
        for(int i=0;i<(int)f.size();i++)
            if(qpow(g,(p-1)/f[i],p)==1)bad++;
        if(get_order(g,p)!=p-1)bad++;
    }
    // 4) 任意模数：阶整除 phi(p)，且 a^ord=1、ord 是最小的（试除 ord 的真因子都不为 1）
    mt19937_64 rnd(20250606);
    for(int t=1;t<=500;t++)
    {
        ll p=rnd()%100000+2;
        ll a=rnd()%(p-1)+1;
        if(__gcd(a,p)!=1)continue;
        ll ord=get_order(a,p);
        ll ph=phi_of(p);
        if(ph%ord!=0)bad++;// 阶一定整除 phi(p)
        if(qpow(a,ord,p)!=1)bad++;
        if(qpow(a,ph,p)!=1)bad++;// 欧拉定理
    }
    // 5) 合数模数的原根：只有 2,4,p^k,2p^k 存在，暴力搜最小原根对拍
    //    注意 8=2^3 与 16=2^4 也不存在原根（2^k(k>=3) 的乘法群不是循环群）
    ll gen[23]={2,4,8,9,16,25,27,32,49,18,50,12,15,3,5,7,11,13,81,98,121,169,242};
    for(int t=0;t<23;t++)
    {
        ll n=gen[t];
        ll g=get_root_general(n);
        ll want=-1;
        for(ll x=1;x<n;x++)
            if(is_root_naive_general(x,n)){want=x;break;}
        if(g!=want)bad++;
        if(g!=-1&&get_order(g,n)!=phi_of(n))bad++;
    }
    printf("order(2,7) = %lld (expect 3)\n",get_order(2,7));
    printf("order(3,8) = %lld (expect 2)\n",get_order(3,8));
    printf("root(2..11) = ");
    for(ll p=2;p<=11;p++)
    {
        bool isp=true;
        for(ll i=2;i*i<=p;i++)
            if(p%i==0){isp=false;break;}
        if(isp)printf("%lld ",get_root(p));
    }
    printf("\nroot(1e9+7) = %lld, root(998244353) = %lld\n",get_root(1000000007LL),get_root(998244353LL));
    printf("is_root(3,7) = %d, is_root(2,7) = %d\n",(int)is_root(3,7),(int)is_root(2,7));
    printf("get_root_general(8) = %lld, (9) = %lld, (18) = %lld, (12) = %lld (expect -1)\n",
           get_root_general(8),get_root_general(9),get_root_general(18),get_root_general(12));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P6091 输入 6 -> 原根 5，以及 5^k 的次幂表
// 样例：P3321 用最小原根把乘法转成加法
// 边界：p=2 的原根是 1；gcd(a,p)>1 时阶不存在返回 -1；get_root 只对素数模数用，
//       合数模数要用 get_root_general（只有 2,4,p^k,2p^k 存在原根，其余返回 -1）

/*
自测记录：
  1) p<=300 的全部互素 (a,p) 求阶与 O(p) 暴力对拍（p 含合数，考 phi(p) 分支）；
  2) p<=200 的全部素数：原根判定与暴力「幂两两不同」对拍，且 get_root 给的是最小原根；
  3) 4 个大素数验算 g^(p-1)=1 且 g^((p-1)/q)!=1（q 取 p-1 的每个素因子）；
  4) 500 组随机 (a,p) 验证 ord | phi(p)、a^ord=1、a^phi(p)=1；
  5) 23 个模数（2,4,p^k,2p^k 与 8,16,12,15 这类无原根的）与暴力搜最小原根对拍。
*/
