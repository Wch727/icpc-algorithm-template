// 卡常技巧 的测试与对拍代码
// 模板本体：09-其他/卡常技巧.cpp
#include "../../09-其他/卡常技巧.cpp"

const int N=1000005;
// ================= 一、快读快写 =================
// getchar 版快读：输入合法且在返回类型范围内，按已知数量读取；EOF 返回 0。
inline ll readll()
{
    int c;
    bool neg=false;
    while((c=getchar())<'0'||c>'9')
    {
        if(c==EOF)return 0;
        if(c=='-')neg=true;
    }
    unsigned long long x=c-'0';
    while((c=getchar())>='0'&&c<='9')x=x*10+c-'0';
    if(neg)return x==(1ULL<<63)?LLONG_MIN:-(ll)x;
    return (ll)x;
}
inline int read(){return (int)readll();}

// fwrite 缓冲快写，满时自动刷新，结束仍须 flush_out；分隔符用 write_char。
char obuf[1<<25];
int olen;
inline void flush_out()
{
    fwrite(obuf,1,olen,stdout);
    olen=0;
}
inline void write_char(char c)
{
    if(olen==(int)sizeof obuf)flush_out();
    obuf[olen++]=c;
}
inline void write_ll(ll x)
{
    unsigned long long u=x;
    if(x<0)write_char('-'),u=0-u;
    char s[20];
    int cnt=0;
    do{s[cnt++]='0'+u%10;u/=10;}while(u);
    while(cnt)write_char(s[--cnt]);
}
inline void write_int(int x){write_ll(x);}

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


// ================= 四、自测 =================

void test_builtin()
{
    // popcount / clz / ctz 与暴力对拍
    mt19937 rnd(12345);
    int ok=1;
    for(int t=1;t<=200000;t++)
    {
        int x=rnd()&0x7fffffff;// 只取 31 位，避免负数让"数 31 位"的暴力 popcount 对不上
        if(x==0)continue;
        int c=0;
        for(int i=0;i<31;i++)if(x>>i&1)c++;
        if(c!=__builtin_popcount(x))ok=0;
        int z=0;//末尾 0 个数
        while((x>>z&1)==0)z++;
        if(z!=__builtin_ctz(x))ok=0;
        int lz=0;//前导 0 个数（32 位）
        while((x>>(31-lz)&1)==0)lz++;
        if(lz!=__builtin_clz(x))ok=0;
    }
    printf("[builtin] 3 个函数各 20 万次与暴力对拍 %s\n",ok?"全部通过":"失败");
    assert(ok);
    printf("[builtin] popcount(0b101101)=%d (期望 4)  ctz(0b1000)=%d (期望 3)  clz(1)=%d (期望 31)\n",
        __builtin_popcount(0b101101),__builtin_ctz(0b1000),__builtin_clz(1));
    printf("[builtin] parity(7)=%d (期望 1，7 有 3 个 1)  __lg(8)=%d (期望 3)  lowbit(12)=%d (期望 4)\n",
        __builtin_parity(7),__lg(8),12&-12);
}

void test_fastio()
{
    // 把一段数字写进文件，再用快读读回来核对
    string s;
    for(int i=1;i<=1000;i++)s+=to_string(i),s+=' ';
    s+="-5 -12345 0 -2147483648 2147483647 -9223372036854775808 9223372036854775807";
    FILE *fp=fopen("_io_in.txt","w");
    fputs(s.c_str(),fp);
    fclose(fp);
    freopen("_io_in.txt","r",stdin);
    ll sum=0;
    for(int i=1;i<=1000;i++)sum+=read();
    int x1=read(),x2=read(),x3=read();
    assert(sum==500500&&x1==-5&&x2==-12345&&x3==0);
    assert(read()==INT_MIN&&read()==INT_MAX);
    assert(readll()==LLONG_MIN&&readll()==LLONG_MAX&&readll()==0);
    fclose(stdin);
    freopen("CONIN$","r",stdin);
    clearerr(stdin);
    remove("_io_in.txt");
    printf("[fastio] 1..1000 之和=%lld (期望 500500)  读到 %d %d %d (期望 -5 -12345 0)\n",sum,x1,x2,x3);
    printf("[fastio] add_mod(7,8,10)=%d (期望 5)\n",add_mod(7,8,10));
    olen=0;
    write_int(-123);
    write_ll(456789012345ll);
    string got(obuf,obuf+olen);
    assert(got=="-123456789012345");
    olen=0;
    write_int(INT_MIN);write_char(' ');write_ll(LLONG_MIN);
    assert(string(obuf,obuf+olen)=="-2147483648 -9223372036854775808");
    olen=0;
    assert(add_mod(INT_MAX-1,INT_MAX-1,INT_MAX)==INT_MAX-2);
    printf("[fastio] 快写输出 \"%s\" (期望 -123456789012345)\n",got.c_str());
}

int main()
{
    test_builtin();
    test_fastio();
    bench();
    return 0;
}
