// 适用：集合子集异或的最大值、可表示性与不同异或值的第 k 小。
// 位数：只处理 ll 的 0..62 位，输入须非负；b[i] 最高位为 i，秩 cnt<=63。
// 关键：高位消元保证每个最高位唯一，相关向量不增加秩；insert 返回是否独立。
// 易错：query_kth 包含空集 0，k 是 1-indexed；先 build，后插入新向量要重新 build。
// k 用 unsigned long long，秩 63 时最后一项的序号是 2^63；zero 不改变不同值的计数。
// 空间判等：普通基依赖插入顺序；各自 build 后逐项比较 b[0..62] 才是唯一规范形。
// 如 {3,5} 与 {6,3} 张成同一空间。判等不比较 zero：它记录输入相关性，不是空间性质。
// 哈希只能加速筛选，须考虑碰撞；全零非空区间也会生成零空间，不能直接丢弃。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 异或线性基：支持插入、最大异或和、第 k 小异或和、判断能否异或得到
// b[i] 存最高位为 i 的基向量；只有 build() 之后才能保证 b 是简化阶梯形
struct LinearBasis{
    ll b[64];
    int cnt;// 基的大小（秩）
    bool zero;// 是否存在异或为 0 的非空子集
    LinearBasis()
    {
        memset(b, 0, sizeof(b));
        cnt= 0;
        zero= false;
    }

    // O(63)，插入 x，返回 x 是否独立
    bool insert(ll x)
    {
        for(int i=62;i>=0;i--)
        {
            if(!(x>>i&1))continue;
            if(!b[i])
            {
                b[i]= x;
                cnt++;
                return true;
            }
            x^=b[i];
        }
        zero=true;// 异或成 0
        return false;
    }

    // O(63)，最大异或和
    ll query_max()
    {
        ll res=0;
        for(int i=62;i>=0;i--)
            if((res^b[i])>res)res^=b[i];
        return res;
    }

    // O(63)，能否把 x 异或出来（含 0，0 恒可）

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

    // O(63^2)，化成唯一约化形；所有主元位在其他向量里均为 0
    // 第 k 小查询前必须先调用
    void build()
    {
        for(int i=62;i>=0;i--)
        {
            if(!b[i])continue;
            for(int j=i-1;j>=0;j--)
                if(b[i]>>j&1)b[i]^=b[j];
        }
    }

    // O(63)，枚举张成里的数，k 从 1 开始且 1<=k<=2^cnt
    // 第 1 小一定是 0（空集或某个异或为 0 的组合），之后是非零值升序
    ll query_kth(unsigned long long k)
    {
        if(k<1||k>(1ULL<<cnt))return -1;
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

// 区间异或：按位置 1..n 插入 a[r]，此时 query_max(l) 查 [l,r] 的最大子集异或。
// 同主元优先保留靠右的向量，被换出的旧向量继续消元；不能只覆盖而丢掉它。
// 每个 b[i] 能由 pos[i]..r 的元素表示，筛选 pos[i]>=l 后恰好张成 [l,r]。
// 插入、查询 O(63)，空间 O(63)。离线询问按右端点排序，不必保存所有前缀。
struct RangeLinearBasis
{
    ll b[63]{};
    int pos[63]{};

    void insert(ll x,int p)
    {
        for(int i=62;i>=0;i--)
        {
            if(!(x>>i&1))continue;
            if(!b[i])
            {
                b[i]= x;
                pos[i]= p;
                return;
            }
            if(pos[i] < p)
            {
                swap(b[i], x);
                swap(pos[i], p);
            }
            x^=b[i];
        }
    }

    ll query_max(int l)
    {
        ll res=0;
        for(int i=62;i>=0;i--)
            if(pos[i]>=l&&(res^b[i])>res)res^=b[i];
        return res;
    }
};
// 求区间空间的规范形：把满足 pos[i]>=l 的向量插入普通基，再 build，O(63^2)。
// 固定 r 移动 l，仅跨过至多 63 个有效位置时空间会改变，可跳过其余左端点。
// 位置基不能直接 build：反消可能混入更早的位置，破坏区间筛选的不变量。
