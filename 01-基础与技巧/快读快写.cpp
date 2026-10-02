#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 沿用 D:\code 中的 readd/print；按已知数量读入，输入须合法且在 ll 范围内。
inline ll readd()
{
    int c;
    bool flag=false;
    while((c=getchar())>'9'||c<'0')
    {
        if(c==EOF)return 0;
        if(c=='-')flag=true;
    }
    unsigned long long res=c-'0';
    while((c=getchar())>='0'&&c<='9')res=(res<<3)+(res<<1)+c-'0';
    if(flag)return res==(1ULL<<63)?LLONG_MIN:-(ll)res;
    return (ll)res;
}

void print(ll x)
{
    if(x<0)
    {
        putchar('-');
        if(x<-9)print(-(x/10)); // 先除再取负，兼容 LLONG_MIN
        putchar('0'-x%10);
    }
    else
    {
        if(x>9)print(x/10);
        putchar(x%10+'0');
    }
}
// 用法：n=readd(); print(ans); putchar('\n'); 不自动输出空格或换行。
// 无锁版本可按平台替换 getchar/putchar，参见 D:\code\luogu\P10815.cpp。
