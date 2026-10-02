#include "../../05-数学/整除商上的质数计数.cpp"
#include "../../05-数学/SternBrocot有理数二分.cpp"
int main()
{
    const int N=200000;
    vector<int>pi(N+1),composite(N+1);
    for(int i=2;i<=N;i++)
    {
        if(!composite[i])for(int j=i+i;j<=N;j+=i)composite[j]=1;
        pi[i]=pi[i-1]+!composite[i];
    }
    for(int n=0;n<=500;n++)
    {
        PrimeCount pc(n);assert(pc.count(0)==0);
        for(ll x:pc.w)assert(pc.count(x)==pi[x]);
    }
    mt19937 rng(123456);
    for(int z=0;z<300;z++)
    {
        int n=1+rng()%N;PrimeCount pc(n);
        for(ll x:pc.w)assert(pc.count(x)==pi[x]);
    }
    assert(PrimeCount(100000000).count(100000000)==5761455);
    using F=pair<ll,ll>;
    auto less=[](F a,F b){return (__int128)a.first*b.second<(__int128)b.first*a.second;};
    for(ll P=0;P<=12;P++)for(ll Q=1;Q<=12;Q++)for(ll A=0;A<=16;A++)for(ll B=1;B<=12;B++)
    {
        auto check=[&](ll p,ll q){return (__int128)p*B<=(__int128)A*q;};
        F lo={0,1},hi={1,0};
        for(ll p=0;p<=P;p++)for(ll q=1;q<=Q;q++)if(gcd(p,q)==1)
        {
            F f={p,q};if(check(p,q)){if(less(lo,f))lo=f;}else if(less(f,hi))hi=f;
        }
        auto got=fraction_bounds(P,Q,check);assert(got[0]==lo&&got[1]==hi);
    }
    ll lim=1000000000000000000LL;int calls=0;
    auto close=fraction_bounds(lim,lim,[&](ll p,ll q){calls++;return (__int128)p*lim<=(__int128)(lim-1)*q;});
    assert(close[0]==F(lim-1,lim)&&close[1]==F(1,1)&&calls<1000);
    cout<<"PASS: exact prime counts, fraction boundaries and long chains\n";
}
