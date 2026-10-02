// 状压DP 的测试与对拍代码
// 模板本体：06-动态规划/状压DP.cpp
#include "../../06-动态规划/状压DP.cpp"

int rndint(int l,int r)// 生成 [l,r] 的随机整数
{
    return l+rand()%(r-l+1);
}

int brute_art(int row,int p1,int p2)
{
    if(row>n)return 0;
    int best=0;
    for(int x=0;x<(1<<m);x++)
    {
        if(x&(x<<1))continue;// 同行左右互攻
        if(x&(x<<2))continue;// 同行隔一个互攻
        if(x&mp[row])continue;// 不能放山地
        if(x&p1)continue;// 与上一行同列
        if(x&p2)continue;// 与上上行同列
        best=max(best,brute_art(row+1,x,p1)+popcnt(x));
    }
    return best;
}

// 暴力：枚举所有访问顺序
double brute_tsp(int n)
{
    int p[20];
    for(int i=1;i<=n;i++)p[i]=i;
    double ans=1e18;
    do
    {
        double cur=0;
        int last=0;
        for(int i=1;i<=n;i++)cur+=dist(last,p[i]),last=p[i];
        ans=min(ans,cur);
    }while(next_permutation(p+1,p+n+1));
    return ans;
}

int main()
{
    srand(20240606);
    printf("==== 固定样例 ====\n");
    n=2,m=3,mp[1]=0,mp[2]=0;// 两行全是平地，每行最多 1 门，共 2
    printf("炮兵阵地 2x3 全平地 : %d (期望 2)\n",artillery(n,m));
    n=5,m=4;
    const char *g[6]={"","PHPP","PPHH","PPPP","PHPP","PHHP"};
    for(int i=1;i<=n;i++)
    {
        mp[i]=0;
        for(int j=0;j<m;j++)
            if(g[i][j]=='H')mp[i]|=(1<<(m-1-j));// H 记成 1
    }
    printf("炮兵阵地 P2704 样例 5x4 : %d  暴力 %d (期望 6)\n",artillery(n,m),brute_art(1,0,0));
    n=4;
    px[1]=1,py[1]=1,px[2]=1,py[2]=-1,px[3]=-1,py[3]=1,px[4]=-1,py[4]=-1;
    printf("吃奶酪 4 个点 : %.2f (期望 7.41)\n",tsp(n));

    printf("==== 随机对拍 ====\n");
    int tt,bad=0;
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,3),m=rndint(1,3);
        for(int i=1;i<=n;i++)
        {
            mp[i]=0;
            for(int j=0;j<m;j++)
                if(rndint(0,1))mp[i]|=(1<<j);// 随机山地
        }
        int ref=brute_art(1,0,0),cur=artillery(n,m);
        if(ref!=cur){bad++;printf("WA! 炮兵 轮%d n=%d m=%d ref=%d cur=%d\n",tt,n,m,ref,cur);break;}
    }
    for(tt=1;tt<=200;tt++)
    {
        n=rndint(1,7);
        for(int i=1;i<=n;i++)px[i]=rndint(-5,5),py[i]=rndint(-5,5);
        double ref=brute_tsp(n),cur=tsp(n);
        if(fabs(ref-cur)>1e-9){bad++;printf("WA! 吃奶酪 轮%d n=%d ref=%.6f cur=%.6f\n",tt,n,ref,cur);break;}
    }
    if(!bad)printf("stress OK (炮兵 200 轮 + 吃奶酪 200 轮 全部通过)\n");
    return 0;
}
