// AC自动机 的测试与对拍代码
// 模板本体：03-字符串/AC自动机.cpp
#include "../../03-字符串/AC自动机.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int main()
{
    // 每个前缀枚举全部非空子集，独立核对最大 LCP 与异或累计值。
    for(vector<string> a:{vector<string>{"a","a","ab","abc","b","ab"},
                         vector<string>{"abcd","ab","abc","abcd","z","za"}})
    {
        PrefixLCP b;int r=0;
        for(const string &s:a)
        {
            ll got=b.insert(s);++r;
            vector<int> best(r+1);
            for(int mask=1;mask<(1<<r);mask++)
            {
                string p;bool first=true;
                for(int i=0;i<r;i++)if(mask>>i&1)
                {
                    if(first){p=a[i];first=false;}
                    else {int k=0;while(k<(int)min(p.size(),a[i].size())&&p[k]==a[i][k])++k;p.resize(k);}
                }
                int j=__builtin_popcount((unsigned)mask);
                best[j]=max(best[j],(int)p.size());
            }
            ll want=0;
            for(int j=1;j<=r;j++){assert(b.f[j]==best[j]);want+=best[j]^j;}
            assert(got==want);
        }
    }
    srand(12345);

    // 基础自测：模式串 he/she/his/hers，文本 ushershe
    clear_trie();
    vector<string> base={"he","she","his","hers"};
    for(int i=0;i<(int)base.size();i++)id[i]=insert(base[i]);
    build_fail();
    string txt="ushershe";
    printf("text=%s  total=%lld\n",txt.c_str(),query(txt));
    for(int i=0;i<(int)base.size();i++)
    {
        int c=0;
        for(int p=0;p+(int)base[i].length()<=(int)txt.length();p++)
            if(txt.substr(p,base[i].length())==base[i])c++;
        printf("  pattern %-5s appear=%d\n",base[i].c_str(),c);
    }

    int bad=0;
    for(int rd=1;rd<=1500;rd++)
    {
        clear_trie();
        int k=rand()%5+1;
        vector<string> pat;
        for(int i=1;i<=k;i++)
        {
            string p=rand_str(rand()%4+1);
            pat.push_back(p);
            id[i]=insert(p);//记录每个模式串的终止结点
        }
        build_fail();
        string tx=rand_str(rand()%12+1);
        ll got=query(tx),want=0;
        // 暴力：枚举每个模式串在文本中的每次出现
        for(int i=0;i<k;i++)
        {
            int pl=pat[i].length();
            for(int p=0;p+pl<=(int)tx.length();p++)
                if(tx.substr(p,pl)==pat[i])want++;
        }
        if(got!=want)
        {
            bad++;
            if(bad<=3)printf("mismatch text=%s got=%lld want=%lld\n",tx.c_str(),got,want);
        }
    }
    printf("random AC automaton bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
