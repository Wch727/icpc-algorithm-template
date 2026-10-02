#include "../../05-数学/类欧几里得(floor_sum).cpp"
int main()
{
    mt19937 g(203);
    for(int t=0;t<20000;t++)
    {
        ll n=g()%101,m=1+g()%100,a=int(g()%2000001)-1000000,b=int(g()%2000001)-1000000,ans=0;
        for(int i=0;i<n;i++)
        {
            ll x=a*i+b,v=x/m;
            if(x%m<0)--v;
            ans+=v;
        }
        assert(floor_sum(n,m,a,b)==ans);
    }
    assert(floor_sum(0,1,LLONG_MIN,LLONG_MIN)==0);
    assert(floor_sum(1,1,LLONG_MIN,LLONG_MIN)==LLONG_MIN);
    assert(floor_sum(LLONG_MAX,LLONG_MAX,1,0)==0);
    assert(floor_sum(LLONG_MAX,LLONG_MAX,-1,0)==-(LLONG_MAX-1));
    assert(floor_sum(2,LLONG_MAX,0,LLONG_MIN)==-4);
    cout<<"floor_sum OK\n";
}
