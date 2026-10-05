#pragma once
#include <bits/stdc++.h>
using namespace std;
namespace judge//本地假交互器：模拟评测机，读程序输出、给回答
{
    const int N=100005;
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
