// @code common
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


// @code random
unsigned long long seed=chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 rnd(seed);
ll randint(ll l,ll r){return uniform_int_distribution<ll>(l,r)(rnd);}

// @code stress
vector<ll> gen(); // 小数据生成器，主动覆盖重复、负数、极小规模等边界
ll solve(vector<ll> a);
ll brute(vector<ll> a);
bool stress(int rounds)
{
    rnd.seed(seed);
    for(int t=1;t<=rounds;t++)
    {
        vector<ll> a=gen();
        ll x=solve(a),y=brute(a);
        if(x!=y)
        {
            cerr<<"seed="<<seed<<" case="<<t<<" got="<<x<<" expected="<<y<<'\n';
            cerr<<a.size()<<'\n';
            for(ll v:a)cerr<<v<<' ';
            cerr<<'\n';
            return false; // 首次不同就停，保留种子和完整输入；别只打印答案
        }
    }
    return true;
}
