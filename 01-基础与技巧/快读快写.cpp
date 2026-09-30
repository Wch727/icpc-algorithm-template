#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// fread 快速读入 + 输出缓冲，O(字节数)
const int BUF=1<<20;// 1MB 输入/输出缓冲区
char ibuf[BUF],obuf[BUF];
int ipos,iend,opos;

inline int get_char()// 返回 -1 表示文件读完
{
    if(ipos==iend)
    {
        iend=(int)fread(ibuf,1,BUF,stdin);
        ipos=0;
        if(iend<=0)return -1;
    }
    return (int)(unsigned char)ibuf[ipos++];
}

inline ll read_long()// 读入 long long，自动处理负号
{
    int c=get_char();
    while(c!='-'&&(c<'0'||c>'9'))// 跳过空白等非数字
    {
        if(c==-1)return 0;
        c=get_char();
    }
    bool flag=false;
    if(c=='-')flag=true,c=get_char();
    ll res=c-'0';
    while((c=get_char())>='0'&&c<='9')res=(res<<3)+(res<<1)+c-'0';
    return flag?-res:res;
}

inline int read_int()// 读入 int
{
    return (int)read_long();
}

inline void read_str(char* s)// 读入一个不含空白的字符串
{
    int c=get_char();
    while(c==' '||c=='\n'||c=='\r'||c=='\t')c=get_char();
    int len=0;
    while(c!=-1&&c!=' '&&c!='\n'&&c!='\r'&&c!='\t')s[len++]=(char)c,c=get_char();
    s[len]='\0';
}

inline void put_char(char c)
{
    if(opos==BUF)
    {
        fwrite(obuf,1,opos,stdout);
        opos=0;
    }
    obuf[opos++]=c;
}

inline void write_long(ll x)// 输出 long long
{
    if(x<0)put_char('-'),x=-x;
    if(x==0)
    {
        put_char('0');
        return;
    }
    char tmp[24];
    int len=0;
    while(x>0)tmp[len++]=(char)('0'+x%10),x/=10;
    while(len>0)put_char(tmp[--len]);
}

inline void write_char(char c)// 配套输出字符/换行
{
    put_char(c);
}

inline void write_str(const char* s)
{
    for(int i=0;s[i];i++)put_char(s[i]);
}

inline void flush_out()// main 结束前必须调用
{
    if(opos>0)
    {
        fwrite(obuf,1,opos,stdout);
        opos=0;
    }
}

int main()
{
    // 自测：把数据写进临时文件再 freopen，这样 fread 那条路径真的被走一遍
    const char* data="123 -456 1000000000000 789 hello\n";
    bool ok=false;
    FILE* fp=fopen("io_selftest_tmp.txt","w");
    if(fp)
    {
        fputs(data,fp);
        fclose(fp);
        if(freopen("io_selftest_tmp.txt","r",stdin)!=NULL)ok=true;// 必须在第一次 fread 前 freopen
    }
    if(ok)ipos=iend=0;// 清掉可能已经缓冲的数据
    if(!ok)// 写文件失败就退化成内存缓冲自测，逻辑不变
    {
        int len=(int)strlen(data);
        for(int i=0;i<len;i++)ibuf[i]=data[i];
        ipos=-1,iend=len;
        printf("use memory buffer\n");
    }
    ll a=read_long();
    int b=read_int();
    ll c=read_long();
    int d=read_int();
    char s[100];
    read_str(s);
    write_char('[');
    write_long(a),write_char(' ');
    write_long(b),write_char(' ');
    write_long(c),write_char(' ');
    write_long(d),write_char(' ');
    write_str(s);
    write_str("]\n");
    ll sum=a+b+c+d;
    write_str("sum="),write_long(sum),write_char('\n');
    write_str("neg="),write_long(-sum),write_char('\n');
    write_str("zero="),write_long(0),write_char('\n');
    flush_out();
    remove("io_selftest_tmp.txt");
    return 0;
}
