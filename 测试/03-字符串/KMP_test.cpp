// KMP 的测试与对拍代码
// 模板本体：03-字符串/KMP.cpp
#include "../../03-字符串/KMP.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int brute_kmp(const string &s,const string &p)
{
    int l1=s.length(),l2=p.length(),cnt=0;
    if(l2==0||l1<l2)return 0;
    for(int i=0;i+l2<=l1;i++)if(s.substr(i,l2)==p)cnt++;
    return cnt;
}

int main()
{
    srand(12345);

    // 基础自测
    s="abababa",t="aba";
    get_nxt(t);
    printf("t=%s  nxt[1..%d]:",t.c_str(),(int)t.length());
    for(int i=1;i<=(int)t.length();i++)printf(" %d",nxt[i]);
    printf("\n");
    printf("count(abababa,aba)=%d (brute=%d)\n",kmp(s,t),brute_kmp(s,t));
    printf("min_cycle(abcab)=%d\n",min_cycle("abcab"));

    int bad=0;
    // 多轮随机对拍：出现次数
    for(int rd=1;rd<=3000;rd++)
    {
        int l1=rand()%14+1,l2=rand()%6+1;
        string a=rand_str(l1),b=rand_str(l2);
        get_nxt(b);
        int x=kmp(a,b),y=brute_kmp(a,b);
        if(x!=y)
        {
            bad++;
            if(bad<=3)printf("mismatch s=%s p=%s got=%d want=%d\n",a.c_str(),b.c_str(),x,y);
        }
    }
    // 多轮随机对拍：最小循环节（c 整除 len 且 len%c 为循环节位数，且没有更小的）
    for(int rd=1;rd<=3000;rd++)
    {
        int len=rand()%14+1;
        string b=rand_str(len);
        int c=min_cycle(b);
        if(c<1||c>len||len%c!=0)bad++;
        else for(int i=0;i<len;i++)if(b[i]!=b[i%c])bad++;
        for(int k=1;k<c;k++)
        {
            if(len%k)continue;
            bool ok=true;
            for(int i=0;i<len;i++)if(b[i]!=b[i%k])ok=false;
            if(ok)bad++;//存在更小的完整循环节
        }
    }
    printf("random kmp + min_cycle bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
