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

int main()
{
    srand(20240522);
    // 自测1：lowbit / is_pow2 / bit_count / bit_length
    for(int x=1;x<=100000;x++)
    {
        if(lowbit(x)!=(x&(-x)))
        {
            printf("fail lowbit x=%d\n",x);
            return 0;
        }
        if(is_pow2(x)!=((x&(x-1))==0))
        {
            printf("fail pow2 x=%d\n",x);
            return 0;
        }
        if(bit_count(x)!=__builtin_popcount((unsigned)x)||bit_count_ll(x)!=__builtin_popcount((unsigned)x))
        {
            printf("fail popcount x=%d\n",x);
            return 0;
        }
        int len=0,t=x;
        while(t>0)t>>=1,len++;
        if(bit_length(x)!=len)
        {
            printf("fail bit_length x=%d\n",x);
            return 0;
        }
    }
    printf("lowbit/popcount self-check OK\n");
    printf("lowbit(12)=%d bit_count(12)=%d bit_length(12)=%d is_pow2(12)=%d\n",lowbit(12),bit_count(12),bit_length(12),(int)is_pow2(12));

    // 自测2：树状数组下标跳转
    for(int x=1;x<=10000;x++)
    {
        if(fenwick_next(x)!=(x+(x&(-x)))||fenwick_prev(x)!=(x-(x&(-x))))
        {
            printf("fail fenwick x=%d\n",x);
            return 0;
        }
    }
    printf("fenwick jump self-check OK\n");

    // 自测3：枚举子集数量 = 2^popcount
    for(int mask=0;mask<(1<<12);mask++)
    {
        vector<int> s=submasks(mask);
        if((int)s.size()!=(1<<__builtin_popcount((unsigned)mask)))
        {
            printf("fail submasks mask=%d\n",mask);
            return 0;
        }
        for(int x:s)
            if((x&mask)!=x)
            {
                printf("fail submask value\n");
                return 0;
            }
    }
    printf("submask self-check OK\n");

    // 自测4：异或性质——只出现一次的数；两个只出现一次的数
    for(int t=1;t<=300;t++)
    {
        int len=rand()%10+1;
        vector<int> v;
        for(int i=1;i<=len;i++)v.push_back(rand()%1000),v.push_back(v.back());// 每个数出现两次
        int x=rand()%1000;
        v.push_back(x);
        int all=0;
        for(int y:v)all^=y;
        if(all!=x)
        {
            printf("fail xor single got=%d want=%d\n",all,x);
            return 0;
        }
    }
    printf("xor single self-check OK\n");
    for(int t=1;t<=300;t++)
    {
        vector<int> v;
        for(int i=1;i<=8;i++)v.push_back(rand()%1000),v.push_back(v.back());
        int x=rand()%1000,y=rand()%1000;
        while(y==x)y=rand()%1000;
        v.push_back(x),v.push_back(y);
        int all=0;
        for(int z:v)all^=z;
        int low=lowbit(all);// 两个数不同的最低位
        int p=0,q=0;
        for(int z:v)
        {
            if(z&low)p^=z;
            else q^=z;
        }
        if(min(p,q)!=min(x,y)||max(p,q)!=max(x,y))
        {
            printf("fail xor pair got=%d,%d want=%d,%d\n",p,q,x,y);
            return 0;
        }
    }
    printf("xor pair self-check OK\n");

    // 自测5：洛谷 P1469 的用法 + 前缀异或
    n=5;
    int w[6]={0,1,2,3,1,2};// 只出现一次的是 3
    int all=0;
    for(int i=1;i<=n;i++)a[i]=w[i],all^=a[i];
    printf("P1469 ans=%d (want 3)\n",all);
    int pre=0;
    for(int i=1;i<=n;i++)pre^=a[i],printf("%d ",pre);
    printf("\n");

    // 自测6：1e6 规模下 x&(x-1) 计数速度
    n=1000000;
    for(int i=1;i<=n;i++)a[i]=rand();
    clock_t st=clock();
    ll sum=0;
    for(int i=1;i<=n;i++)sum+=bit_count(a[i]);
    printf("n=%d total_bits=%lld time=%.3fs\n",n,sum,(double)(clock()-st)/CLOCKS_PER_SEC);
    return 0;
}
