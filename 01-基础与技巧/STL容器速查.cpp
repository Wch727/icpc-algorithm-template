#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// STL 常用容器速查：vector/pair/set/map/priority_queue/stack/queue/deque/bitset
// 每个板块都是一段能跑的小例子，赛场上直接抄走用
int n,m;

struct Node// 想用自定义比较器，结构体最省事
{
    int val,id;
};

// 优先队列：小根堆（STL 默认是大根堆，想要小根堆就反过来写比较）
struct CmpSmall
{
    bool operator()(const Node& a,const Node& b)const
    {
        if(a.val!=b.val)return a.val>b.val;// val 小的排前面（堆顶）
        return a.id>b.id;// val 相同按 id，保证结果稳定
    }
};

// 优先队列：按「先 id 大、再 val 小」排序的多关键字比较器
struct CmpMulti
{
    bool operator()(const Node& a,const Node& b)const
    {
        if(a.id!=b.id)return a.id<b.id;// id 大的在堆顶
        return a.val>b.val;
    }
};

// set 自定义比较器：按 (val,id) 字典序，注意等价元素会被去重
struct CmpPair
{
    bool operator()(const Node& a,const Node& b)const
    {
        if(a.val!=b.val)return a.val<b.val;
        return a.id<b.id;
    }
};
