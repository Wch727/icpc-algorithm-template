#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000005;
int n,m;
string s;

// sa[i] 排名第 i 的后缀起点，rk[i] 后缀 i 的排名，height[i]=lcp(sa[i-1],sa[i])
int sa[N],rk[N],height[N];
int cnt[N],xs[N],yr[N],tmp[N];

// 倍增 + 基数排序，O(n log n)；字符集按 ASCII 处理，数字/大小写字母都行
// 传入 0 下标字符串首地址，函数内只用局部长度 len，顺手回填全局 n
// 调用前保证 str 末尾有 '\0'（空串请直接特判返回）
void build_sa(char *str)
{
    int len=strlen(str);
    n=len;
    if(len==0)return;
    int mx=max(len,300);
    for(int i=0;i<=mx;i++)cnt[i]=0;
    for(int i=0;i<len;i++)cnt[(unsigned char)str[i]]++;
    for(int i=1;i<=mx;i++)cnt[i]+=cnt[i-1];
    for(int i=len-1;i>=0;i--)sa[--cnt[(unsigned char)str[i]]]=i;
    int p=1;
    rk[sa[0]]=0;
    for(int i=1;i<len;i++)
    {
        if(str[sa[i]]!=str[sa[i-1]])p++;
        rk[sa[i]]=p-1;
    }
    for(int k=1;k<len&&p<len;k<<=1)
    {
        // 第二关键字排序：后半段为空的后缀排最前，其余按上一轮 rk 顺序
        int num=0;
        for(int i=len-k;i<len;i++)yr[num++]=i;
        for(int i=0;i<len;i++)if(sa[i]>=k)yr[num++]=sa[i]-k;
        for(int i=0;i<=p;i++)cnt[i]=0;
        for(int i=0;i<len;i++)cnt[rk[yr[i]]]++;
        for(int i=1;i<=p;i++)cnt[i]+=cnt[i-1];
        for(int i=len-1;i>=0;i--)sa[--cnt[rk[yr[i]]]]=yr[i];
        // 用 (第一关键字, 第二关键字) 重算 rk
        p=1;
        tmp[sa[0]]=0;
        for(int i=1;i<len;i++)
        {
            int a=sa[i-1],b=sa[i];
            int a2=(a+k<len)?rk[a+k]:-1;
            int b2=(b+k<len)?rk[b+k]:-1;
            if(rk[a]!=rk[b]||a2!=b2)p++;
            tmp[b]=p-1;
        }
        for(int i=0;i<len;i++)rk[i]=tmp[i];
    }
    // height 用结论 height[rk[i]]>=height[rk[i-1]]-1，O(n)
    int k=0;
    for(int i=0;i<len;i++)
    {
        if(rk[i]==0)
        {
            k=0;
            height[0]=0;
            continue;
        }
        if(k)k--;
        int j=sa[rk[i]-1];
        while(i+k<len&&j+k<len&&str[i+k]==str[j+k])k++;
        height[rk[i]]=k;
    }
}

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
