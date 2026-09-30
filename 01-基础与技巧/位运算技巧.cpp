// 适用：状态压缩、子集枚举、树状数组下标跳转；位编号从最低位 0 开始。
// 本组接口按非负整数使用；1<<bits 要求 0<=bits<31，低位操作避免 INT_MIN 取负。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// 位运算技巧：lowbit / 枚举子集 / 二进制位数 / 异或性质
int n;
int a[N];

// O(1)，返回 x 最低有效位的权值，x=0 返回 0；补码相与消去其余位。
inline int lowbit(int x)// x&(-x)，取最低位 1 对应的值
{
    return x&(-x);
}

// O(1)，判断 x 是否为正的 2 的幂；零不能只靠 x&(x-1) 判断。
inline bool is_pow2(int x)// 2 的幂判定，x>0
{
    return x>0&&(x&(x-1))==0;
}

// O(置位数)，计数非负 int x 的 1；每轮清除一个最低位 1。
inline int bit_count(int x)// 二进制中 1 的个数，O(log x)
{
    int cnt=0;
    while(x>0)x&=x-1,cnt++;// x&(x-1) 消掉最低位的 1
    return cnt;
}

// O(置位数)，计数非负 ll x 的 1；负数不会进入此循环。
inline int bit_count_ll(ll x)
{
    int cnt=0;
    while(x>0)x&=x-1,cnt++;
    return cnt;
}

// O(log x)，非负 x 的有效二进制位数；返回长度而非从 0 开始的位编号。
inline int bit_length(ll x)// 最高位 1 的位置，x=0 返回 0
{
    int len=0;
    while(x>0)x>>=1,len++;
    return len;
}

// O(1)，x 为 1-based 树状数组下标；跳到父块，调用方检查是否超过 n。
inline int fenwick_next(int x)// 树状数组向上走的下标
{
    return x+lowbit(x);
}

// O(1)，x 为 1-based 树状数组下标；剥去最低位对应块，0 为终止哨兵。
inline int fenwick_prev(int x)// 树状数组向下走的下标
{
    return x-lowbit(x);
}

// O(2^k) 时间和空间，k=mask 的置位数；降序返回所有子掩码，先判零避免回到 mask。
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

// O(bits * 2^bits)，返回非空掩码；先按置位数后按数值排序，空间 O(2^bits)。
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
