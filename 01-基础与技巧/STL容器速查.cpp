// 适用：堆的多关键字排序、集合去重；这里只提供比较器，不负责容器操作。
// 比较须满足严格弱序，相等时返回 false；堆比较表示优先级较低，方向与 set 相反。
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=100005;

// STL 常用容器速查：vector/pair/set/map/priority_queue/stack/queue/deque/bitset
// 每个板块都是一段能跑的小例子，赛场上直接抄走用
int n,m;

// 保存排序值 val 与标识 id，单个结点空间 O(1)；id 是业务编号，无固定下标约定。
struct Node// 想用自定义比较器，结构体最省事
{
    int val,id;
};

// 优先队列：小根堆（STL 默认是大根堆，想要小根堆就反过来写比较）
// 小根堆比较器，空间 O(1)；适配 priority_queue<Node,vector<Node>,CmpSmall>。
struct CmpSmall
{
    // O(1)，比较结点 a、b 的优先级；不修改参数。
    bool operator()(const Node& a,const Node& b)const
    {
        if(a.val!=b.val)return a.val>b.val;// val 小的排前面（堆顶）
        return a.id>b.id;// val 相同按 id，保证结果稳定
    }
};

// 优先队列：按「先 id 大、再 val 小」排序的多关键字比较器
// 多关键字堆比较器，空间 O(1)；id 优先，其次 val。
struct CmpMulti
{
    // O(1)，比较结点 a、b 的优先级；不修改参数。
    bool operator()(const Node& a,const Node& b)const
    {
        if(a.id!=b.id)return a.id<b.id;// id 大的在堆顶
        return a.val>b.val;
    }
};

// set 自定义比较器：按 (val,id) 字典序，注意等价元素会被去重
// 有序集合比较器，空间 O(1)；只有 val、id 都等价时才被 set 去重。
struct CmpPair
{
    // O(1)，比较结点 a、b 的优先级；不修改参数。
    bool operator()(const Node& a,const Node& b)const
    {
        if(a.val!=b.val)return a.val<b.val;
        return a.id<b.id;
    }
};
