// 扩展欧几里得 的测试与对拍代码
// 模板本体：05-数学/扩展欧几里得.cpp
#include "../../05-数学/扩展欧几里得.cpp"

int main()
{
    int bad=0;
    mt19937_64 rnd(20250101);
    // 1) exgcd 解代回验证：a*x+b*y == gcd(a,b)，且 gcd 与 __gcd 一致
    for(int i=1;i<=5000;i++)
    {
        ll a=rnd()%1000000000+1,b=rnd()%1000000000+1;
        ll x,y;
        ll g=exgcd(a,b,x,y);
        if(__gcd(a,b)!=g)bad++;
        if((__int128)a*x+(__int128)b*y!=g)bad++;
    }
    // 2) 有负数的情况；注意 a,b 都负时 exgcd 返回 -gcd，等式仍成立
    for(int i=1;i<=3000;i++)
    {
        ll a=(ll)(rnd()%1000000)-500000,b=(ll)(rnd()%1000000)-500000;
        ll x,y;
        ll g=exgcd(a,b,x,y);
        if(abs(g)!=__gcd(abs(a),abs(b)))bad++;
        if((__int128)a*x+(__int128)b*y!=g)bad++;
    }
    // 3) 同余方程：暴力枚举 [0,m) 对拍
    for(int i=1;i<=3000;i++)
    {
        ll a=rnd()%50,b=rnd()%50,m=rnd()%50+1;
        ll got=solve_cong(a,b,m);
        ll want=-1;
        for(ll x=0;x<m;x++)
            if((a*x-b)%m==0){want=x;break;}
        if(got!=want)bad++;
    }
    // 4) 逆元：暴力枚举对拍
    for(int i=1;i<=3000;i++)
    {
        ll p=rnd()%1000+2,a=rnd()%(p-1)+1;
        if(__gcd(a,p)!=1)continue;
        ll iv=inv_exgcd(a,p);
        if(iv<0||iv>=p)bad++;
        if((__int128)a*iv%p!=1)bad++;
    }
    ll d1,d2;
    printf("exgcd(12,8) : %lld, 12*1+8*(-1)=4\n",exgcd(12,8,d1,d2));
    printf("solve_cong(3,1,5) = %lld (3x=1 mod 5 -> x=2)\n",solve_cong(3,1,5));
    printf("solve_cong(4,1,6) = %lld (no solution)\n",solve_cong(4,1,6));
    printf("solve_cong(14,8,30) = %lld\n",solve_cong(14,8,30));
    printf("inv_exgcd(3,11) = %lld\n",inv_exgcd(3,11));
    printf("bezout_min(12,18) = %lld\n",bezout_min(12,18));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P1082 输入 3 11 -> 4；输入 2 6 -> 无解（本题保证有解）
// 样例：P4549 裴蜀定理，n 个数的线性组合最小正值为所有数的 gcd
// 边界：a 或 b 为 0；解要求最小非负，最后统一 (x%t+t)%t

/*
自测记录：
  1) 随机正数 / 含负数，代回验证 a*x+b*y=gcd 且 gcd 与 __gcd 一致；
  2) 同余方程 ax=b (mod m) 与 [0,m) 暴力枚举对拍（含无解情形）；
  3) 逆元与乘法验证 + 暴力枚举对拍。
*/
