#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;

// ================= 一、快读快写 =================
// getchar 版快读，比 scanf 快 2~3 倍；文件输入时同样有效
inline int read()
{
    int x=0,f=1;
    char c=getchar();
    while(c<'0'||c>'9')
    {
        if(c=='-')f=-1;
        c=getchar();
    }
    while(c>='0'&&c<='9')
    {
        x=(x<<3)+(x<<1)+c-'0';//x*10 用移位加法
        c=getchar();
    }
    return x*f;
}

inline ll readll()
{
    ll x=0;
    int f=1;
    char c=getchar();
    while(c<'0'||c>'9')
    {
        if(c=='-')f=-1;
        c=getchar();
    }
    while(c>='0'&&c<='9')
    {
        x=(x<<3)+(x<<1)+c-'0';
        c=getchar();
    }
    return x*f;
}

// fwrite 缓冲快写，比 printf 快；注意最后要 flush
char obuf[1<<25];
int olen;
inline void write_int(int x)
{
    if(x<0){obuf[olen++]='-';x=-x;}
    if(x>9)write_int(x/10);
    obuf[olen++]=x%10+'0';
}

inline void write_ll(ll x)
{
    if(x<0){obuf[olen++]='-';x=-x;}
    if(x>9)write_ll(x/10);
    obuf[olen++]=x%10+'0';
}

inline void flush_out()
{
    fwrite(obuf,1,olen,stdout);
    olen=0;
}

// ================= 二、常用卡常手段 =================

// 1) 位运算替代算术：%2^n 改成 &(2^n-1)，*2 改成 <<1，/2 改成 >>1
inline int mod_pow2(int x){return x&1023;}//等价 x%1024（x 非负）

// 2) 减少取模：加法/乘法批量后再取模；比较用减法代替取模后的判断
inline int add_mod(int a,int b,int mod){int c=a+b;return c>=mod?c-mod:c;}

// 3) __builtin 系列：popcount / clz / ctz / ffs，常数极小
// __builtin_popcount(x)   二进制 1 的个数
// __builtin_clz(x)        x 的二进制前导 0 个数（x=0 未定义）
// __builtin_ctz(x)        x 的二进制末尾 0 个数（x=0 未定义）
// __builtin_parity(x)     1 的个数的奇偶
inline int lowbit(int x){return x&(-x);}

// 4) 数组连续性：把二维数组按 [小下标][大下标] 声明，访问时内层走大下标，
//    这样 cache 命中率高。A[i][j] 与 A[j][i] 的遍历速度能差几倍。

// 5) 结构体对齐：把 double/int 混排的结构体按大小降序排成员，省空间

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

// 7) 手写 min/max：避免函数调用（开 O2 后基本没差，读代码更直接）
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

// 8) inline / static：小函数加 inline；只在文件内用的加 static 帮助内联
static inline int sqr(int x){return x*x;}

// 9) register 是 C++11 起的废弃关键字，C++17 起不能再用（g++ 直接报错），
//    现代写法是靠 -O2 让编译器自己分配寄存器，不要手写 register

// 10) 编译选项：-O2 -march=native（评测机不一定支持 native，慎用）

// ================= 三、时间测试 =================

double time_ms(function<void()> f)
{
    auto st=chrono::steady_clock::now();
    f();
    auto en=chrono::steady_clock::now();
    return chrono::duration<double,milli>(en-st).count();
}

int a[N];

void bench()
{
    int n=1000000;
    for(int i=1;i<=n;i++)a[i]=i;
    volatile ll sink=0;

    double t1=time_ms([&](){ll s=0;for(int i=1;i<=n;i++)s+=a[i]%1024;sink=s;});
    double t2=time_ms([&](){ll s=0;for(int i=1;i<=n;i++)s+=a[i]&1023;sink=s;});
    printf("[卡常] 取模 %%1024: %.2f ms   位运算 &1023: %.2f ms\n",t1,t2);

    double t3=time_ms([&](){ll s=0;for(int i=1;i<=n;i++)s+=a[i];sink=s;});
    double t4=time_ms([&](){sink=sum_unrolled(a,n);});
    printf("[卡常] 朴素求和: %.2f ms   四路展开求和: %.2f ms\n",t3,t4);

    // 二维数组遍历顺序：行优先（内层走连续内存）vs 列优先
    static int mp[1005][1005];
    for(int i=0;i<1005;i++)
        for(int j=0;j<1005;j++)mp[i][j]=i+j;
    double t5=time_ms([&](){ll s=0;for(int i=0;i<1005;i++)for(int j=0;j<1005;j++)s+=mp[i][j];sink=s;});
    double t6=time_ms([&](){ll s=0;for(int j=0;j<1005;j++)for(int i=0;i<1005;i++)s+=mp[i][j];sink=s;});
    printf("[卡常] 行优先遍历: %.2f ms   列优先遍历: %.2f ms (缓存不友好)\n",t5,t6);

    // 快读 vs scanf：都从同一个文件读 20 万个整数，顺便核对读到的和一样
    int m=200000;
    FILE *fp=fopen("_bench_in.txt","w");
    for(int i=1;i<=m;i++)fprintf(fp,"%d ",(i%2000)-1000);
    fclose(fp);

    fp=freopen("_bench_in.txt","r",stdin);
    double t7=time_ms([&](){ll s=0;for(int i=1;i<=m;i++)s+=read();sink=s;});
    ll s1=sink;
    fclose(stdin);

    fp=freopen("_bench_in.txt","r",stdin);
    double t8=time_ms([&](){ll s=0;for(int i=1;i<=m;i++){int x;scanf("%d",&x);s+=x;}sink=s;});
    ll s2=sink;
    fclose(stdin);

    // 还回控制台并清掉 EOF 标志，否则后面的 cin/scanf 会直接读到 EOF
    fp=freopen("CONIN$","r",stdin);
    (void)fp;
    clearerr(stdin);
    remove("_bench_in.txt");
    printf("[卡常] 快读 20 万整数: %.2f ms   scanf: %.2f ms   结果一致=%d\n",
        t7,t8,(int)(s1==s2));
}
