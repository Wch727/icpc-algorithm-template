// 平衡树 Treap（带旋）：插入 / 删除 / 排名 / 第k小 / 前驱后继
// 每个结点随机一个优先值，小根堆性质 → 期望 O(log n)，最坏 O(n)
// 重复值按独立结点存（不去重），排名约定：get_rank(x) 返回「比 x 小的数个数」（0 起）
// 关键坑：旋转必须先用 int &q=ls[p] 绑好引用，递归返回后 ls[p] 可能已经不是原来那个结点了
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=200005;
const int INF=0x3f3f3f3f;
int n,x;

// xorshift 伪随机，比 rand() 快
unsigned long long seed=20240513;
