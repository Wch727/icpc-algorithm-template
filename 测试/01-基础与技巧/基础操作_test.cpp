#include<bits/stdc++.h>
using namespace std;
namespace discrete_test {
#include "../../01-基础与技巧/离散化.cpp"
}
namespace bits {
#include "../../01-基础与技巧/位运算技巧.cpp"
}
namespace sos {
#include "../../06-动态规划/SOS子集与超集和.cpp"
}
namespace mono_queue {
#include "../../01-基础与技巧/单调队列.cpp"
}

int main()
{
    mt19937 rng(20261001);
    for(int t=0;t<100;t++)
    {
        int n=8;
        discrete_test::n=n;
        vector<long long> a(n+1);
        for(int i=1;i<=n;i++)a[i]=(int)(rng()%21)-10;
        for(int i=1;i<=n;i++)discrete_test::a[i]=a[i];
        discrete_test::discrete();
        for(int i=1;i<=n;i++)
        {
            set<long long> smaller;
            for(int j=1;j<=n;j++)if(a[j]<a[i])smaller.insert(a[j]);
            assert(discrete_test::id[i]==(int)smaller.size()+1);
            assert(discrete_test::b[discrete_test::id[i]-1]==a[i]);
        }
        mono_queue::n=n;
        for(int i=1;i<=n;i++)mono_queue::a[i]=(int)(rng()%7)-3;
        for(int k=1;k<=n;k++)
        {
            mono_queue::k=k;
            mono_queue::solve();
            for(int i=1;i<=n;i++)
            {
                int want=max(1,i-k+1);
                for(int j=want+1;j<=i;j++)if(mono_queue::a[j]<=mono_queue::a[want])want=j;
                assert(mono_queue::ans[i]==want);
            }
            vector<long long> values(n+1),want_dp(n+1);
            for(int i=1;i<=n;i++)values[i]=(int)(rng()%21)-10;
            for(int i=1;i<=n;i++)
            {
                long long best=LLONG_MIN;
                for(int j=max(0,i-k);j<i;j++)best=max(best,want_dp[j]);
                want_dp[i]=best+values[i];
            }
            for(int i=1;i<=n;i++)mono_queue::a[i]=values[i];
            mono_queue::dp();
            for(int i=0;i<=n;i++)assert(mono_queue::f[i]==want_dp[i]);
        }
    }
    for(unsigned long long mask=0;mask<256;mask++)
    {
        vector<unsigned long long> want;
        for(unsigned long long s=mask;;s--)
        {
            if((s&mask)==s)want.push_back(s);
            if(s==0)break;
        }
        assert(bits::submasks(mask)==want);
        int count=0,length=0;
        for(unsigned long long x=mask;x;x>>=1)count+=x&1,length++;
        assert(bits::bit_count(mask)==count&&bits::bit_length(mask)==length);
        assert(bits::is_pow2(mask)==(count==1));
        vector<int> positions;
        for(int i=0;i<8;i++)if(mask&(1ULL<<i))positions.push_back(i);
        assert(bits::set_bits(mask)==positions);
    }
    for(int b=0;b<=8;b++)
    {
        unsigned long long limit=1ULL<<b;
        for(unsigned long long mask=0;mask<limit;mask++)
        {
            vector<unsigned long long> want;
            for(unsigned long long s=mask;s<limit;s++)if((s&mask)==mask)want.push_back(s);
            assert(bits::supermasks(mask,b)==want);
        }
        for(int k=0;k<=b;k++)
        {
            vector<unsigned long long> want;
            for(unsigned long long s=0;s<limit;s++)if(__builtin_popcountll(s)==k)want.push_back(s);
            assert(bits::combinations(b,k)==want);
        }
        vector<long long> original(limit),f;
        for(auto &x:original)x=(int)(rng()%21)-10;
        f=original;
        sos::subset_sum(f);
        for(unsigned long long s=0;s<limit;s++)
        {
            long long want=0;
            for(unsigned long long t=0;t<limit;t++)if((t&s)==t)want+=original[t];
            assert(f[s]==want);
        }
    }
    unsigned long long prefix=0;
    for(unsigned long long n=0;n<=1000;n++)
    {
        prefix^=n;
        assert(bits::xor_prefix(n)==prefix);
    }
    assert(bits::next_combination(0)==0);
    assert(bits::next_combination(1ULL<<63)==0);
    assert(bits::next_combination((1ULL<<63)-1)==((1ULL<<63)|((1ULL<<62)-1)));
    assert(bits::combinations(63,63)==vector<unsigned long long>{(1ULL<<63)-1});
    assert(bits::combinations(63,1).back()==(1ULL<<62));
    assert(bits::set_bits(1ULL<<63)==vector<int>{63});
    assert(bits::lowbit(1ULL<<63)==(1ULL<<63));
    assert(bits::bit_length(1ULL<<63)==64);
    puts("基础操作与窗口/DP 单调队列随机对拍：OK");
}
