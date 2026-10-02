// 中国剩余定理 的测试与对拍代码
// 模板本体：05-数学/中国剩余定理.cpp
#include "../../05-数学/中国剩余定理.cpp"

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        ll a1=-1,m1=LLONG_MAX-24;
        assert(crt_merge(a1,m1,-1,m1)&&a1==m1-1);
    }

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
