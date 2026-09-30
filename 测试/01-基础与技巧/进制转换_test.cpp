// 进制转换 的测试与对拍代码
// 模板本体：01-基础与技巧/进制转换.cpp
#include "../../01-基础与技巧/进制转换.cpp"

// 暴力：一位一位地除（不翻转也逐位存），用来和 dec_to_any 对拍

string brute_dec_to_any(ll x,int base)
{
    string res;
    while(x>0)res+=val_to_char((int)(x%base)),x/=base;
    reverse(res.begin(),res.end());
    return res;
}

// 暴力：任意进制串转十进制，weight 连乘
ll brute_any_to_dec(const string& t,int base)
{
    ll res=0;
    for(char c:t)res=res*base+char_to_val(c);
    return res;
}

int main()
{
    srand(20240530);
    // 自测1：dec_to_any 与暴力对拍（ll 范围非负，防止 long long 溢出）
    for(int t=1;t<=200000;t++)
    {
        int base=rand()%35+2;
        ll x=(ll)rand()<<31|rand();
        string got=dec_to_any(x,base);
        if(got!=brute_dec_to_any(x,base))
        {
            printf("fail dec_to_any t=%d base=%d x=%lld got=%s\n",t,base,x,got.c_str());
            return 0;
        }
    }
    printf("dec_to_any self-check OK\n");

    // 自测2：any_to_dec 与暴力对拍
    for(int t=1;t<=200000;t++)
    {
        int base=rand()%35+2;
        ll x=(ll)(((unsigned)rand()<<15|rand())&0x7fffffffLL);
        string str=dec_to_any(x,base);
        if(any_to_dec(str,base)!=brute_any_to_dec(str,base)||any_to_dec(str,base)!=x)
        {
            printf("fail any_to_dec t=%d base=%d str=%s\n",t,base,str.c_str());
            return 0;
        }
    }
    printf("any_to_dec self-check OK\n");

    // 自测3：双向互转闭环 dec->any->dec
    for(int t=1;t<=100000;t++)
    {
        int base=rand()%35+2;
        ll x=(ll)(((unsigned)rand()<<15|rand())&0x3fffffffLL);
        if(any_to_dec(dec_to_any(x,base),base)!=x)
        {
            printf("fail roundtrip base=%d x=%lld\n",base,x);
            return 0;
        }
    }
    printf("roundtrip self-check OK\n");

    // 自测4：边界值 0 / 负数 / 进制端点
    if(dec_to_any(0,2)!="0"||dec_to_any(0,36)!="0")
    {
        printf("fail zero\n");
        return 0;
    }
    ll corner[10]={0,1,-1,35,-35,36,2147483647,-2147483647,LLONG_MAX,LLONG_MIN};
    for(int i=0;i<10;i++)
    {
        ll x=corner[i];
        for(int base=2;base<=36;base++)
        {
            string got=dec_to_any(x,base);
            ll back=0;
            if(!any_to_dec(got,base,back)||back!=(ll)x)
            {
                printf("fail corner x=%lld base=%d got=%s\n",x,base,got.c_str());
                return 0;
            }
        }
    }
    printf("corner self-check OK\n");

    // 自测5：大数版与 ll 版结果必须一致
    for(int t=1;t<=20000;t++)
    {
        int base=rand()%35+2;
        ll x=(ll)rand()<<20|rand();
        string dec=big_dec_to_any(dec_to_any(x,10),base);
        if(dec!=dec_to_any(x,base))
        {
            printf("fail big_dec_to_any base=%d x=%lld got=%s\n",base,x,dec.c_str());
            return 0;
        }
        string b36=dec_to_any(x,base);
        if(big_any_to_dec(b36,base)!=dec_to_any(x,10))
        {
            printf("fail big_any_to_dec base=%d x=%lld str=%s\n",base,x,b36.c_str());
            return 0;
        }
    }
    printf("big number self-check OK\n");

    // 自测6：超 ll 的大数（2^100-1 = 31 个 1）转十进制
    string big(100,'1');// 100 个 1 的二进制
    printf("2^100-1 = %s (want 1267650600228229401496703205375)\n",big_any_to_dec(big,2).c_str());
    printf("1267650600228229401496703205375 -> base36 = %s\n",big_dec_to_any("1267650600228229401496703205375",36).c_str());

    // 自测7：非法输入与非法进制
    ll tmp=0;
    if(any_to_dec("2",2,tmp)||any_to_dec("1G",16,tmp)||any_to_dec("",10,tmp)||any_to_dec("-",10,tmp))
    {
        printf("fail illegal input not rejected\n");
        return 0;
    }
    if(dec_to_any(10,37)!="?"||dec_to_any(10,1)!="?")
    {
        printf("fail illegal base not rejected\n");
        return 0;
    }
    printf("illegal input self-check OK\n");

    // 自测8：套题演示（洛谷 P1143 进制转换 / P1604 之类都能直接套）
    // 样例：(2, "1011") -> 11；(16, 255) -> FF
    printf("(2,1011) -> %lld (want 11)\n",any_to_dec("1011",2));
    printf("(255,16) -> %s (want FF)\n",dec_to_any(255,16).c_str());
    printf("(-255,16) -> %s (want -FF)\n",dec_to_any(-255,16).c_str());
    printf("(base36) 1234567890 -> %s (want KF12OI)\n",dec_to_any(1234567890LL,36).c_str());
    printf("to_bin(12) = ...%s to_hex(-255) = %s\n",to_bin(12).substr(60).c_str(),to_hex(-255).c_str());
    return 0;
}
