// @code protocol
#include<bits/stdc++.h>
using namespace std;

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

// @code example
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
