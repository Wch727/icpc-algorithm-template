// 两阶段单纯形：max c*x，约束 A*x<=b 且 x>=0；实数 LP，不保证整数解。
// Solve 返回 -INF 无解、+INF 无界，否则最优值并输出 x；重复调用须重新构建对象。
// 实践中较快，但最坏指数级；EPS、缩放和病态数据影响精度。long double 存表。
// 算法整理自 Stanford ACM / KACTL 两阶段单纯形；B/N 是基/非基变量编号。
#include<bits/stdc++.h>
using namespace std;
struct LPSolver
{
    using Real=long double;static constexpr Real EPS=1e-10L;
    int m,n;vector<int>B,N;vector<vector<Real>> D;
    LPSolver(const vector<vector<Real>>&A,const vector<Real>&b,const vector<Real>&c):m(b.size()),n(c.size()),B(m),N(n+1),D(m+2,vector<Real>(n+2))
    {
        for(int i= 0; i < m; i++)
        {
            for(int j= 0; j < n; j++)
                D[i][j]= A[i][j];
            B[i]= n + i;
            D[i][n]= -1;
            D[i][n + 1]= b[i];
        }
        for(int j= 0; j < n; j++)
            N[j]= j, D[m][j]= -c[j];
        N[n]= -1;
        D[m + 1][n]= 1;
    }
    void pivot(int r,int s)
    {
        Real inv= 1 / D[r][s];
        for(int i= 0; i < m + 2; i++)
            if(i != r)
                for(int j= 0; j < n + 2; j++)
                    if(j != s)
                        D[i][j]-= D[r][j] * D[i][s] * inv;
        for(int j= 0; j < n + 2; j++)
            if(j != s)
                D[r][j]*= inv;
        for(int i= 0; i < m + 2; i++)
            if(i != r)
                D[i][s]*= -inv;
        D[r][s]= inv;
        swap(B[r], N[s]);
    }
    bool simplex(int phase)
    {
        int row=phase==1?m+1:m;
        while(true)
        {
            int s= -1;
            for(int j= 0; j <= n; j++)
                if(!(phase == 2 && N[j] == -1))
                    if(s < 0 || D[row][j] < D[row][s] - EPS ||
                       (fabsl(D[row][j] - D[row][s]) <= EPS && N[j] < N[s]))
                        s= j;
            if(s<0||D[row][s]>=-EPS)return true;int r=-1;
            for(int i= 0; i < m; i++)
                if(D[i][s] > EPS)
                {
                    if(r < 0)
                        r= i;
                    else
                    {
                        Real a= D[i][n + 1] / D[i][s], b= D[r][n + 1] / D[r][s];
                        if(a < b - EPS || (fabsl(a - b) <= EPS && B[i] < B[r]))
                            r= i;
                    }
                }
            if(r<0)return false;pivot(r,s);
        }
    }
    Real Solve(vector<Real> &x)
    {
        const Real INF=numeric_limits<Real>::infinity();x.assign(n,0);
        if(!n)
        {
            for(int i= 0; i < m; i++)
                if(D[i][n + 1] < -EPS)
                    return -INF;
            return 0;
        }
        if(m)
        {
            int r= 0;
            for(int i= 1; i < m; i++)
                if(D[i][n + 1] < D[r][n + 1])
                    r= i;
            if(D[r][n + 1] < -EPS)
            {
                pivot(r, n);
                if(!simplex(1) || fabsl(D[m + 1][n + 1]) > EPS)
                    return -INF;
                for(int i= 0; i < m; i++)
                    if(B[i] == -1)
                    {
                        int s= -1;
                        for(int j= 0; j <= n; j++)
                            if(fabsl(D[i][j]) > EPS && (s < 0 || N[j] < N[s]))
                                s= j;
                        if(s >= 0)
                            pivot(i, s);
                    }
            }
        }
        if(!simplex(2))
            return INF;
        for(int i= 0; i < m; i++)
            if(0 <= B[i] && B[i] < n)
                x[B[i]]= D[i][n + 1];
        return D[m][n + 1];
    }
};
