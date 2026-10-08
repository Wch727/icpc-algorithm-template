// 在 0<=p<=P、1<=q<=Q 的既约分数中找单调判定边界，P>=0、Q>=1。
// check(p,q) 为真是一个向下闭合的前缀，必须含 0/1；返回 {最大真分数,最小假分数}。
// 无合法假分数时右界为 1/0（正无穷哨兵），不调用 check(1,0)；P,Q<=1e18。
// 连续同方向走 k 步用倍增+二分加速，总 O(log(P+Q)) 次判定，额外空间 O(1)。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
template<class Check>
array<pair<ll,ll>,2> fraction_bounds(ll P,ll Q,Check check)
{
    assert(0<=P&&P<=1000000000000000000LL&&1<=Q&&Q<=1000000000000000000LL);
    assert(check(0,1));
    ll a=0,b=1,c=1,d=0;
    while((__int128)a+c<=P&&(__int128)b+d<=Q)
    {
        bool left=check(a+c,b+d);
        ll x=left?a:c,y=left?b:d,u=left?c:a,v=left?d:b;
        ll cap= LLONG_MAX;
        if(u)
            cap= min(cap, (P - x) / u);
        if(v)
            cap= min(cap, (Q - y) / v);
        auto same= [&](ll k) { return check(x + k * u, y + k * v) == left; };
        ll good=1,bad=cap;
        while(good<cap)
        {
            ll k=good>cap/2?cap:good*2;
            if(!same(k))
            {
                bad= k;
                break;
            }
            good=k;
        }
        while(good+1<bad)
        {
            ll k=good+(bad-good)/2;
            if(same(k))good=k;else bad=k;
        }
        if(left)a=x+good*u,b=y+good*v;
        else c=x+good*u,d=y+good*v;
    }
    return {{{a, b}, {c, d}}};
}
// 邻界满足 cb-ad=1；mediant=(a+c)/(b+d)，在它之间的其他既约分数分子分母更大。
// 精确比较 p/q<=x/y：(__int128)p*y<=(__int128)x*q；不要转 double 判等。
// check 很贵时可缓存结果；链内反复走一次 mediant 可能退化为 O(P+Q)。
