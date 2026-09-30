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
