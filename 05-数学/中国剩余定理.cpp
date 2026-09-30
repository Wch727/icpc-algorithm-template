#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// O(log)，扩展欧几里得
ll exgcd(ll a,ll b,ll &x,ll &y)
{
    if(!b){x=1,y=0;return a;}
    ll xx,yy;
    ll g=exgcd(b,a%b,xx,yy);
    x=yy,y=xx-(a/b)*yy;
    return g;
}

// 模数两两互质版 CRT：x === a[i] (mod m[i])，返回最小非负解
// 用前缀积 p[i]=m[1..i]、后缀积 s[i]=m[i..n] 把 M/m[i] 控制在 1e18 内
// 总模数乘积应 <= 1e18，否则解可能溢出
ll crt(int n,ll a[],ll m[])
{
    static ll p[N],s[N];
    p[0]=1;
    for(int i=1;i<=n;i++)p[i]=p[i-1]*m[i];
    s[n+1]=1;
    for(int i=n;i>=1;i--)s[i]=s[i+1]*m[i];
    ll M=p[n],ans=0;
    for(int i=1;i<=n;i++)
    {
        ll Mi=p[i-1]*s[i+1];// M / m[i]
        ll x,y;
        exgcd(Mi%m[i],m[i],x,y);// Mi 在模 m[i] 下的逆元
        x=(x%m[i]+m[i])%m[i];
        ans=(ans+(__int128)a[i]*Mi%M*x)%M;
    }
    return (ans%M+M)%M;
}

// 扩展 CRT（模数不必互质）：合并 x === a1 (mod m1) 与 x === a2 (mod m2)
// 返回是否可合并；可合并时 a1 为新余数、m1 为新模数，均取最小非负
// O(log)
bool crt_merge(ll &a1,ll &m1,ll a2,ll m2)
{
    ll x,y;
    ll g=exgcd(m1,m2,x,y);
    ll d=a2-a1;
    if(d%g)return false;// 无解
    ll t=m2/g;
    x=(ll)((__int128)(d/g)%t*x%t);
    x=(x%t+t)%t;
    a1=a1+(__int128)m1*x;
    m1=m1/g*m2;// lcm
    a1=(a1%m1+m1)%m1;
    return true;
}

// O(n log)，扩展 CRT 数组版，无解返回 -1（要求 lcm 不超过 9e18）
ll crt_ex(int n,ll a[],ll m[])
{
    ll ra=a[1],rm=m[1];
    for(int i=2;i<=n;i++)
        if(!crt_merge(ra,rm,a[i],m[i]))return -1;
    return (ra%rm+rm)%rm;
}

int main()
{
    int bad=0;
    // 1) 互质版：与 [0,lcm) 暴力枚举对拍
    int ms1[4]={2,3,5,7};
    mt19937_64 rnd(20250505);
    for(int t=1;t<=2000;t++)
    {
        int k=rnd()%4+1;
        ll a[6],m[6];
        ll L=1;
        for(int i=1;i<=k;i++)
        {
            m[i]=ms1[i-1];
            a[i]=rnd()%m[i];
            L=L/__gcd(L,m[i])*m[i];
        }
        ll got=crt(k,a,m);
        ll want=-1;
        for(ll x=0;x<L;x++)
        {
            bool ok=true;
            for(int i=1;i<=k;i++)
                if(x%m[i]!=a[i]){ok=false;break;}
            if(ok){want=x;break;}
        }
        if(got!=want)bad++;
        if(crt_ex(k,a,m)!=want)bad++;
    }
    // 2) 非互质版：模数含公因子，暴力对拍
    int ms2[6]={4,6,9,10,14,15};
    for(int t=1;t<=3000;t++)
    {
        int k=rnd()%6+1;
        ll a[8],m[8];
        ll L=1;
        for(int i=1;i<=k;i++)
        {
            m[i]=ms2[i-1];
            a[i]=rnd()%m[i];
            L=L/__gcd(L,m[i])*m[i];
        }
        ll want=-1;
        for(ll x=0;x<L;x++)
        {
            bool ok=true;
            for(int i=1;i<=k;i++)
                if(x%m[i]!=a[i]){ok=false;break;}
            if(ok){want=x;break;}
        }
        ll got=crt_ex(k,a,m);
        if(got!=want)bad++;
    }
    // 3) 无解情形：需要 lcm 很大时无法暴力，但小模数可暴力，已在上面覆盖
    // 单独验证一个必然无解的：x=1 mod 4, x=2 mod 6
    ll aa[3]={0,1,2},mm[3]={0,4,6};
    if(crt_ex(2,aa,mm)!=-1)bad++;
    // 手算样例：x=2 mod 3, x=3 mod 5, x=2 mod 7 -> 23
    ll A3[4]={0,2,3,2},B3[4]={0,3,5,7};
    if(crt(3,A3,B3)!=23)bad++;
    // 4) 大模数：余数必须先对模数取模再比对（a[i] >= m[i] 时算法内部会自己取模）
    // 两个大模数走互质版，三个 1e9 级模数走扩展版
    ll A[4]={0,123456789,987654321};
    ll B[4]={0,1000000007,999999937};
    for(int i=1;i<=2;i++)A[i]%=B[i];
    ll x=crt(2,A,B);
    for(int i=1;i<=2;i++)
        if(x%B[i]!=A[i])bad++;
    ll C3[4]={0,123456789,987654321,555555555};
    ll D3[4]={0,1000003,1000033,1000037};
    for(int i=1;i<=3;i++)C3[i]%=D3[i];
    ll x3=crt_ex(3,C3,D3);
    if(x3<0)bad++;
    else
        for(int i=1;i<=3;i++)
            if(x3%D3[i]!=C3[i])bad++;
    printf("crt(2 mod 3, 3 mod 5, 2 mod 7) = %lld (expect 23)\n",crt(3,A3,B3));
    printf("crt_ex(1 mod 4, 2 mod 6) = %lld (expect -1)\n",crt_ex(2,aa,mm));
    printf("crt 2 big moduli -> %lld\n",x);
    printf("crt_ex 3 big moduli -> %lld\n",x3);
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1082 求 ax=1 mod b 的最小正解，可看作单项 CRT
// 边界：模数不互质时先判 d%g；无解返回 -1；结果取最小非负
// 注意：互质版内部用前缀积，要求所有模数之积不超过 long long

/*
自测记录：
  1) 互质模数（2,3,5,7 子集）2000 组，与 [0,lcm) 暴力枚举对拍，互质版与扩展版都验；
  2) 非互质模数（4,6,9,10,14,15 子集）3000 组，与暴力对拍；
  3) 无解情形；两个大模数走互质版，三个 1e9 级模数走扩展版，都逐个代回验证。
*/
