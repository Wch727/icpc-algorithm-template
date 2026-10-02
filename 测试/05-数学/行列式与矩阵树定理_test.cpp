// 行列式与矩阵树定理 的测试与对拍代码
// 模板本体：05-数学/行列式与矩阵树定理.cpp
#include "../../05-数学/行列式与矩阵树定理.cpp"

ll det_naive(ll a[N][N],int n)
{
    if(n==1)return a[1][1];
    ll res=0;
    ll b[N][N];
    for(int j=1;j<=n;j++)
    {
        for(int i=2;i<=n;i++)
        {
            int c=0;
            for(int k=1;k<=n;k++)
            {
                if(k==j)continue;
                b[i-1][++c]=a[i][k];// 划掉第 1 行第 j 列
            }
        }
        ll sub=det_naive(b,n-1);
        if(j&1)res+=a[1][j]*sub;
        else res-=a[1][j]*sub;
    }
    return res;
}

ll tree_naive(int n,ll g[N][N],int m,int eu[],int ev[])
{
    ll res=0;
    for(int mask=0;mask<(1<<m);mask++)
    {
        if(__builtin_popcount(mask)!=n-1)continue;// 生成树恰好 n-1 条边
        int fa[N];
        for(int i=1;i<=n;i++)fa[i]=i;
        function<int(int)> findd=[&](int x){return fa[x]==x?x:fa[x]=findd(fa[x]);};
        bool ok=true;
        for(int i=1;i<=m;i++)
            if(mask>>(i-1)&1)
            {
                int a=findd(eu[i]),b=findd(ev[i]);
                if(a==b){ok=false;break;}// 出现环
                fa[a]=b;
            }
        if(ok)res++;
    }
    return res;
}

ll det_naive_mod(ll a[N][N],int n,ll p)
{
    ll v=det_naive(a,n);
    return ((v%p)+p)%p;
}

int main()
{
    int bad=0;
    // 1) 行列式：与按第一行展开的 O(n!) 暴力对拍（含奇异矩阵）
    mt19937_64 rnd(20250707);
    ll a[N][N],b[N][N];
    for(int t=1;t<=300;t++)
    {
        int s=rnd()%5+1;
        for(int i=1;i<=s;i++)
            for(int j=1;j<=s;j++)
            {
                a[i][j]=rnd()%7;// 0..6，整数系数
                b[i][j]=a[i][j];
            }
        ll d1=det_mod(a,s,mod);
        ll d2=det_naive_mod(b,s,mod);
        if(d1!=d2)bad++;
    }
    // 2) 含负数的矩阵
    for(int t=1;t<=200;t++)
    {
        int s=rnd()%4+1;
        for(int i=1;i<=s;i++)
            for(int j=1;j<=s;j++)
            {
                a[i][j]=(ll)(rnd()%11)-5;
                b[i][j]=a[i][j];
            }
        if(det_mod(a,s,mod)!=det_naive_mod(b,s,mod))bad++;
    }
    // 3) 奇异矩阵必须返回 0：两行相同 / 成比例
    n=4;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)a[i][j]=rnd()%100;
    for(int j=1;j<=n;j++)a[4][j]=a[2][j];// 第 4 行 = 第 2 行
    if(det_mod(a,n,mod)!=0)bad++;
    for(int j=1;j<=n;j++)a[4][j]=3*a[2][j]%mod;
    if(det_mod(a,n,mod)!=0)bad++;
    // 4) 单位矩阵行列式为 1
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)a[i][j]=(i==j);
    if(det_mod(a,n,mod)!=1)bad++;
    // 5) 矩阵树定理与暴力枚举生成树对拍
    for(int t=1;t<=300;t++)
    {
        int s=rnd()%5+2;// 2..6 个点
        static ll g[N][N];
        int eu[E],ev[E],mm=0;
        for(int i=1;i<=s;i++)
            for(int j=1;j<=s;j++)g[i][j]=0;
        for(int i=1;i<=s;i++)
            for(int j=i+1;j<=s;j++)
                if(rnd()%3)// 约 2/3 概率有边
                {
                    g[i][j]=g[j][i]=1;
                    eu[++mm]=i,ev[mm]=j;
                }
        ll x=matrix_tree(s,g);
        ll y=tree_naive(s,g,mm,eu,ev);
        if(x!=y)bad++;
    }
    // 6) 经典值：完全图 K_n 的生成树个数 = n^(n-2)
    for(int s=2;s<=8;s++)
    {
        static ll g[N][N];
        for(int i=1;i<=s;i++)
            for(int j=1;j<=s;j++)g[i][j]=(i!=j);
        if(matrix_tree(s,g)!=qpow(s,s-2,mod))bad++;
    }
    // 7) 链（n-1 条边）生成树个数为 1，环为 n
    for(int s=3;s<=8;s++)
    {
        static ll g[N][N];
        for(int i=1;i<=s;i++)
            for(int j=1;j<=s;j++)g[i][j]=0;
        for(int i=1;i<s;i++)g[i][i+1]=g[i+1][i]=1;
        if(matrix_tree(s,g)!=1)bad++;
        g[1][s]=g[s][1]=1;// 首尾相连成环
        if(matrix_tree(s,g)!=s)bad++;
    }
    // 8) 重边（边权为 2）也要正确：两点之间 2 条重边 -> 2 棵生成树
    {
        static ll g[N][N];
        for(int i=1;i<=2;i++)
            for(int j=1;j<=2;j++)g[i][j]=0;
        g[1][2]=g[2][1]=2;
        if(matrix_tree(2,g)!=2)bad++;
    }
    {
        static ll c[N][N];
        c[1][1]=1,c[1][2]=2,c[2][1]=3,c[2][2]=4;
        printf("det [[1,2],[3,4]] = %lld (expect %lld)\n",det_mod(c,2,mod),mod-2);
    }
    {
        static ll g[N][N];
        for(int i=1;i<=4;i++)
            for(int j=1;j<=4;j++)g[i][j]=(i!=j);
        printf("K4 spanning trees = %lld (expect 16)\n",matrix_tree(4,g));
    }
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P6178 / P3317 用矩阵树定理统计生成树
// 样例：P4783 输入 i+j 型矩阵验证行列式（本题要求 mod 素数）
// 边界：n=1 时矩阵树返回 1；行列式有一列全 0 返回 0；交换行时答案取 mod-res 保证符号

/*
自测记录：
  1) 500 组小矩阵（含负数）行列式与按第一行展开的暴力展开对拍；
  2) 两行相同 / 成比例 / 单位矩阵三类特例；
  3) 300 组随机无向图（2..6 点）生成树个数与 2^m 枚举对拍；
  4) 完全图 K_n 与 n^(n-2)、链、环、重边的经典结果校验。
*/
