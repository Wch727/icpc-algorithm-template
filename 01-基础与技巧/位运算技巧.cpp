#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

typedef unsigned long long ull;

ull lowbit(ull x){return x&(-x);}
bool is_pow2(ull x){return x>0&&(x&(x-1))==0;}
int bit_count(ull x){return __builtin_popcountll(x);}
int bit_length(ull x){return x?64-__builtin_clzll(x):0;}
// __builtin_ctzll(x)：最低位 1 的编号；clzll/ctzll 都不能直接传 0。

// O(popcount(x))，从低到高枚举所有为 1 的位编号。
vector<int> set_bits(ull x)
{
    vector<int> res;
    for(;x;x&=x-1)res.push_back(__builtin_ctzll(x));
    return res;
}

// 单个 mask 的子集共 2^popcount(mask) 个，含 mask 和 0。
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

// 枚举 bits 位全集内包含 mask 的所有超集，0<=bits<=63，mask<2^bits。
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
// 枚举所有 mask 的所有子集共 O(3^bits)，不是 O(4^bits)。
// 上述枚举内的 push_back 可直接换成题目操作，不必存下所有结果。

// Gosper：下一更大的、置位数相同的掩码；0 或已无法用 ull 表示下一项时返回 0。
ull next_combination(ull x)
{
    ull t=lowbit(x),y=x+t;
    return y?y|(((x^y)>>2)/t):0;
}

// 枚举 bits 位中恰好选 k 位的掩码，O(C(bits,k))；0<=k<=bits<=63。
vector<ull> combinations(int bits,int k)
{
    if(k==0)return {0};
    vector<ull> res;
    ull limit=1ULL<<bits;
    for(ull s=(1ULL<<k)-1;s&&s<limit;s=next_combination(s))res.push_back(s);
    return res;
}

// SOS DP：f[mask] 变为所有子集原值之和，O(bits*2^bits)，f.size()==2^bits。
void sos_sum(vector<ll> &f,int bits)
{
    for(int i=0;i<bits;i++)
        for(size_t s=0;s<f.size();s++)
            if(s&(1ULL<<i))f[s]+=f[s^(1ULL<<i)];
}
// 子集和逆变换：+= 改为 -=；超集和：条件改成 !(s&(1ULL<<i))，来源改成 s|(1ULL<<i)。

// 0^1^...^n 按 n%4 为 n、1、n+1、0；[l,r] 异或用 xor_prefix(r)^xor_prefix(l-1)。
ull xor_prefix(ull n){return n%4==0?n:(n%4==1?1:(n%4==2?n+1:0));}
// l=0 时直接 xor_prefix(r)，避免 l-1 下溢；有符号移位不作为掩码使用。

// 对齐块 [b,b+len)：len=2^k，b%len=0，0<=k<64，且整个区间在 ull 值域内。
// 低 k 位异或后仍是一个全排列；高位恒定，可 O(1) 求 sum((b+i)^v)。
unsigned __int128 xor_block_sum(ull b,ull len,ull v)
{
    assert(is_pow2(len)&&b%len==0&&b<=ULLONG_MAX-(len-1));
    return (unsigned __int128)len*(b^(v&~(len-1)))+(unsigned __int128)len*(len-1)/2;
}
