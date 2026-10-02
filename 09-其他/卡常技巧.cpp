#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 实用卡常：先定位瓶颈，再实测；快读写见第一章，基准测量在测试文件。

// 1) 非负整数且结果不溢出时可用掩码/移位；常量乘除/取模通常已被编译器优化。
inline int mod_pow2(int x){return x&1023;}//等价 x%1024（x 非负）

// 2) 两个已归一化的数做模加，只需判断并减一次模数；批量累加须保证不溢出。
inline int add_mod(int a,int b,int mod){ll c=1LL*a+b;return c>=mod?c-mod:c;} // 0<=a,b<mod，mod>0

// 3) __builtin 系列：popcount / clz / ctz / ffs，常数极小
// __builtin_popcount(x)   二进制 1 的个数
// __builtin_clz(x)        x 的二进制前导 0 个数（x=0 未定义）
// __builtin_ctz(x)        x 的二进制末尾 0 个数（x=0 未定义）
// __builtin_parity(x)     1 的个数的奇偶
inline int lowbit(int x){return x&(-x);} // x 非负

// 4) 二维数组最后一维连续；a[i][j] 尽量让 j 作内层循环，避免跨行访问。

// 5) 结构体成员按对齐需求排列可减少填充，实际检查 sizeof；位宽/平台会影响结果。

// 6) 循环展开 + 局部变量：把 a[i] 提到寄存器里，减少重复寻址
ll sum_unrolled(int *a,int n)
{
    ll s=0;
    int i=1;
    for(;i+3<=n;i+=4)
    {
        s+=a[i],s+=a[i+1],s+=a[i+2],s+=a[i+3];
    }
    for(;i<=n;i++)s+=a[i];
    return s;
}

// 7) min/max 通常能内联；以下宏会重复求值，参数不要带 i++ 等副作用。
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

// 8) inline 不保证强制内联，static 表示内部链接；最终性能取决于编译优化和实测。
static inline int sqr(int x){return x*x;} // 要求结果不溢出 int

// 9) register 在 C++17 已移除，部分编译器只给警告；不要依赖它来优化，
//    现代写法是靠 -O2 让编译器自己分配寄存器，不要手写 register

// 10) 编译选项：-O2 -march=native（评测机不一定支持 native，慎用）
