// 卡常技巧 的测试与对拍代码
// 模板本体：09-其他/卡常技巧.cpp
#include "../../09-其他/卡常技巧.cpp"

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
    printf("[builtin] popcount(0b101101)=%d (期望 4)  ctz(0b1000)=%d (期望 3)  clz(1)=%d (期望 31)\n",
        __builtin_popcount(0b101101),__builtin_ctz(0b1000),__builtin_clz(1));
    printf("[builtin] parity(7)=%d (期望 1，7 有 3 个 1)  __lg(8)=%d (期望 3)  lowbit(12)=%d (期望 4)\n",
        __builtin_parity(7),__lg(8),lowbit(12));
}

void test_fastio()
{
    // 把一段数字写进文件，再用快读读回来核对
    string s;
    for(int i=1;i<=1000;i++)s+=to_string(i),s+=' ';
    s+="-5 -12345 0";
    FILE *fp=fopen("_io_in.txt","w");
    fputs(s.c_str(),fp);
    fclose(fp);
    freopen("_io_in.txt","r",stdin);
    ll sum=0;
    for(int i=1;i<=1000;i++)sum+=read();
    int x1=read(),x2=read(),x3=read();
    fclose(stdin);
    freopen("CONIN$","r",stdin);
    clearerr(stdin);
    remove("_io_in.txt");
    printf("[fastio] 1..1000 之和=%lld (期望 500500)  读到 %d %d %d (期望 -5 -12345 0)\n",sum,x1,x2,x3);
    printf("[fastio] mod_pow2(1025)=%d (期望 1)  add_mod(7,8,10)=%d (期望 5)\n",mod_pow2(1025),add_mod(7,8,10));
    olen=0;
    write_int(-123);
    write_ll(456789012345ll);
    string got(obuf,obuf+olen);
    printf("[fastio] 快写输出 \"%s\" (期望 -123456789012345)\n",got.c_str());
    printf("[fastio] sqr(7)=%d (期望 49)  MIN(3,5)=%d (期望 3)\n",sqr(7),MIN(3,5));
}

int main()
{
    test_builtin();
    test_fastio();
    bench();
    return 0;
}
