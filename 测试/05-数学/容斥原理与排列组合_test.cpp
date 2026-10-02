// 容斥原理与排列组合 的测试与对拍代码
// 模板本体：05-数学/容斥原理与排列组合.cpp
#include "../../05-数学/容斥原理与排列组合.cpp"

int main()
{
    srand(19260817);
    init(1000);
    assert(avoid(LLONG_MAX,{LLONG_MAX})==LLONG_MAX-1);
    assert(avoid(LLONG_MAX,{LLONG_MAX,LLONG_MAX-1})==LLONG_MAX-2);
    assert(avoid(0,{1,2})==0&&avoid(10,{2,2})==5);
    bool ok=derange(4)==9&&multiset_count({2,1})==3&&avoid(10,{2,3})==3;
    for(int t=1;t<=40;t++)
    {
        int n=rand()%9;
        vector<int> p(n);
        iota(p.begin(),p.end(),0);
        ll want=0;
        do
        {
            bool f=true;
            for(int i=0;i<n;i++)if(p[i]==i)f=false;
            want+=f;
        }while(next_permutation(p.begin(),p.end()));
        if(derange(n)!=want)ok=false;
        vector<int> cnt(3),a;
        n=rand()%9;
        for(int i=0;i<n;i++)cnt[rand()%3]++;
        for(int i=0;i<3;i++)for(int j=0;j<cnt[i];j++)a.push_back(i);
        want=0;
        do{want++;}while(next_permutation(a.begin(),a.end()));
        if(multiset_count(cnt)!=want)ok=false;
        int m=rand()%7;
        vector<ll> d(m);
        for(ll &x:d)x=rand()%15+1;
        n=rand()%100+1,want=0;
        for(int x=1;x<=n;x++)
        {
            bool f=true;
            for(ll v:d)if(x%v==0)f=false;
            want+=f;
        }
        if(avoid(n,d)!=want)ok=false;
        n=rand()%9;
        for(int k=0;k<=n;k++)
        {
            want=0;
            for(int s=0;s<(1<<n);s++)if(__builtin_popcount((unsigned)s)==k)want++;
            if(C(n,k)!=want)ok=false;
        }
    }
    printf("容斥与排列组合 %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
