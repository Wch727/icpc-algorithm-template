// KM算法(最大权匹配) 的测试与对拍代码
// 模板本体：04-图论/KM算法(最大权匹配).cpp
#include "../../04-图论/KM算法(最大权匹配).cpp"

ll brute(int nn)// n! 枚举所有完美匹配求最大权
{
    int p[10];
    for(int i=1;i<=nn;i++)p[i]=i;
    ll best=-INF;
    do
    {
        ll s=0;
        for(int i=1;i<=nn;i++)s+=w[i][p[i]];
        if(s>best)best=s;
    }while(next_permutation(p+1,p+nn+1));
    return best;
}

int main()
{
    // 自测 1：手造 3*3，最大权完美匹配取 w12+w23+w31 = 5+6+7 = 18
    n=3;
    ll a[4][4]={{0,0,0,0},{0,3,5,1},{0,2,4,6},{0,7,1,4}};
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)w[i][j]=a[i][j];
    printf("手造3*3 最大权=%lld 暴力=%lld（期望 18 18）\n",km(),brute(n));
    // 自测 2：全负权 2*2，也必须选完（-5 + -2 = -7）
    n=2;
    w[1][1]=-5,w[1][2]=-9,w[2][1]=-8,w[2][2]=-2;
    printf("全负权 最大权=%lld 暴力=%lld（期望 -7 -7）\n",km(),brute(n));
    // 自测 3：随机方阵（含负权），与 n! 枚举对拍
    for(int T=1;T<=200;T++)
    {
        n=rand()%6+1;// 1..6
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)w[i][j]=rand()%16-5;
        ll got=km(),want=brute(n);
        if(got!=want)
        {
            printf("WA T=%d n=%d got=%lld want=%lld\n",T,n,got,want);
            return 0;
        }
    }
    printf("KM 与 n! 枚举 200 组通过\n");
    return 0;
}
/* 顶标含义：任意匹配的权值和 <= sum(lx)+sum(ly)，相等子图里的完美匹配就是最优解
   每次增广失败时取 d=min slack，把左顶标减小、右顶标加大，相当于把边「放进来」
   slack 优化让每次调整只需 O(n) 找 d，总计 O(n^3)
   权值改成 -w 就是最小权匹配；n 很大且只要最大匹配（不要求最大权）时用匈牙利更快 */
