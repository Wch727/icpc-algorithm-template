#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

// 以下按容器分区，抄需要的操作即可；下标从 0 开始，迭代器区间均为 [l,r)。
struct Node
{
    int val,id;
};

// 堆：比较器返回 true 表示 a 的优先级低于 b；相等时必须返回 false。
struct CmpSmall
{
    bool operator()(const Node &a,const Node &b)const
    {
        if(a.val!=b.val)return a.val>b.val;
        return a.id>b.id;
    }
};
struct CmpMulti // 先 id 大，再 val 小
{
    bool operator()(const Node &a,const Node &b)const
    {
        if(a.id!=b.id)return a.id<b.id;
        return a.val>b.val;
    }
};
struct CmpPair // set：按 (val,id) 升序，两个字段均相等才视为重复
{
    bool operator()(const Node &a,const Node &b)const
    {
        if(a.val!=b.val)return a.val<b.val;
        return a.id<b.id;
    }
};

void stl_usage() // 各块是独立示例，无需复制外层函数
{
    // vector、排序、去重、二分：O(n log n) 排序，O(log n) 二分
    {
        vector<int> v={3,1,3,2};
        sort(v.begin(),v.end());
        v.erase(unique(v.begin(),v.end()),v.end()); // unique 只移动元素，不改变 size
        int l=lower_bound(v.begin(),v.end(),2)-v.begin(); // 第一个 >=2
        int r=upper_bound(v.begin(),v.end(),2)-v.begin(); // 第一个 >2
        cout<<r-l<<'\n'; // 有序数组中 2 的个数；找不到时迭代器可能是 end()
        v.erase(v.begin()+l,v.begin()+r); // 删除半开区间，后面的元素前移
        v.reserve(100); // 只预留容量，不增加 size，不能直接访问 v[99]
    }
    // 第 k 小、部分排序、排列、累加
    {
        vector<int> v={5,1,4,2,3};
        int k=3; // 1<=k<=size
        nth_element(v.begin(),v.begin()+k-1,v.end()); // 第 k 小就位，其余不保证有序
        cout<<v[k-1]<<'\n';
        partial_sort(v.begin(),v.begin()+k,v.end()); // 最小 k 个升序，O(n log k)
        cout<<accumulate(v.begin(),v.end(),0LL)<<'\n'; // 初值决定累加类型
        reverse(v.begin(),v.end());
        sort(v.begin(),v.end(),greater<int>()); // 降序
        vector<int> p={1,1,2};
        do{cout<<p[0]<<p[1]<<p[2]<<' ';}while(next_permutation(p.begin(),p.end()));
        cout<<'\n'; // 枚举全部排列前先升序；重复元素会自动去重
    }
    // set / multiset：插入、删除、查找 O(log n)
    {
        set<int> s={1,3,5};
        int x=3;
        auto it=s.lower_bound(x); // 用成员函数，别用 lower_bound(s.begin(),s.end(),x)
        if(it!=s.end())cout<<*it<<'\n'; // >=x 的后继
        if(it!=s.begin())cout<<*prev(it)<<'\n'; // <x 的前驱
        it=s.upper_bound(x); // >x；end() 不能解引用
        if(it!=s.end())s.erase(it); // 删除当前元素后，原 it 失效
        multiset<int> ms={2,2,3};
        it=ms.find(2);
        if(it!=ms.end())ms.erase(it); // 只删一个 2；ms.erase(2) 会删掉所有 2
        auto range=ms.equal_range(2); // [first,second) 包含所有等于 2 的元素
        cout<<distance(range.first,range.second)<<'\n'; // 迭代器计数是线性的
        set<Node,CmpPair> custom; // 比较器等价的元素只保留一个
        custom.insert({4,1});
    }
    // map：按键有序 O(log n)；unordered_map：均摊 O(1)，最坏 O(n)
    {
        map<int,ll> mp;
        mp[7]+=3; // 不存在会先插入默认值 0；仅查找用 find
        auto it=mp.find(7);
        if(it!=mp.end())cout<<it->second<<'\n';
        cout<<mp.count(8)<<'\n'; // 是否存在，0 或 1；不会插入
        for(auto it=mp.begin();it!=mp.end();)
            if(it->second==0)it=mp.erase(it); // 删除时用返回的下一迭代器
            else ++it;
        unordered_map<ll,int> cnt;
        cnt.max_load_factor(0.7); // reserve 前设置，降低装载因子
        cnt.reserve(1000);
        cnt[123]++;
        cnt.erase(123); // 遍历顺序不确定；rehash 会使迭代器失效
    }
    // priority_queue：插入/弹出 O(log n)，top O(1)；pop 不返回值
    {
        priority_queue<int> big; // 大根堆
        priority_queue<int,vector<int>,greater<int> > small; // 小根堆
        priority_queue<pair<ll,int>,vector<pair<ll,int> >,greater<pair<ll,int> > > q;
        q.push({10,2}),q.push({10,1}); // pair 字典序：先距离，再编号
        cout<<q.top().second<<'\n';
        q.pop();
        priority_queue<Node,vector<Node>,CmpSmall> custom;
        custom.push({4,2}),custom.push({4,1});
        cout<<custom.top().id<<'\n';
        big.push(2),small.push(2); // 堆不支持按值删除或直接修改元素
    }
    // string：substr 是 (起点,长度)，不是左右端点
    {
        string s="ababa";
        cout<<s.substr(1,3)<<'\n'; // bab
        size_t pos=s.find("ba");
        if(pos!=string::npos)cout<<pos<<'\n'; // find 失败返回 npos，不是普通 int 下标
        s.erase(1,2); // 从下标 1 起删除 2 个字符
        s.insert(1,"xy");
        s.replace(1,2,"z"); // 从下标 1 起替换 2 个字符
        cout<<s<<'\n';
    }
    // deque / stack / queue：端点操作 O(1)，读取或弹出前保证非空
    {
        deque<int> dq={2,3};
        dq.push_front(1),dq.push_back(4);
        dq.pop_front(),dq.pop_back();
        cout<<dq.front()<<' '<<dq.back()<<'\n'; // deque 支持随机下标
        stack<int> st;
        st.push(1);cout<<st.top()<<'\n';st.pop();
        queue<int> q;
        q.push(1);cout<<q.front()<<'\n';q.pop();
    }
    // bitset：容量为编译期常量；位编号从低位 0 开始
    {
        bitset<16> b(5),c(3);
        b.set(4),b.reset(0),b.flip(1); // 不带位置参数则操作全部位
        cout<<b.count()<<' '<<b.any()<<' '<<b.all()<<' '<<b.none()<<'\n';
        cout<<(b&c)<<' '<<(b|c)<<' '<<(b^c)<<'\n';
        cout<<(b<<2)<<'\n'; // 空位补 0，越界位丢弃；逻辑值读取用 b[i] 或 b.test(i)
        cout<<b.to_ullong()<<'\n'; // 高位不能放进 ull 时会抛出异常
    }
}
