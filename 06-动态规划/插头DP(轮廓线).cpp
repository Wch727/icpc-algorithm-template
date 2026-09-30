#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=14;// 轮廓线长度（列数上限）
int n,m;// n 行 m 列，插头 dp 适合 n,m<=12
int vis[10][10];
// 状态编码：2 bit 一个插头，从低位到高位对应轮廓线上第 0,1,...,m 个位置
// 0 = 无插头，1 = 左括号 '(', 2 = 右括号 ')'
// 轮廓线上"还没接好的插头"一定是匹配好的括号序列，所以只需要记括号类型
// 第 j 个格子的左插头在位置 j，上插头在位置 j+1（j 从 0 起）
unordered_map<int,ll> f[2];// 滚动数组，按格转移
