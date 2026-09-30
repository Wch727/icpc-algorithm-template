#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1005;

// ================= 一、随机数 =================
// mt19937 比 rand() 快且质量好；范围用 [l,r] 闭区间

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
