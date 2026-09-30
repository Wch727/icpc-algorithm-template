#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 位运算技巧：lowbit / 枚举子集 / 二进制位数 / 异或性质
int n;
int a[N];

inline int lowbit(int x)// x&(-x)，取最低位 1 对应的值
{
    return x&(-x);
}

inline bool is_pow2(int x)// 2 的幂判定，x>0
{
    return x>0&&(x&(x-1))==0;
}

inline int bit_count(int x)// 二进制中 1 的个数，O(log x)
{
    int cnt=0;
    while(x>0)x&=x-1,cnt++;// x&(x-1) 消掉最低位的 1
    return cnt;
}

inline int bit_count_ll(ll x)
{
    int cnt=0;
    while(x>0)x&=x-1,cnt++;
    return cnt;
}

inline int bit_length(ll x)// 最高位 1 的位置，x=0 返回 0
{
    int len=0;
    while(x>0)x>>=1,len++;
    return len;
}

inline int fenwick_next(int x)// 树状数组向上走的下标
{
    return x+lowbit(x);
}

inline int fenwick_prev(int x)// 树状数组向下走的下标
{
    return x-lowbit(x);
}

vector<int> submasks(int mask)// 枚举 mask 的所有子集（含 0 和 mask），O(3^n) 总量
{
    vector<int> res;
    for(int s=mask;;s=(s-1)&mask)
    {
        res.push_back(s);
        if(s==0)break;
    }
    return res;
}

vector<int> subsets_by_pop(int bits)// 按 popcount 从小到大枚举 1..(1<<bits)-1 的非空子集
{
    vector<int> res;
    for(int s=1;s<(1<<bits);s++)res.push_back(s);
    sort(res.begin(),res.end(),[](int x,int y)
    {
        int cx=__builtin_popcount(x),cy=__builtin_popcount(y);
        if(cx!=cy)return cx<cy;
        return x<y;
    });
    return res;
}
