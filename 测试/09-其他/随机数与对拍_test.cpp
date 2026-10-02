#include "../../09-其他/随机数与对拍.cpp"

bool wrong=false;
vector<ll> gen()
{
    vector<ll> a(randint(1,20));
    for(ll &x:a)x=randint(-20,20);
    return a;
}
ll solve(vector<ll> a)
{
    ll best=LLONG_MIN,sum=0;
    for(ll x:a)sum=max(x,sum+x),best=max(best,sum);
    return best+(wrong?1:0);
}
ll brute(vector<ll> a)
{
    ll best=LLONG_MIN;
    for(int l=0;l<(int)a.size();l++)
    {
        ll sum=0;
        for(int r=l;r<(int)a.size();r++)sum+=a[r],best=max(best,sum);
    }
    return best;
}
int main()
{
    seed=20261001;
    assert(randint(LLONG_MIN,LLONG_MIN)==LLONG_MIN);
    assert(randint(LLONG_MAX,LLONG_MAX)==LLONG_MAX);
    rnd.seed(seed);
    vector<ll> first=gen();
    rnd.seed(seed);
    assert(gen()==first);
    vector<int> p(100);
    iota(p.begin(),p.end(),1);
    shuffle(p.begin(),p.end(),rnd);
    sort(p.begin(),p.end());
    for(int i=0;i<100;i++)assert(p[i]==i+1);
    assert(stress(1000));
    wrong=true;
    ostringstream log;
    auto old=cerr.rdbuf(log.rdbuf());
    bool result=stress(1);
    cerr.rdbuf(old);
    assert(!result&&log.str().find("seed=20261001 case=1")!=string::npos);
    assert(log.str().find(to_string(first.size())+"\n")!=string::npos);
    puts("随机生成、种子复现、实际对拍与失败报告：OK");
}
