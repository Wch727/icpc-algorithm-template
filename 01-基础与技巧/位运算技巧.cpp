// @code common
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

typedef unsigned long long ull;

// @code primitives
ull lowbit(ull x){return x&(-x);}
bool is_pow2(ull x){return x>0&&(x&(x-1))==0;}
int bit_count(ull x){return __builtin_popcountll(x);}
int bit_length(ull x){return x?64-__builtin_clzll(x):0;}

// @code set_bits
vector<int> set_bits(ull x)
{
    vector<int> res;
    for(;x;x&=x-1)res.push_back(__builtin_ctzll(x));
    return res;
}

// @code subsets
vector<ull> submasks(ull mask)
{
    vector<ull> res;
    for(ull s=mask;;s=(s-1)&mask)
    {
        res.push_back(s);
        if(s==0)break;
    }
    return res;
}

// @code supersets
vector<ull> supermasks(ull mask,int bits)
{
    ull all=(1ULL<<bits)-1;
    vector<ull> res;
    for(ull s=mask;;s=(s+1)|mask)
    {
        res.push_back(s);
        if(s==all)break;
    }
    return res;
}

// @code fixed_size
ull next_combination(ull x)
{
    ull t=lowbit(x),y=x+t;
    return y?y|(((x^y)>>2)/t):0;
}

vector<ull> combinations(int bits,int k)
{
    if(k==0)return {0};
    vector<ull> res;
    ull limit=1ULL<<bits;
    for(ull s=(1ULL<<k)-1;s&&s<limit;s=next_combination(s))res.push_back(s);
    return res;
}

// @code subset_count
unsigned long long count_subset(unsigned mask,unsigned allow){return 1ULL<<__builtin_popcount(mask&allow);}

// @code xor_range
ull xor_prefix(ull n){return n%4==0?n:(n%4==1?1:(n%4==2?n+1:0));}

// @code xor_block
unsigned __int128 xor_block_sum(ull b,ull len,ull v)
{
    assert(is_pow2(len)&&b%len==0&&b<=ULLONG_MAX-(len-1));
    return (unsigned __int128)len*(b^(v&~(len-1)))+(unsigned __int128)len*(len-1)/2;
}
