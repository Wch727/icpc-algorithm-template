// 后缀自动机SAM 的测试与对拍代码
// 模板本体：03-字符串/后缀自动机SAM.cpp
#include "../../03-字符串/后缀自动机SAM.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%2);// 只用 a,b 制造大量重复
    return r;
}

int main()
{
    srand(12345);

    // 基础自测：手算例子 ababa
    sam_init();
    string base="ababa";
    n=base.length();
    for(int i=0;i<n;i++)sam_extend(base[i]-'a');
    sam_build();
    printf("s=%s  states=%d  diff_substr=%lld\n",base.c_str(),tot,count_diff_substr());
    printf("occ of \"ab\"=%d  occ of \"ba\"=%d\n",siz[nxt[nxt[1][0]][1]],siz[nxt[nxt[1][1]][0]]);
    printf("lcs(ababa, babab)=%d\n",longest_common("babab"));

    int bad=0;
    for(int rd=1;rd<=800;rd++)
    {
        int ls=rand()%12+1,lt=rand()%8+1;
        string a=rand_str(ls),b=rand_str(lt);
        sam_init();
        n=ls;
        for(int i=0;i<ls;i++)sam_extend(a[i]-'a');
        sam_build();

        // 暴力 1：所有子串丢进 set
        set<string> st;
        for(int i=0;i<ls;i++)
        {
            string cur="";
            for(int j=i;j<ls;j++)cur+=a[j],st.insert(cur);
        }
        if(count_diff_substr()!=(ll)st.size()){bad++;if(bad<=3)printf("diff_substr mismatch a=%s\n",a.c_str());}

        // 暴力 2：每个子串的出现次数
        for(int i=0;i<ls;i++)
        {
            string cur="";
            for(int j=i;j<ls;j++)
            {
                cur+=a[j];
                int p=1;
                for(int k=0;k<(int)cur.length();k++)p=nxt[p][cur[k]-'a'];
                int want=0;
                for(int k=0;k+(int)cur.length()<=ls;k++)if(a.substr(k,cur.length())==cur)want++;
                if(siz[p]!=want){bad++;if(bad<=3)printf("occ mismatch a=%s sub=%s got=%d want=%d\n",a.c_str(),cur.c_str(),siz[p],want);}
            }
        }

        // 暴力 3：最长公共子串
        int want=0;
        for(int i=0;i<ls;i++)
            for(int j=i;j<ls;j++)
            {
                string cur=a.substr(i,j-i+1);
                if(b.find(cur)!=string::npos)want=max(want,(int)cur.length());
            }
        if(longest_common(b)!=want){bad++;if(bad<=3)printf("lcs mismatch a=%s b=%s got=%d want=%d\n",a.c_str(),b.c_str(),longest_common(b),want);}
    }
    printf("random SAM bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
