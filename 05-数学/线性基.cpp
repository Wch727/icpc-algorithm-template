// 适用：集合子集异或的最大值、可表示性与不同异或值的第 k 小。
// 位数：只处理 ll 的 0..62 位，输入须非负；b[i] 最高位为 i，秩 cnt<=63。
// 关键：高位消元保证每个最高位唯一，相关向量不增加秩；build 消去基向量的低位主元。
// 结论：insert 返回 true 表示新向量独立，false 表示相关；原“返回是否相关”注释方向相反。
// 易错：query_kth 包含空集 0，k 是 1-indexed；先 build，后插入新向量要重新 build。
// 边界：cnt=63 时 1LL<<cnt 超出正有符号范围，第 k 小的范围检查不可靠；zero 不改变此处 0 的计数。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 异或线性基：支持插入、最大异或和、第 k 小异或和、判断能否异或得到
// b[i] 存最高位为 i 的基向量；只有 build() 之后才能保证 b 是简化阶梯形
struct LinearBasis{
    ll b[64];
    int cnt;// 基的大小（秩）
    bool zero;// 是否能异或出 0（插入时出现过线性相关）
    // O(64)，清空基与秩；zero 只标记非空子集能否异或成 0。
    LinearBasis(){memset(b,0,sizeof(b));cnt=0;zero=false;}

    // O(60)，插入 x，返回 x 是否与已有基线性相关
    // O(63)，x 为待加入非负整数；找到空主元返回 true，完全消成 0 返回 false。
    bool insert(ll x)
    {
        for(int i=62;i>=0;i--)
        {
            if(!(x>>i&1))continue;
            if(!b[i]){b[i]=x;cnt++;return true;}
            x^=b[i];
        }
        zero=true;// 异或成 0
        return false;
    }

    // O(60)，最大异或和
    // O(63)，从高位贪心，仅在异或后值变大时取该基，返回含空集的最大值。
    ll query_max()
    {
        ll res=0;
        for(int i=62;i>=0;i--)
            if((res^b[i])>res)res^=b[i];
        return res;
    }

    // O(60)，能否把 x 异或出来（含 0，0 恒可）

    // O(63)，判断 x 能否由基异或得到；x=0 恒 true，包括空集。
    bool check(ll x)
    {
        if(!x)return true;
        for(int i=62;i>=0;i--)
        {
            if(!(x>>i&1))continue;
            if(!b[i])return false;
            x^=b[i];
        }
        return true;
    }

    // O(60^2)，化成简化阶梯形：每个 b[i] 的最低位就是 i，且该位在其他基向量里为 0
    // 第 k 小查询前必须先调用
    // O(63^2)，把每个低位主元从更高基中消去，使按低位基选择能升序枚举。
    void build()
    {
        for(int i=62;i>=0;i--)
        {
            if(!b[i])continue;
            for(int j=i-1;j>=0;j--)
                if(b[i]>>j&1)b[i]^=b[j];
        }
    }

    // O(60)，枚举张成里的数，k 从 1 开始且 1<=k<=2^cnt
    // 第 1 小一定是 0（空集或某个异或为 0 的组合），之后是非零值升序
    // O(63)，k 是不同异或结果的第几小；返回 -1 表示越界，cnt=63 的边界另有风险。
    ll query_kth(ll k)
    {
        if(k<1||k>(1LL<<cnt))return -1;
        if(k==1)return 0;
        k--;// 去掉 0，剩下按秩枚举
        ll res=0;
        for(int i=0,j=0;i<=62;i++)
            if(b[i])
            {
                if(k>>j&1)res^=b[i];
                j++;
            }
        return res;
    }
};
