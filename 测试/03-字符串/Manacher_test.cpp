// Manacher 的测试与对拍代码
// 模板本体：03-字符串/Manacher.cpp
#include "../../03-字符串/Manacher.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int brute_longest(const string &str)
{
    int len=str.length(),ans=0;
    for(int i=0;i<len;i++)
    for(int j=i;j<len;j++)
    {
        bool ok=true;
        for(int x=i,y=j;x<y;x++,y--)if(str[x]!=str[y]){ok=false;break;}
        if(ok&&j-i+1>ans)ans=j-i+1;
    }
    return ans;
}

ll brute_count(const string &str)
{
    int len=str.length();
    ll ans=0;
    for(int i=0;i<len;i++)
    for(int j=i;j<len;j++)
    {
        bool ok=true;
        for(int x=i,y=j;x<y;x++,y--)if(str[x]!=str[y]){ok=false;break;}
        if(ok)ans++;
    }
    return ans;
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        string x("!#%\0%#!",7); M.build(x);
        assert(M.longest()==7); M.build(""); assert(M.longest()==0&&M.count_pal()==0);
    }

    srand(12345);

    s="abacaba";
    M.build(s);
    printf("s=%s  longest=%d (brute=%d)  count=%lld (brute=%lld)\n",
        s.c_str(),M.longest(),brute_longest(s),M.count_pal(),brute_count(s));

    int bad=0;
    for(int rd=1;rd<=3000;rd++)
    {
        int len=rand()%13+1;
        string str=rand_str(len);
        M.build(str);
        int a=M.longest(),b=brute_longest(str);
        ll c=M.count_pal(),dd=brute_count(str);
        if(a!=b||c!=dd)
        {
            bad++;
            if(bad<=3)printf("mismatch s=%s longest %d/%d count %lld/%lld\n",str.c_str(),a,b,c,dd);
        }
    }
    printf("random manacher bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
