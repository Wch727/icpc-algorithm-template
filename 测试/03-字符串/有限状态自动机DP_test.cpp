// 有限状态自动机DP 的测试与对拍代码
// 模板本体：03-字符串/有限状态自动机DP.cpp
#include "../../03-字符串/有限状态自动机DP.cpp"

// 独立暴力：枚举所有串，再用 string::find 检查，不调用自动机转移。

ll brute(int n,int sig,const vector<string> &pat,ll mod)
{
    ll top=1,ans=0;
    for(int i=1;i<=n;i++)top*=sig;//只用于小 n 自测
    for(ll mask=0;mask<top;mask++)
    {
        ll x=mask;
        string s(n,'a');
        for(int i=0;i<n;i++)s[i]+=(char)(x%sig),x/=sig;
        bool ok=true;
        for(int i=0;i<(int)pat.size();i++)
            if(s.find(pat[i])!=string::npos)
            {
                ok=false;
                break;
            }
        if(ok)ans=(ans+1)%mod;
    }
    return ans;
}

bool check(int n,int sig,const vector<string> &pat,ll mod)
{
    ac.init(sig);
    for(int i=0;i<(int)pat.size();i++)ac.insert(pat[i]);
    ac.build();
    ac.build();//重复建图应无影响
    ll x=ac.count(n,mod),y=brute(n,sig,pat,mod);
    cnt++;
    if(x==y)return true;
    printf("FAIL n=%d sig=%d mod=%lld got=%lld want=%lld\n",n,sig,mod,x,y);
    for(int i=0;i<(int)pat.size();i++)printf("pattern=[%s]\n",pat[i].c_str());
    return false;
}

// 无禁串时答案为 sig^n；此处大模数验证中间加法不溢出。
ll power(int a,int n,ll mod)
{
    ll ans=1%mod;
    for(int i=1;i<=n;i++)ans=(ll)((__int128)ans*a%mod);
    return ans;
}

int main()
{
    // 固定边界：空串、空禁串、所有字母禁用、重复禁串、后缀 fail 命中。
    vector<vector<string> > fixed={{},{""},{"a","b"},{"a","a"},
        {"b","aba"},{"ab","bab"},{"aaaaaaaaaa"},{"aa","aaa"}};
    ll mods[]={1,2,97,1000000007,LLONG_MAX};
    for(int t=0;t<(int)fixed.size();t++)
        for(int n=0;n<=8;n++)
            for(int j=0;j<5;j++)
                if(!check(n,2,fixed[t],mods[j]))return 1;

    // 穷举二字母、长度至多 2 的模式集合，共 64 种。
    vector<string> all={"a","b","aa","ab","ba","bb"};
    for(int mask=0;mask<(1<<6);mask++)
    {
        vector<string> pat;
        for(int i=0;i<6;i++)if(mask&(1<<i))pat.push_back(all[i]);
        for(int n=0;n<=8;n++)
            for(int j=0;j<4;j++)
                if(!check(n,2,pat,mods[j]))return 1;
    }

    // 固定种子随机对拍，改变字母表、长度、模式与模数，并反复清空重建。
    for(int t=1;t<=3000;t++)
    {
        int sig=rnd()%3+1,n=rnd()%9,k=rnd()%6;
        vector<string> pat;
        for(int i=1;i<=k;i++)
        {
            int len=rnd()%6;
            string s(len,'a');
            for(int j=0;j<len;j++)s[j]+=(char)(rnd()%sig);
            pat.push_back(s);
        }
        if(!check(n,sig,pat,mods[rnd()%5]))return 1;
    }

    ac.init(26);
    ac.build();
    for(int j=0;j<5;j++)
    {
        cnt++;
        if(ac.count(200,mods[j])!=power(26,200,mods[j]))return 1;
    }
    ac.init(2);
    ac.insert("aa");
    ac.build();
    if(ac.count(3,1000000007)!=5)return 1;
    printf("sample length=3 alphabet=ab forbidden=aa count=5\n");
    printf("PASS checks=%lld mismatches=0\n",cnt);
    return 0;
}
