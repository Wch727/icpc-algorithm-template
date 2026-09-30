#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=105;

const double EPS=1e-8;
int n;
double a[N][N];// a[i][1..n] 系数，a[i][n+1] 常数项

// 返回值：0 唯一解（解在 a[i][n+1]），1 无穷多解，-1 无解
// O(n^3)，列主元高斯-约当消元
int gauss(int n)
{
    int r=1;// 当前主元行
    for(int c=1;c<=n;c++)
    {
        int pivot=r;
        for(int i=r+1;i<=n;i++)
            if(fabs(a[i][c])>fabs(a[pivot][c]))pivot=i;
        if(fabs(a[pivot][c])<EPS)continue;// 这一列全 0，跳过
        for(int j=1;j<=n+1;j++)swap(a[r][j],a[pivot][j]);
        double div=a[r][c];
        for(int j=1;j<=n+1;j++)a[r][j]/=div;
        for(int i=1;i<=n;i++)
        {
            if(i==r)continue;
            double mul=a[i][c];
            if(fabs(mul)<EPS)continue;
            for(int j=1;j<=n+1;j++)a[i][j]-=mul*a[r][j];
        }
        r++;
    }
    if(r<=n)
    {
        for(int i=r;i<=n;i++)
            if(fabs(a[i][n+1])>EPS)return -1;// 0=非0，无解
        return 1;// 有效方程数小于未知数个数，无穷多解
    }
    return 0;
}

// O(n^2)，把得到的解代回原方程组，返回最大残差；用于自测
double check(double b[N][N],double x[N],int n)
{
    double worst=0;
    for(int i=1;i<=n;i++)
    {
        double s=0;
        for(int j=1;j<=n;j++)s+=b[i][j]*x[j];
        worst=max(worst,fabs(s-b[i][n+1]));
    }
    return worst;
}

int main()
{
    int bad=0;
    mt19937_64 rnd(20250404);
    // 1) 随机唯一解：构造已知解 x，回代验证残差
    for(int t=1;t<=200;t++)
    {
        int m=rnd()%5+1;
        double b[N][N],x[N],want[N];
        n=m;
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n+1;j++)b[i][j]=0;
        for(int i=1;i<=n;i++)
        {
            want[i]=(int)(rnd()%21)-10;
            for(int j=1;j<=n;j++)b[i][j]=(int)(rnd()%11)-5;
        }
        for(int i=1;i<=n;i++)
        {
            double s=0;
            for(int j=1;j<=n;j++)s+=b[i][j]*want[j];
            b[i][n+1]=s;
        }
        memcpy(a,b,sizeof(b));
        int st=gauss(n);
        if(st==0)
        {
            for(int i=1;i<=n;i++)x[i]=a[i][n+1];
            if(check(b,x,n)>1e-6)bad++;
        }
        // 行列式为 0 时可能判成无穷多解，这里不强制要求
    }
    // 2) 无解 / 无穷多解 的手造样例
    // x+y=1, x+y=2 -> 无解
    n=2;
    a[1][1]=1,a[1][2]=1,a[1][3]=1;
    a[2][1]=1,a[2][2]=1,a[2][3]=2;
    int s1=gauss(n);
    if(s1!=-1)bad++;
    // x+y=1, 2x+2y=2 -> 无穷多解
    a[1][1]=1,a[1][2]=1,a[1][3]=1;
    a[2][1]=2,a[2][2]=2,a[2][3]=2;
    int s2=gauss(n);
    if(s2!=1)bad++;
    // 唯一解：x+y=3, x-y=1 -> x=2,y=1
    a[1][1]=1,a[1][2]=1,a[1][3]=3;
    a[2][1]=1,a[2][2]=-1,a[2][3]=1;
    int s3=gauss(n);
    double x2=a[1][3],y2=a[2][3];
    if(s3!=0)bad++;
    if(fabs(x2-2)>1e-6||fabs(y2-1)>1e-6)bad++;
    printf("x=%.4f y=%.4f (expect 2 1)\n",x2,y2);
    printf("no-solution case status = %d (expect -1)\n",s1);
    printf("infinite case status   = %d (expect 1)\n",s2);
    printf("unique case status     = %d (expect 0)\n",s3);
    printf("brute force mismatch = %d\n",bad);
    printf("%s\n",bad==0?"PASS":"FAIL");
    return 0;
}
// 样例：P3389 输入 1 3 4 5 -> 0.33；无唯一解输出 No Solution
// 样例：P2455 -1 无解，0 无穷多解，1 唯一解
// 边界：主元绝对值 < EPS 视为 0；秩小于 n 时用常数项判无解

/*
自测记录：
  1) 200 组随机方程组（阶数 1..5）构造已知整数解，消元后代回验证残差 < 1e-6；
  2) 手造无解 / 无穷多解 / 唯一解三例，判定结果与解值均校验。
*/
