// 折半搜索 的测试与对拍代码
// 模板本体：07-搜索/折半搜索.cpp
#include "../../07-搜索/折半搜索.cpp"

ll brute(const vector<ll> &a,ll target)
{
    ll ans=0;
    int n=a.size();
    for(int mask=0;mask<(1<<n);mask++)
    {
        ll sum=0;
        for(int i=0;i<n;i++)if(mask>>i&1)sum+=a[i];
        ans+=sum==target;
    }
    return ans;
}

int main()
{
    srand(19260817);
    ::a={1,2,3};bool ok=solve(3)==2;
    ::a.clear();ok=ok&&solve(0)==1;
    for(int t=1;t<=40;t++)
    {
        vector<ll> a(rand()%16);
        for(ll &x:a)x=rand()%21-10;
        ll target=rand()%41-20;
        ::a=a;
        if(solve(target)!=brute(a,target))ok=false;
    }
    printf("折半搜索 %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
