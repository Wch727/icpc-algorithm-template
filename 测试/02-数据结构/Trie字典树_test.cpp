// Trie字典树 的测试与对拍代码
// 模板本体：02-数据结构/Trie字典树.cpp
#include "../../02-数据结构/Trie字典树.cpp"

// 暴力：把所有串存下来，逐个比较，用于对拍

bool brute_exist(const vector<string> &v,const string &s)
{
    for(int i=0;i<(int)v.size();i++)if(v[i]==s)return true;
    return false;
}
int brute_count_word(const vector<string> &v,const string &s)
{
    int c=0;
    for(int i=0;i<(int)v.size();i++)if(v[i]==s)c++;
    return c;
}
int brute_count_pre(const vector<string> &v,const string &s)
{
    int c=0;
    for(int i=0;i<(int)v.size();i++)
    {
        const string &x=v[i];
        if(x.length()>=s.length()&&x.compare(0,s.length(),s)==0)c++;
    }
    return c;
}

// 随机小写串，长度 1~4
string rand_str()
{
    int len=rand()%4+1;
    string res;
    for(int i=1;i<=len;i++)res+=(char)('a'+rand()%3);
    return res;
}

int main()
{
    // 审核回归：覆盖原随机小值测试遗漏的边界。
    {
        t.clear(); t.insert(""); t.insert("a"); t.insert("ab");
        assert(t.count_pre("")==3&&t.count_word("")==1); t.clear();
    }

    srand(20240513);

    // 1. 小数据手测：a / ab / abc / ab / abd
    t.clear();
    t.insert("a"),t.insert("ab"),t.insert("abc"),t.insert("ab"),t.insert("abd");
    printf("exist(a)=%d exist(ab)=%d exist(abcd)=%d\n",(int)t.exist("a"),(int)t.exist("ab"),(int)t.exist("abcd"));
    printf("word(ab)=%d word(a)=%d word(abd)=%d\n",t.count_word("ab"),t.count_word("a"),t.count_word("abd"));
    printf("pre(a)=%d pre(ab)=%d pre(abc)=%d pre(abz)=%d\n",t.count_pre("a"),t.count_pre("ab"),t.count_pre("abc"),t.count_pre("abz"));

    // 2. 随机多轮对拍：插入 + 三种查询 与暴力比较
    bool ok=true;
    for(int T=1;T<=20&&ok;T++)
    {
        t.clear();
        vector<string> v;
        n=rand()%12+1;
        for(int i=1;i<=n;i++)
        {
            s=rand_str();
            t.insert(s),v.push_back(s);
        }
        for(int q=1;q<=30;q++)
        {
            s=rand_str();
            if(t.exist(s)!=brute_exist(v,s)){ok=false;break;}
            if(t.count_word(s)!=brute_count_word(v,s)){ok=false;break;}
            if(t.count_pre(s)!=brute_count_pre(v,s)){ok=false;break;}
        }
    }
    printf("random %s\n",ok?"all passed":"FAILED");

    // 3. 大写 + 数字也走一遍，验证字符映射
    t.clear();
    t.insert("A0"),t.insert("A0"),t.insert("A0b");
    printf("A0: exist=%d word=%d pre(A0)=%d\n",(int)t.exist("A0"),t.count_word("A0"),t.count_pre("A0"));

    // 样例：3 个串 ab aa abc，查询 ab / aa / abc / a
    // 输出：exist=1/1/1/0，pre(a)=3
    return 0;
}
