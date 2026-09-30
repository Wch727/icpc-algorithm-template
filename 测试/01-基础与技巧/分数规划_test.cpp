// 分数规划 的测试与对拍代码
// 模板本体：01-基础与技巧/分数规划.cpp
#include "../../01-基础与技巧/分数规划.cpp"

// 选恰好 k 项最大化 sum(a)/sum(b)，b>0；判定取最大的 k 个 a-mid*b
// O(迭代次数*n log n)，b 全为 1 就是最大平均值

bool check(const vector<double> &a,const vector<double> &b,int k,double mid)
{
    vector<double> c(a.size());
    for(int i=0;i<(int)a.size();i++)c[i]=a[i]-mid*b[i];
    sort(c.begin(),c.end(),greater<double>());
    double sum=0;
    for(int i=0;i<k;i++)sum+=c[i];
    return sum>=0;
}

double solve(const vector<double> &a,const vector<double> &b,int k)
{
    assert(a.size()==b.size()&&k>=1&&k<=(int)a.size());
    double l=1e100,r=-1e100;
    for(int i=0;i<(int)a.size();i++)
    {
        assert(b[i]>0);
        l=min(l,a[i]/b[i]),r=max(r,a[i]/b[i]);
    }
    for(int t=1;t<=80;t++)
    {
        double mid=(l+r)/2;
        if(check(a,b,k,mid))l=mid;
        else r=mid;
    }
    return l;
}

double brute(const vector<double> &a,const vector<double> &b,int k)
{
    double ans=-1e100;
    for(int mask=0;mask<(1<<(int)a.size());mask++)if(__builtin_popcount((unsigned)mask)==k)
    {
        double x=0,y=0;
        for(int i=0;i<(int)a.size();i++)if(mask>>i&1)x+=a[i],y+=b[i];
        ans=max(ans,x/y);
    }
    return ans;
}

int main()
{
    srand(19260817);
    bool ok=fabs(solve({1,2,3},{1,1,1},2)-2.5)<1e-8;
    for(int t=1;t<=100;t++)
    {
        int n=rand()%10+1,k=rand()%n+1;
        vector<double> a(n),b(n);
        for(int i=0;i<n;i++)a[i]=rand()%81-40,b[i]=rand()%15+1;
        if(fabs(solve(a,b,k)-brute(a,b,k))>1e-8)ok=false;
    }
    printf("分数规划 %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
