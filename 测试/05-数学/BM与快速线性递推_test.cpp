#include "../../05-数学/BM与快速线性递推.cpp"
int main()
{
    auto residue=[](ll x){return (x%MOD+MOD)%MOD;};
    assert(berlekamp_massey({}).empty()&&berlekamp_massey(vector<ll>(30)).empty());
    assert(linear_nth({}, {},ULLONG_MAX)==0);
    assert(linear_nth({-2},{1},ULLONG_MAX)==MOD-2);
    mt19937 g(202);
    for(int k=1;k<=12;k++)for(int z=0;z<80;z++)
    {
        vector<ll> c(k),a(160);
        for(int i=0;i<k;i++)c[i]=int(g()%21)-10,a[i]=residue(int(g()%21)-10);
        for(int i=k;i<(int)a.size();i++)
            for(int j=0;j<k;j++)a[i]=(a[i]+residue(c[j])*a[i-j-1])%MOD;
        vector<ll> init(a.begin(),a.begin()+k),prefix(a.begin(),a.begin()+2*k);
        auto b=berlekamp_massey(prefix);
        assert(b.size()<=(unsigned)k);
        for(int i=0;i<(int)a.size();i++)
        {
            assert(linear_nth(init,c,i)==a[i]);
            assert(linear_nth(prefix,b,i)==a[i]);
        }
    }
    vector<ll> f={0,1};
    auto fib=[&](unsigned long long n)
    {
        array<ll,4> a={1,0,0,1},b={1,1,1,0};
        auto mul=[](array<ll,4> a,array<ll,4> b)
        {
            array<ll,4> c{};
            for(int i=0;i<2;i++)for(int j=0;j<2;j++)for(int k=0;k<2;k++)c[2*i+j]=(c[2*i+j]+a[2*i+k]*b[2*k+j])%MOD;
            return c;
        };
        for(;n;n>>=1,b=mul(b,b))if(n&1)a=mul(a,b);
        return a[1];
    };
    for(auto n:{0ULL,1ULL,2ULL,1000000000000000000ULL,ULLONG_MAX})assert(linear_nth(f,{1,1},n)==fib(n));
    cout<<"BM and recurrence OK\n";
}
