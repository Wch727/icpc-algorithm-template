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
