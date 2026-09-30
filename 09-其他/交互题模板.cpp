#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// ================= 一、交互题框架 =================
// 规则：
//  1. 每次输出询问后必须 flush，否则评测机收不到，用 endl 或 cout.flush()
//  2. 询问格式按题目要求，例如 "? L" / "? 3 5" / "1 2 3"
//  3. 得到答案后用 "! ans" 或直接输出，然后立刻 return
//  4. 询问次数有上限，注意别超（一般 n log n 或 常数次）
//  5. 本地调试时用一个假交互器把询问/回答打在同一个终端里

namespace judge//本地假交互器：模拟评测机，读程序输出、给回答
{
    int n,m;
    int a[N];
    int qcnt,limit;

    void init(const char *data)//从字符串读入隐藏数据，格式：n m 然后 n 个数
    {
        stringstream ss(data);
        ss>>n>>m;
        for(int i=1;i<=n;i++)ss>>a[i];
        qcnt=0,limit=100000;
    }

    // 处理一次询问，返回回答；返回 INT_MIN 表示格式错误
    int reply(const string &s)
    {
        qcnt++;
        if(qcnt>limit)return INT_MIN;
        stringstream ss(s);
        char c;
        ss>>c;
        if(c=='?')//? k v  询问：区间内第 k 小的数是否大于 v
        {
            int k,v;
            ss>>k>>v;
            if(k<1||k>n)return INT_MIN;
            vector<int> t(a+1,a+n+1);
            sort(t.begin(),t.end());
            return t[k-1]>v;
        }
        return INT_MIN;
    }

    bool check_answer(const string &s)//检查最终答案，题目：第 m 小的数
    {
        stringstream ss(s);
        char c;
        ss>>c;
        if(c!='!')return false;
        int v;
        ss>>v;
        vector<int> t(a+1,a+n+1);
        sort(t.begin(),t.end());
        return v==t[m-1];
    }
}

// 发一次询问并读回回答
int ask(const string &s)
{
    cout<<s<<endl;//endl 会 flush，等价于 cout<<s<<"\n"<<flush;
    int r;
    cin>>r;
    return r;
}

void answer(int v)
{
    cout<<"! "<<v<<endl;
}

// 题目：n 个数互不相同，每次可以问「第 k 小的数是否大于 v」，
// 求第 m 小的数。做法：对值域二分，二分到确定第 m 小。
void solve()
{
    int n,m;
    cin>>n>>m;
    int lo=0,hi=1000000000;//值域二分
    // 先问出全局上界：第 n 小（即最大）是否大于 X 的做法不通用，
    // 这里直接固定值域二分 [lo,hi]，找最小的 v 使「第 m 小 > v」为假
    while(lo<hi)
    {
        int mid=(lo+hi+1)>>1;
        int r=ask("? "+to_string(m)+" "+to_string(mid));
        if(r==1)lo=mid;//第 m 小 > mid
        else hi=mid-1;
    }
    answer(lo+1);
}

// ================= 二、自测：本地假装是评测机 =================

void test_sample()
{
    // 一个「能直接看到交互过程」的小样例：n=5,m=3 的数据是 5 1 9 3 7
    // 真提交时 solve() 里的 ask 会向评测机发问；这里用假交互器直接回答
    string data="5 3\n5 1 9 3 7";
    judge::init(data.c_str());
    vector<int> t(judge::a+1,judge::a+judge::n+1);
    sort(t.begin(),t.end());
    printf("[交互] 隐藏数组 n=%d m=%d 第 m 小 = %d\n",judge::n,judge::m,t[judge::m-1]);
    // 演示一次两个方向的询问
    printf("[交互] \"? 3 5\" -> %d (第 3 小=5 是否大于 5)\n",judge::reply("? 3 5"));
    printf("[交互] \"? 3 4\" -> %d (第 3 小=5 是否大于 4)\n",judge::reply("? 3 4"));
    printf("[交互] \"! 5\" 判定 = %d (期望 1)\n",(int)judge::check_answer("! 5"));
    printf("[交互] \"! 7\" 判定 = %d (期望 0)\n",(int)judge::check_answer("! 7"));
    printf("[交互] 已用询问次数 = %d\n",judge::qcnt);
}

void test_interactive()
{
    // 说明：真正跑交互要用「询问时读回答」，所以这里用一个内置的假评测机
    // 直接测 solve 的核心逻辑（二分 + 询问次数）。
    int ok=1;
    mt19937 rnd(20240607);
    for(int t=1;t<=30;t++)
    {
        int n=rnd()%20+1;
        set<int> s;
        while((int)s.size()<n)s.insert(rnd()%1000000000+1);
        vector<int> v(s.begin(),s.end());
        int m=rnd()%n+1;
        judge::init("");//先清空，再手动填入数据
        judge::n=n,judge::m=m,judge::qcnt=0,judge::limit=100000;
        for(int i=1;i<=n;i++)judge::a[i]=v[i-1];
        // 模拟 solve 的二分过程：每步向假交互器提问
        int lo=0,hi=1000000000;
        while(lo<hi)
        {
            int mid=(lo+hi+1)>>1;
            int r=judge::reply("? "+to_string(m)+" "+to_string(mid));
            if(r==1)lo=mid;
            else hi=mid-1;
        }
        int got=lo+1;
        if(got!=v[m-1])
        {
            ok=0;
            printf("  第 %d 组: 得到 %d 期望 %d\n",t,got,v[m-1]);
        }
    }
    printf("[交互] 30 组随机数据二分询问 %s\n",ok?"全部通过":"失败");
}

int main()
{
    test_sample();
    test_interactive();
    // 真实交互时用下面这行（提交时只留 solve 的内容）：
    // solve();
    return 0;
}

/*
标准交互题骨架（提交用）：
    int main()
    {
        int T;cin>>T;
        while(T--)
        {
            int n;cin>>n;
            cout<<"? 1 1"<<endl;      // 询问 + flush
            int x;cin>>x;
            if(x==1){cout<<"! "<<n<<endl;continue;}
            cout<<"? 2"<<endl;        // 形如 "? L"
            int cur;cin>>cur;
            ...
            cout<<"! "<<ans<<endl;
        }
        return 0;
    }
注意：最后一定要 return，否则会多读一次导致 WA/Idle limit。
*/
