#include<bits/stdc++.h>
using namespace std;
namespace ternary {
#include "../../01-基础与技巧/三分法.cpp"
double f(double x){return (x-2.5)*(x-2.5);}
long long left_opt,right_opt;
long long g(long long x){return x<left_opt?left_opt-x:(x>right_opt?x-right_opt:0);}
}
namespace mono {
#include "../../01-基础与技巧/单调栈.cpp"
}
namespace inversion {
#include "../../01-基础与技巧/逆序对.cpp"
}
namespace greedy {
#include "../../01-基础与技巧/反悔贪心.cpp"
ll job_schedule(vector<Job> input){jobs=move(input);return job_schedule();}
}
namespace ski {
#include "../../01-基础与技巧/记忆化搜索.cpp"
}
namespace fractional_test {
#include "../../01-基础与技巧/分数规划.cpp"
}
namespace io {
#include "../../01-基础与技巧/快读快写.cpp"
}

int main()
{
    mt19937 rng(20261001);
    for(int t=0;t<300;t++)
    {
        int n=rng()%15+1;
        vector<int> a(n);
        for(int &x:a)x=(int)(rng()%21)-10;
        mono::n=n;
        for(int i=1;i<=n;i++)mono::a[i]=inversion::a[i]=a[i-1];
        mono::right_greater();
        for(int i=1;i<=n;i++)
        {
            int want=0;
            for(int j=i+1;j<=n;j++)if(a[j-1]>a[i-1]){want=j;break;}
            assert(mono::ans[i]==want);
        }
        mono::left_less();
        for(int i=1;i<=n;i++)
        {
            int want=0;
            for(int j=i-1;j>=1;j--)if(a[j-1]<a[i-1]){want=j;break;}
            assert(mono::ans[i]==want);
        }
        long long inv=0;
        long long min_sum=0,area=0;
        for(int l=0;l<n;l++)
        {
            int low=INT_MAX,height=INT_MAX;
            for(int r=l;r<n;r++)
            {
                low=min(low,a[r]);
                height=min(height,abs(a[r]));
                min_sum+=low;
                area=max(area,1LL*height*(r-l+1));
            }
        }
        assert(mono::sum_min()==min_sum);
        for(int i=1;i<=n;i++)mono::a[i]=abs(a[i-1]);
        assert(mono::histogram()==area);
        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++)inv+=a[i]>a[j];
        }
        assert(inversion::count_inv(1,n)==inv);
        assert(is_sorted(inversion::a+1,inversion::a+n+1));
        // 枚举所有选取集合，独立检查截止约束和分数规划。
        int m=rng()%8+1;
        vector<greedy::Job> jobs;
        fractional_test::n=m,fractional_test::k=rng()%m+1;
        for(int i=0;i<m;i++)
        {
            jobs.push_back({(int)(rng()%(m+1)),(int)(rng()%20)});
            fractional_test::a[i+1]=(int)(rng()%31)-10,fractional_test::b[i+1]=rng()%10+1;
        }
        long long profit=0;
        double frac=-1e100;
        for(int mask=0;mask<(1<<m);mask++)
        {
            vector<int> deadlines;
            long long value=0;
            double sa=0,sb=0;
            for(int i=0;i<m;i++)if(mask>>i&1)
            {
                deadlines.push_back(jobs[i].d),value+=jobs[i].p;
                sa+=fractional_test::a[i+1],sb+=fractional_test::b[i+1];
            }
            sort(deadlines.begin(),deadlines.end());
            bool valid=true;
            for(int i=0;i<(int)deadlines.size();i++)if(deadlines[i]<i+1)valid=false;
            if(valid)profit=max(profit,value);
            if(__builtin_popcount((unsigned)mask)==fractional_test::k)frac=max(frac,sa/sb);
        }
        assert(greedy::job_schedule(jobs)==profit);
        assert(abs(fractional_test::fractional()-frac)<1e-8);
    }
    assert(abs(ternary::ternary_min(-20,30)-2.5)<1e-7);
    assert(abs(ternary::ternary_min(5,30)-5)<1e-7);
    assert(abs(ternary::ternary_min(-20,1)-1)<1e-7);
    for(int t=0;t<1000;t++)
    {
        long long l=(int)(rng()%101)-50,r=l+rng()%101;
        ternary::left_opt=(int)(rng()%201)-100;
        ternary::right_opt=ternary::left_opt+rng()%20;
        long long pos=ternary::ternary_int(l,r),best=LLONG_MAX;
        for(long long i=l;i<=r;i++)best=min(best,ternary::g(i));
        assert(pos>=l&&pos<=r&&ternary::g(pos)==best);
    }
    // 记忆化与按高度排序的迭代 DP 对拍。
    for(int t=0;t<100;t++)
    {
        ski::n=ski::m=4;
        vector<array<int,3> > cells;
        int dp[5][5]={0};
        for(int i=1;i<=4;i++)for(int j=1;j<=4;j++)
        {
            ski::h[i][j]=rng()%20,ski::f[i][j]=0;
            cells.push_back({ski::h[i][j],i,j});
        }
        sort(cells.begin(),cells.end());
        for(array<int,3> c:cells)
        {
            int x=c[1],y=c[2];
            dp[x][y]=1;
            for(int d=0;d<4;d++)
            {
                int nx=x+ski::dx[d],ny=y+ski::dy[d];
                if(nx>=1&&nx<=4&&ny>=1&&ny<=4&&ski::h[nx][ny]<c[0])dp[x][y]=max(dp[x][y],dp[nx][ny]+1);
            }
            assert(ski::dfs(x,y)==dp[x][y]);
        }
    }
    // 标准输入重定向，验证符号及 ll 极值。
    FILE *input=fopen("bin/part1_io_input.txt","w");
    assert(input);
    fputs("0 +7 -8 -9223372036854775808 9223372036854775807",input);
    fclose(input);
    assert(freopen("bin/part1_io_input.txt","r",stdin));
    for(long long x:{0LL,7LL,-8LL,LLONG_MIN,LLONG_MAX})assert(io::readd()==x);
    fclose(stdin);
    remove("bin/part1_io_input.txt");
    puts("精简模板随机对拍与边界：OK");
}
