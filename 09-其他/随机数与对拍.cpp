#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// seed 改成固定值可复现；randint 为闭区间，避免 rnd()%范围 的偏差和范围计算溢出。
unsigned long long seed=chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 rnd(seed);
ll randint(ll l,ll r){return uniform_int_distribution<ll>(l,r)(rnd);}
// 随机排列：iota(p.begin(),p.end(),1); shuffle(p.begin(),p.end(),rnd);
// 生成树可让 i=2..n 随机连向 randint(1,i-1)，但这种方式不均匀覆盖所有标号树。

// 同进程对拍：按题目实现这三个函数；solve/brute 各自拿副本，不会互相修改输入。
vector<ll> gen(); // 小数据生成器，主动覆盖重复、负数、极小规模等边界
ll solve(vector<ll> a);
ll brute(vector<ll> a);
bool stress(int rounds)
{
    rnd.seed(seed);
    for(int t=1;t<=rounds;t++)
    {
        vector<ll> a=gen();
        ll x=solve(a),y=brute(a);
        if(x!=y)
        {
            cerr<<"seed="<<seed<<" case="<<t<<" got="<<x<<" expected="<<y<<'\n';
            cerr<<a.size()<<'\n';
            for(ll v:a)cerr<<v<<' ';
            cerr<<'\n';
            return false; // 首次不同就停，保留种子和完整输入；别只打印答案
        }
    }
    return true;
}
// 若输入是图/矩阵，改 gen 的数据类型和失败打印；浮点输出按题意比较误差。
// 不只生成“普通随机数据”：手写最小规模、全相等、单调、极值等组，再跑随机组。

/* Windows 双进程对拍：另存为 bat，与 gen.exe/std.exe/my.exe 放同目录。
:loop
 gen.exe > in.txt
 if errorlevel 1 goto error
 std.exe < in.txt > std.out
 if errorlevel 1 goto error
 my.exe < in.txt > my.out
 if errorlevel 1 goto error
 fc /b std.out my.out > nul
 if not errorlevel 1 goto loop
:error
 pause
字节比较要求输出格式一致；允许空白差异或浮点误差时用专门的比较器。
*/
