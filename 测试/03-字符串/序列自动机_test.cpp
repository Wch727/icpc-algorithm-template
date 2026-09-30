// 序列自动机 的测试与对拍代码
// 模板本体：03-字符串/序列自动机.cpp
#include "../../03-字符串/序列自动机.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%2);// 只用 a,b 制造大量重复
    return r;
}

int main()
{
    srand(12345);

    // 基础自测
    s="ababc";
    build_seq(s);
    printf("s=%s  is_subseq(abc)=%d  is_subseq(aac)=%d  is_subseq(cba)=%d\n",s.c_str(),(int)is_subseq("abc"),(int)is_subseq("aac"),(int)is_subseq("cba"));
    printf("diff_subseq(ababc)=%lld (含空串)\n",count_diff_subseq());

    int bad=0;
    for(int rd=1;rd<=1200;rd++)
    {
        int ls=rand()%12+1;
        s=rand_str(ls);
        build_seq(s);

        // 暴力 1：枚举 2^ls 个子序列放进 set
        set<string> st;
        for(int mask=0;mask<(1<<ls);mask++)
        {
            string cur="";
            for(int i=0;i<ls;i++)if(mask>>i&1)cur+=s[i];
            st.insert(cur);
        }
        if(count_diff_subseq()!=(ll)st.size())
        {
            bad++;
            if(bad<=3)printf("diff_subseq mismatch s=%s got=%lld want=%d\n",s.c_str(),count_diff_subseq(),(int)st.size());
        }

        // 暴力 2：随机小串判断子序列
        for(int q=1;q<=5;q++)
        {
            int lt=rand()%5+1;
            string t=rand_str(lt);
            bool want=false;
            for(int mask=0;mask<(1<<ls)&&!want;mask++)
            {
                string cur="";
                for(int i=0;i<ls;i++)if(mask>>i&1)cur+=s[i];
                if(cur==t)want=true;
            }
            if(is_subseq(t)!=want)
            {
                bad++;
                if(bad<=3)printf("subseq mismatch s=%s t=%s got=%d want=%d\n",s.c_str(),t.c_str(),(int)is_subseq(t),(int)want);
            }
        }
    }
    printf("random seq automaton bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
