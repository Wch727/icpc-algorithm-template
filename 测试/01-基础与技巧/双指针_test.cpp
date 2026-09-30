// 双指针 的测试与对拍代码
// 模板本体：01-基础与技巧/双指针.cpp
#include "../../01-基础与技巧/双指针.cpp"

int brute(const string &s,const string &t)
{
    if(t.empty())return 0;
    int ans=INT_MAX;
    for(int l=0;l<(int)s.size();l++)
        for(int r=l;r<(int)s.size();r++)
        {
            int cnt[256]={0};
            for(int i=l;i<=r;i++)cnt[(unsigned char)s[i]]++;
            bool ok=true;
            for(unsigned char c:t)if(--cnt[c]<0)ok=false;
            if(ok)ans=min(ans,r-l+1);
        }
    return ans==INT_MAX?-1:ans;
}

int main()
{
    srand(19260817);
    bool ok=min_cover("ADOBECODEBANC","ABC")==4;
    for(int t=1;t<=100;t++)
    {
        vector<int> a(rand()%20);
        for(int &x:a)x=rand()%31-15;
        sort(a.begin(),a.end());
        int target=rand()%41-20;
        bool found=false;
        for(int i=0;i<(int)a.size();i++)
            for(int j=i+1;j<(int)a.size();j++)if(a[i]+a[j]==target)found=true;
        pair<int,int> p=two_sum(a,target);
        if((p.first>=0)!=found)ok=false;
        if(p.first>=0&&(p.first>=p.second||a[p.first]+a[p.second]!=target))ok=false;
        string s,u;
        for(int i=0,n=rand()%15;i<n;i++)s+=char('a'+rand()%4);
        for(int i=0,n=rand()%6;i<n;i++)u+=char('a'+rand()%4);
        if(min_cover(s,u)!=brute(s,u))ok=false;
    }
    printf("双指针 %s\n",ok?"OK":"FAILED");
    if(!ok)return 1;
    return 0;
}
