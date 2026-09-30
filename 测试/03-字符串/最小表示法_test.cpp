// 最小表示法 的测试与对拍代码
// 模板本体：03-字符串/最小表示法.cpp
#include "../../03-字符串/最小表示法.cpp"

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
    printf("min_show(banana)=%d  min_str=%s\n",min_show("banana"),min_str("banana").c_str());
    printf("min_show(aaaa)=%d  min_show(bbaa)=%d  max_show(bbaa)=%d\n",min_show("aaaa"),min_show("bbaa"),max_show("bbaa"));

    int bad=0;
    for(int rd=1;rd<=2000;rd++)
    {
        int ls=rand()%12+1;
        string a=rand_str(ls);
        // 暴力：n 种循环移位取 min / max
        string wmin="",wmax="";
        int wpmin=0,wpmax=0;
        for(int k=0;k<ls;k++)
        {
            string cur=a.substr(k)+a.substr(0,k);
            if(k==0||cur<wmin)wmin=cur,wpmin=k;
            if(k==0||cur>wmax)wmax=cur,wpmax=k;
        }
        if(min_str(a)!=wmin||min_show(a)!=wpmin)
        {
            bad++;
            if(bad<=3)printf("min mismatch a=%s got=%s@%d want=%s@%d\n",a.c_str(),min_str(a).c_str(),min_show(a),wmin.c_str(),wpmin);
        }
        if(max_show(a)!=wpmax)
        {
            bad++;
            if(bad<=3)printf("max mismatch a=%s got=%d want=%d\n",a.c_str(),max_show(a),wpmax);
        }
    }
    printf("random min_show bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
