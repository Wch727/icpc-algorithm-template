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
