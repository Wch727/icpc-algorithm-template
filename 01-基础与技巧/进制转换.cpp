#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// 进制转换：2~36，数字用 0-9 和 A-Z（大小写都收，输出统一大写）
// 十年 OI 一场空，不开 long long 见祖宗：大数一律用字符串存，别塞进 ll
string s;
int a[N];

// 字符 -> 数值，非法返回 -1
int char_to_val(char c)
{
    if(c>='0'&&c<='9')return c-'0';
    if(c>='A'&&c<='Z')return c-'A'+10;
    if(c>='a'&&c<='z')return c-'a'+10;
    return -1;
}

// 数值 -> 字符
char val_to_char(int v)
{
    if(v<10)return (char)('0'+v);
    return (char)('A'+v-10);
}

// 任意进制转十进制：O(len)，溢出返回 -1
// 这就是模拟手算：ans = ans*base + 当前位
// 负数注意：先剥符号，按绝对值算，最后补回符号；别直接对 '-' 取模
bool any_to_dec(const string& t,int base,ll& out)
{
    out=0;
    if(base<2||base>36)return false;
    int i=0,neg=0;
    if(t.size()>0&&(t[0]=='-'||t[0]=='+'))neg=(t[0]=='-'),i=1;// 前导符号单独处理
    if(i>=(int)t.size())return false;
    for(;i<(int)t.size();i++)
    {
        int v=char_to_val(t[i]);
        if(v<0||v>=base)return false;// 该位超出进制，如二进制里出现 2
        out=out*base+v;
    }
    if(neg)out=-out;
    return true;
}

ll any_to_dec(const string& t,int base)// 题目保证合法时的简写
{
    ll res=0;
    return any_to_dec(t,base,res)?res:0;
}

// 十进制转任意进制：O(log_base x)，用取模法倒着取余
// 负数走无符号：u=(unsigned)ll(x) 就是补码，-u 正好是绝对值，LLONG_MIN 也不会溢出
// 千万别写 x=-x 处理 -2^63，那是 UB，开 O2 会被优化掉
string dec_to_any(ll x,int base)
{
    if(base<2||base>36)return "?";
    if(x==0)return "0";// 特判 0，否则循环一次都不进，返回空串
    int neg=0;
    unsigned long long u=(unsigned long long)x;
    if(x<0)neg=1,u=0-u;// 补码取负 = 绝对值，不用怕 -2^63
    string res;
    while(u>0)
    {
        res+=val_to_char((int)(u%(unsigned long long)base));// 低位先出，所以最后要翻
        u/=(unsigned long long)base;
    }
    reverse(res.begin(),res.end());
    if(neg)res="-"+res;
    return res;
}

// string 版本：支持超出 ll 的大数（只处理非负串）
// 每位做一遍长除法，商写回自己，余数就是当前最低位
string big_dec_to_any(const string& t,int base)
{
    if(base<2||base>36)return "?";
    vector<int> d;
    for(char c:t)
    {
        int v=char_to_val(c);
        if(v<0)return "?";
        d.push_back(v);
    }
    int st=0;
    while(st<(int)d.size()&&d[st]==0)st++;// 去掉前导零
    if(st==(int)d.size())return "0";
    string res;
    while(st<(int)d.size())
    {
        int rem=0;
        for(int i=st;i<(int)d.size();i++)
        {
            int cur=rem*10+d[i];// 当前位的被除数
            d[i]=cur/base;
            rem=cur%base;
        }
        res+=val_to_char(rem);
        while(st<(int)d.size()&&d[st]==0)st++;// 商的高位零要跳过
    }
    reverse(res.begin(),res.end());
    return res;
}

// 大数版任意进制转十进制：Horner 展开，逐位乘加
string big_any_to_dec(const string& t,int base)
{
    if(base<2||base>36)return "?";
    vector<int> d;
    for(char c:t)
    {
        int v=char_to_val(c);
        if(v<0||v>=base)return "?";
        d.push_back(v);
    }
    vector<int> res;
    res.push_back(0);
    for(int i=0;i<(int)d.size();i++)
    {
        int carry=d[i];
        for(int j=(int)res.size()-1;j>=0;j--)// 整串乘 base 加 carry
        {
            int cur=res[j]*base+carry;
            res[j]=cur%10;
            carry=cur/10;
        }
        while(carry>0)res.insert(res.begin(),carry%10),carry/=10;// 最高位进位前插
    }
    string out;
    for(int i=0;i<(int)res.size();i++)out+=val_to_char(res[i]);
    int st=0;
    while(st<(int)out.size()-1&&out[st]=='0')st++;
    return out.substr(st);
}

// 两个常用特化：二进制 / 十六进制
string to_bin(ll x)// 负数按补码输出 64 位
{
    string res;
    for(int i=63;i>=0;i--)res+=((x>>i)&1)?'1':'0';
    return res;
}

string to_hex(ll x)// 负数输出 - 加绝对值
{
    if(x==0)return "0";
    if(x<0)return "-"+dec_to_any(-x,16);
    return dec_to_any(x,16);
}

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
    printf("(base36) 1234567890 -> %s (want 3K6QHQE)\n",dec_to_any(1234567890LL,36).c_str());
    printf("to_bin(12) = ...%s to_hex(-255) = %s\n",to_bin(12).substr(60).c_str(),to_hex(-255).c_str());
    return 0;
}
