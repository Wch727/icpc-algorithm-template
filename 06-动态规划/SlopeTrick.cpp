// 凸分段线性函数 f(x)=min_value + Σ max(l-x,0) + Σ max(x-r,0)，整数断点、单位斜率。
// 两堆维护拐点，add_abs(a) 加 |x-a|；add_left/right 分别加 max(a-x,0)/max(x-a,0)。
// prefix_min：f(x)=min(y<=x)f(y)；suffix_min 对称。shift(a,b)：min(x-b<=y<=x-a)f(y)，a<=b。
// 每次加项 O(log n)，平移 O(1)，eval O(n) 仅作查询/验证；数值须放入 ll。
// 例：dp_i(x)=|x-a_i|+min(y<=x)dp_(i-1)(y)，每轮 prefix_min(); add_abs(a_i)。
#include<bits/stdc++.h>
using namespace std;
struct SlopeTrick
{
    using ll=long long;priority_queue<ll> L;priority_queue<ll,vector<ll>,greater<ll>> R;
    ll left_shift=0,right_shift=0,min_value=0;
    void add_left(ll a)
    {
        if(!R.empty())
            min_value+= max(0LL, a - (R.top() + right_shift));
        R.push(a - right_shift);
        ll x= R.top() + right_shift;
        R.pop();
        L.push(x - left_shift);
    }
    void add_right(ll a)
    {
        if(!L.empty())
            min_value+= max(0LL, (L.top() + left_shift) - a);
        L.push(a - left_shift);
        ll x= L.top() + left_shift;
        L.pop();
        R.push(x - right_shift);
    }
    void add_abs(ll a)
    {
        add_left(a);
        add_right(a);
    }
    void add_constant(ll a){min_value+=a;}
    void prefix_min(){R={};}
    void suffix_min(){L={};}
    void shift(ll a, ll b)
    {
        assert(a <= b);
        left_shift+= a;
        right_shift+= b;
    }
    // 此处 eval 只求凸函数数值，不解析或执行任何代码。
    ll eval(ll x) const
    {
        ll ans= min_value;
        auto l= L;
        auto r= R;
        while(!l.empty())
            ans+= max(0LL, l.top() + left_shift - x), l.pop();
        while(!r.empty())
            ans+= max(0LL, x - r.top() - right_shift), r.pop();
        return ans;
    }
};
