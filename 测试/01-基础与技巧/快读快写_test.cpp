// 快读快写 的测试与对拍代码
// 模板本体：01-基础与技巧/快读快写.cpp
#include "../../01-基础与技巧/快读快写.cpp"

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
