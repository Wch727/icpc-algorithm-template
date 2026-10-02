// 字符串哈希 的测试与对拍代码
// 模板本体：03-字符串/字符串哈希.cpp
#include "../../03-字符串/字符串哈希.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%2);
    return r;
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        string x; x.push_back(char(255)); x.push_back(char(128));
        H2.build(x); assert(H2.get(1,1).first==255&&H2.get(2,2).first==128);
    }

    srand(12345);

    // 基础自测：子串哈希与暴力比较
    s="abacababca";
    H1.build(s);
    H2.build(s);
    printf("s=%s\n",s.c_str());
    printf("get(3,6)=%llu  get(3,6)double=(%llu,%llu)\n",H1.get(3,6),H2.get(3,6).first,H2.get(3,6).second);

    // 多轮随机对拍：相等子串哈希必相等，不等子串哈希必不等
    int bad=0;
    for(int rd=1;rd<=2000;rd++)
    {
        int len=rand()%12+1;
        string str=rand_str(len);
        H1.build(str);
        H2.build(str);
        for(int l=1;l<=len;l++)
        for(int r=l;r<=len;r++)
        for(int x=1;x<=len;x++)
        for(int y=x;y<=len;y++)
        {
            bool same=(str.substr(l-1,r-l+1)==str.substr(x-1,y-x+1));
            if((H1.get(l,r)==H1.get(x,y))!=same)bad++;
            if((H2.get(l,r)==H2.get(x,y))!=same)bad++;
        }
    }
    printf("random sub-hash check bad=%d\n",bad);

    // 用法示例：P3370 不同字符串个数
    set<pair<ull,ull>> st;
    for(int i=1;i<=1000;i++)st.insert(H2.get(1,i%17+1));
    printf("distinct hash count=%d\n",(int)st.size());

    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
