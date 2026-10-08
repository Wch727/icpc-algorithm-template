#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N=105;
int n,m,k;
ll d[N][N][N]; // 1-indexed；各维预留 0 和右端点+1，按题目调整大小

// 非零初始值直接填 d，再建差分；全零起始清 d 后跳过 build。
// 建差分沿每一维倒序做相邻相减，避免覆盖尚未使用的原值。
void build()
{
    for(int x= n; x >= 1; x--)
        for(int y= 1; y <= m; y++)
            for(int z= 1; z <= k; z++)
                d[x][y][z]-= d[x - 1][y][z];
    for(int x= 1; x <= n; x++)
        for(int y= m; y >= 1; y--)
            for(int z= 1; z <= k; z++)
                d[x][y][z]-= d[x][y - 1][z];
    for(int x= 1; x <= n; x++)
        for(int y= 1; y <= m; y++)
            for(int z= k; z >= 1; z--)
                d[x][y][z]-= d[x][y][z - 1];
}

// 闭长方体 [x1,x2] × [y1,y2] × [z1,z2] 加 v，修改八个角点。
// 每选一个右端点+1，符号翻转一次：0/2 个取正，1/3 个取负。
void add(int x1,int y1,int z1,int x2,int y2,int z2,ll v)
{
    ++x2,++y2,++z2;
    d[x1][y1][z1]+=v;
    d[x2][y1][z1]-=v; d[x1][y2][z1]-=v; d[x1][y1][z2]-=v;
    d[x2][y2][z1]+=v; d[x2][y1][z2]+=v; d[x1][y2][z2]+=v;
    d[x2][y2][z2]-=v;
}

// 修改结束后，沿每一维正序做前缀和；结果仍在 d，只还原一次。
void restore()
{
    for(int x= 1; x <= n; x++)
        for(int y= 1; y <= m; y++)
            for(int z= 1; z <= k; z++)
                d[x][y][z]+= d[x - 1][y][z];
    for(int x= 1; x <= n; x++)
        for(int y= 1; y <= m; y++)
            for(int z= 1; z <= k; z++)
                d[x][y][z]+= d[x][y - 1][z];
    for(int x= 1; x <= n; x++)
        for(int y= 1; y <= m; y++)
            for(int z= 1; z <= k; z++)
                d[x][y][z]+= d[x][y][z - 1];
}
// 三维建表/还原 O(nmk)，修改 O(1)；D 维建表/还原 O(D*S)，S 为格数。
// 空间随各维长度相乘；这里静态数组约 8.8 MiB，须保证 n+1,m+1,k+1<N。

// D 维统一公式：x=(x1,...,xD)，ei 为第 i 维单位向量，b∈{0,1}^D。
// |b| 为 b 中 1 的个数，x-b 表示对应坐标减 1；任一坐标为 0 时值为 0。
// 建差分：d(x)=Σ_b (-1)^|b| * a(x-b)。
// 直接还原：a(x)=d(x)+Σ_{b≠0} (-1)^(|b|+1) * a(x-b)，各维正序。
// 即减一维的项相加，减两维的项相减，减三维的项相加……容斥交替。
// 更省计算的逐维递推（上面的三维代码就是此式）：
//   建表 G0(x)=a(x)，Gi(x)=G(i-1)(x)-G(i-1)(x-ei)，GD(x)=d(x)。
//   还原 F0(x)=d(x)，Fi(x)=F(i-1)(x)+Fi(x-ei)，FD(x)=a(x)。
// 原地执行时：建表沿当前维倒序，保留上一阶段的邻格；还原沿当前维正序。
// 区域 [l1,r1]×...×[lD,rD] 加 v：枚举 b，pi=(bi?ri+1:li)，
//   d(p)+=(-1)^|b|*v；共有 2^D 个角点，奇数个右端点减，偶数个加。
// 逐维建表/还原 O(D*S)，直接容斥还原 O(2^D*S)；S 为格数，先估算内存。
