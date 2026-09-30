// 后缀数组 的测试与对拍代码
// 模板本体：03-字符串/后缀数组.cpp
#include "../../03-字符串/后缀数组.cpp"

string rand_str(int len)
{
    string r="";
    for(int i=1;i<=len;i++)r+=(char)('a'+rand()%3);
    return r;
}

int main()
{
    srand(12345);

    // 基础自测：手算例子 banana
    s="banana";
    string keep=s;
    build_sa(&s[0]);
    printf("s=%s  n=%d\n",keep.c_str(),n);
    printf("sa:");
    for(int i=0;i<n;i++)printf(" %d",sa[i]);
    printf("\nheight:");
    for(int i=1;i<n;i++)printf(" %d",height[i]);
    printf("\nreadable sa:");
    for(int i=0;i<n;i++)printf(" %s",keep.substr(sa[i]).c_str());
    printf("\n");

    int bad=0;
    for(int rd=1;rd<=800;rd++)
    {
        int len=rand()%30+1;
        string str=rand_str(len);
        vector<int> want;
        for(int i=0;i<len;i++)want.push_back(i);
        sort(want.begin(),want.end(),[&](int a,int b){return str.substr(a)<str.substr(b);});
        string buf=str;
        buf.push_back('\0');// 保证 build_sa 里的 strlen 正确
        build_sa(&buf[0]);
        for(int i=0;i<len;i++)if(sa[i]!=want[i])bad++;
        for(int i=1;i<len;i++)
        {
            int c=0;
            while(want[i-1]+c<len&&want[i]+c<len&&str[want[i-1]+c]==str[want[i]+c])c++;
            if(height[i]!=c)bad++;
        }
    }
    printf("random suffix array bad=%d\n",bad);
    printf("%s\n",bad==0?"ALL OK":"FAILED");
    return 0;
}
