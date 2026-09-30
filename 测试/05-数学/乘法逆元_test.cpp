// 乘法逆元 的测试与对拍代码
// 模板本体：05-数学/乘法逆元.cpp
#include "../../05-数学/乘法逆元.cpp"

int main()
{
    int bad=0;
    ll P=1000000007;
    mt19937_64 rnd(20250202);
    // 1) 费马 / exgcd / 线性递推三种逆元互相对拍 + 乘法验证
    int M=200000;
    inv_init(M,P);
    for(int i=1;i<=M;i++)
    {
        if((ll)i*inv[i]%P!=1)bad++;
        if(i<=3000)// 前 3000 个三种方法都验一遍
        {
            if(inv_fermat(i,P)!=inv[i])bad++;
            if(inv_exgcd(i,P)!=inv[i])bad++;
        }
    }
    // 2) 随机数验证三种方法
    for(int i=1;i<=3000;i++)
    {
        ll a=rnd()%(P-1)+1;
        ll x1=inv_fermat(a,P),x2=inv_exgcd(a,P);
        if(x1!=x2)bad++;
        if((__int128)a*x1%P!=1)bad++;
    }
    // 3) 组合数对拍杨辉三角
    fact_init(2000,P);
    static ll c[2005][2005];
    for(int i=0;i<=2000;i++)
    {
        c[i][0]=1;
        for(int j=1;j<=i;j++)
            c[i][j]=(c[i-1][j-1]+(j<=i-1?c[i-1][j]:0))%P;
    }
    for(int i=0;i<=2000;i++)
        for(int j=0;j<=i;j++)
            if(C_small(i,j,P)!=c[i][j])bad++;
    // 4) 非素数模数下的 exgcd 逆元（gcd=1 才有逆元）
    for(int i=1;i<=1000;i++)
    {
        ll m=rnd()%1000+2,a=rnd()%(m-1)+1;
        if(__gcd(a,m)!=1)continue;
        ll iv=inv_exgcd(a,m);
        if((__int128)a*iv%m!=1)bad++;
    }
    printf("inv[1..8] mod 1e9+7 : ");
    for(int i=1;i<=8;i++)printf("%lld ",inv[i]);
    printf("\ninv_fermat(2)=%lld inv_exgcd(2)=%lld\n",inv_fermat(2,P),inv_exgcd(2,P));
    printf("C(10,3) via fact = %lld, pascal = %lld\n",C_small(10,3,P),c[10][3]);
    printf("C(2000,1000) = %lld\n",C_small(2000,1000,P));
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P3811 输入 10 13 -> 1 7 9 10 8 11 2 5 3 4
// 边界：inv[1]=1；线性推逆元要求 n<p；p 非素数时只能用 exgcd

/*
自测记录：
  1) 1..200000 线性递推逆元逐个乘法验证，前 3000 个三种方法互拍；
  2) 随机大数与费马/exgcd 互拍；
  3) 组合数与杨辉三角 0..2000 全表对拍；
  4) 随机非素数模数下 exgcd 逆元验证。
*/
