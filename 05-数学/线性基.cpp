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
    LinearBasis(){memset(b,0,sizeof(b));cnt=0;zero=false;}

    // O(60)，插入 x，返回 x 是否与已有基线性相关
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
    ll query_max()
    {
        ll res=0;
        for(int i=62;i>=0;i--)
            if((res^b[i])>res)res^=b[i];
        return res;
    }

    // O(60)，能否把 x 异或出来（含 0，0 恒可）

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
