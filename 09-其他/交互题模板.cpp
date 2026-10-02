#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// ================= 一、交互题框架 =================
// 规则：
//  1. 每次输出询问后必须 flush，否则评测机收不到，用 endl 或 cout.flush()
//  2. 询问格式按题目要求，例如 "? L" / "? 3 5" / "1 2 3"
//  3. 得到答案后用 "! ans" 或直接输出，再按协议结束当前测试或整个程序
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
        n=m=0;
        qcnt=0,limit=100000;
        if(!(ss>>n>>m)||n<1||n>=N||m<1||m>n){n=m=0;return;}
        for(int i=1;i<=n;i++)if(!(ss>>a[i])){n=m=0;return;}
    }

    // 处理一次询问，返回回答；返回 INT_MIN 表示格式错误
    int reply(const string &s)
    {
        qcnt++;
        if(qcnt>limit)return INT_MIN;
        stringstream ss(s);
        char c;
        if(!(ss>>c))return INT_MIN;
        if(c=='?')// ? k v：整个隐藏数组的第 k 小是否大于 v
        {
            int k,v;
            if(!(ss>>k>>v)||k<1||k>n)return INT_MIN;
            ss>>ws;
            if(!ss.eof())return INT_MIN;
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
        if(!(ss>>c)||m<1||m>n)return false;
        if(c!='!')return false;
        int v;
        if(!(ss>>v))return false;
        ss>>ws;
        if(!ss.eof())return false;
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
    if(!(cin>>r)||r==-1)exit(0); // 本示例协议以 -1 表示错误；其他题按协议改
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
    if(!(cin>>n>>m))return;
    int lo=0,hi=1000000000;//值域二分
    // 已知答案在 [0,10^9]，找最小的 v 使「第 m 小 > v」为假。
    while(lo<hi)
    {
        int mid=lo+(hi-lo)/2;
        int r=ask("? "+to_string(m)+" "+to_string(mid));
        if(r==1)lo=mid+1;
        else if(r==0)hi=mid;
        else exit(0); // 只接受本协议的 0/1 回答
    }
    answer(lo);
}
