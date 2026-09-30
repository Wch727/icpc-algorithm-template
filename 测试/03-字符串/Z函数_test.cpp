// Z函数 的测试与对拍代码
// 模板本体：03-字符串/Z函数.cpp
#include "../../03-字符串/Z函数.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int brute_z(const string &str,int i)
{
    int len=str.length(),c=0;
    while(i+c<len&&str[c]==str[i+c])c++;
    return c;
}

int ex[N];

int main()
{
    srand(12345);

    s="aabaaab";
    get_z(s);
    printf("s=%s  z[1..%d]:",s.c_str(),(int)s.length()-1);
    for(int i=1;i<(int)s.length();i++)printf(" %d",z[i]);
    printf("\n");

    int bad=0;
    for(int rd=1;rd<=3000;rd++)
    {
        int len=rand()%16+1;
        string str=rand_str(len);
        get_z(str);
        for(int i=1;i<len;i++)if(z[i]!=brute_z(str,i))bad++;
        // 顺带对拍扩展 KMP
        string txt=rand_str(rand()%16+1),pat=rand_str(rand()%8+1);
        get_ex(txt,pat,ex);
        for(int i=0;i<(int)txt.length();i++)
        {
            int c=0;
            while(i+c<(int)txt.length()&&c<(int)pat.length()&&txt[i+c]==pat[c])c++;
            if(ex[i]!=c)bad++;
        }
    }
    printf("random z + exkmp bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
