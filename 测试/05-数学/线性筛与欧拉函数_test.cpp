// 线性筛与欧拉函数 的测试与对拍代码
// 模板本体：05-数学/线性筛与欧拉函数.cpp
#include "../../05-数学/线性筛与欧拉函数.cpp"

int main()
{
    int M=30000;
    get_prime(M);
    int bad=0,pcnt=0;
    for(int i=1;i<=M;i++)
    {
        if(!vis[i]&&i>1)pcnt++;
        if(phi[i]!=phi_naive(i))bad++;
        if(mu[i]!=mu_naive(i))bad++;
        if(d[i]!=d_naive(i))bad++;
    }
    if(pcnt!=cnt)bad++;
    // 素数表本身查一遍
    for(int i=2;i<=M;i++)
    {
        bool p=true;
        for(int j=2;(ll)j*j<=i;j++)
            if(i%j==0){p=false;break;}
        if(p!=!vis[i])bad++;
    }
    // 再筛满一遍，用 1e6 的边界值验证
    get_prime(1000000);
    if(phi[1000000]!=phi_naive(1000000))bad++;
    if(d[1000000]!=d_naive(1000000))bad++;
    if(mu[1000000]!=mu_naive(1000000))bad++;
    if(phi[999983]!=999982)bad++;// 999983 是素数
    printf("prime count <= %d : %d\n",M,cnt);
    printf("phi(1..8) = ");
    for(int i=1;i<=8;i++)printf("%d ",phi[i]);
    printf("\nmu(1..8) = ");
    for(int i=1;i<=8;i++)printf("%d ",mu[i]);
    printf("\nd(1..8)  = ");
    for(int i=1;i<=8;i++)printf("%d ",d[i]);
    printf("\nphi(1e6) = %d, d(1e6) = %d, mu(1e6) = %d\n",phi[1000000],d[1000000],mu[1000000]);
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P3383 输入 100 5 后跟 1 2 3 4 5 -> 2 3 5 7 11
// 样例：P1865 输入 2 5 -> 1 3 得 2, 2 5 得 3
// 边界：phi[1]=mu[1]=d[1]=1；n<2 时筛出的素数表为空

/*
自测记录：
  1) 素数表与 O(sqrt n) 试除逐一比对；
  2) phi / mu / d 三个数组在 1..30000 上与 O(sqrt n) 暴力对拍。
*/
