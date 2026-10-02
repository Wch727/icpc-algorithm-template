// STL容器速查 的测试与对拍代码
// 模板本体：01-基础与技巧/STL容器速查.cpp
#include "../../01-基础与技巧/STL容器速查.cpp"

int main()
{
    stl_usage(); // 运行模板中的全部速查示例，检查实际访问和迭代器操作。
    srand(20240531);
    // ---- vector：最常用的动态数组 ----
    vector<int> v={5,3,8,1};
    v.push_back(9);
    v.pop_back();
    sort(v.begin(),v.end());// 默认升序
    sort(v.begin(),v.end(),greater<int>());// 降序
    sort(v.begin(),v.end(),[](int a,int b){return a>b;});// 等价写法
    v.insert(v.begin(),100);// 头部插入是 O(n)，别在循环里干
    v.erase(v.begin());
    v.resize(10,7);// 扩到 10，多出来的补 7
    v.assign(3,4);// 变成 3 个 4
    printf("vector: size=%d front=%d back=%d\n",(int)v.size(),v.front(),v.back());
    printf("vector: 下标遍历 ");
    for(int i=0;i<(int)v.size();i++)printf("%d ",v[i]);
    printf("\n");
    printf("vector: 迭代器遍历 ");
    for(vector<int>::iterator it=v.begin();it!=v.end();it++)printf("%d ",*it);
    printf("\n");
    printf("vector: 范围 for ");
    for(int x:v)printf("%d ",x);
    printf("\n");
    vector<int> w={4,4,4};
    if(v==w)printf("vector: 支持 == 比较\n");
    v.clear();
    if(v.empty())printf("vector: clear 之后 empty\n");
    // 去重三步曲：排序 + unique + erase，unique 返回新末尾
    vector<int> d={3,1,3,2,1};
    sort(d.begin(),d.end());
    d.erase(unique(d.begin(),d.end()),d.end());
    printf("vector: 去重后 ");
    for(int x:d)printf("%d ",x);
    printf("\n");

    // ---- pair：绑定两个值，sort 默认先比 first 再比 second ----
    pair<int,int> p=make_pair(3,5);
    p={7,2};// C++11 起可以直接花括号
    printf("pair: (%d,%d) 比较: (1,9)<(2,0) = %d\n",p.first,p.second,(int)(make_pair(1,9)<make_pair(2,0)));
    vector<pair<int,int>> vp={{2,3},{1,9},{1,2}};
    sort(vp.begin(),vp.end());
    printf("pair: 排序后 ");
    for(int i=0;i<(int)vp.size();i++)printf("(%d,%d) ",vp[i].first,vp[i].second);
    printf("\n");
    // 每个元素是一个 pair 时，范围 for 想改值要写引用 &
    for(pair<int,int>& x:vp)x.second++;
    printf("pair: 引用改值后 first of last=%d second=%d\n",vp.back().first,vp.back().second);

    // ---- set：有序、去重，增删查都是 O(log n) ----
    set<int> st;
    st.insert(3),st.insert(1),st.insert(3);// 插两次 3 只留一个
    st.insert(7),st.insert(5);
    printf("set: size=%d 自动升序 ",(int)st.size());
    for(int x:st)printf("%d ",x);
    printf("\n");
    st.erase(3);// 按值删
    if(st.count(5))printf("set: count(5)=1\n");
    set<int>::iterator it=st.lower_bound(4);// 第一个 >= 4
    if(it!=st.end())printf("set: lower_bound(4)=%d\n",*it);
    it=st.upper_bound(5);// 第一个 > 5
    if(it!=st.end())printf("set: upper_bound(5)=%d\n",*it);
    // 找前驱（set 只能 ++/-- 走，迭代器是双向的）
    it=st.lower_bound(100);
    if(it==st.end())it=st.end(),--it;
    printf("set: 最大的元素=%d\n",*it);
    // multiset 允许重复，erase(x) 会把等于 x 的全删掉
    multiset<int> mst={1,1,2,3};
    mst.erase(1);// 删掉所有 1
    mst.erase(mst.find(2));// 只删一个 2
    printf("multiset: 剩下 %d 个元素\n",(int)mst.size());
    // set 也支持自定义比较器
    set<Node,CmpPair> sn;
    sn.insert({2,1}),sn.insert({1,5}),sn.insert({2,0});
    printf("set<Node>: ");
    for(const Node& x:sn)printf("(%d,%d) ",x.val,x.id);
    printf("\n");

    // ---- map：有序键值对，底层红黑树，按 key 排序 ----
    map<string,int> mp;
    mp["apple"]=3;
    mp["banana"]=1;
    mp["cherry"]=2;
    mp["apple"]+=4;// 已存在就覆盖/累加，[] 会默认插入 0
    printf("map: 按 key 字典序遍历 ");
    for(map<string,int>::iterator i2=mp.begin();i2!=mp.end();i2++)printf("%s=%d ",i2->first.c_str(),i2->second);
    printf("\n");
    if(mp.count("apple"))printf("map: apple=%d\n",mp["apple"]);
    mp.erase("banana");
    printf("map: erase 后 size=%d\n",(int)mp.size());
    // 统计出现次数（数组下标装不下的键用 map 最合适）
    map<int,int> cnt;
    int arr[7]={3,1,3,2,1,3,2};
    for(int i=0;i<7;i++)cnt[arr[i]]++;
    printf("map: 众数 ");
    for(map<int,int>::iterator i2=cnt.begin();i2!=cnt.end();i2++)
        if(i2->second>=2)printf("%d:%d ",i2->first,i2->second);
    printf("\n");
    // 需要哈希（O(1) 但无序）就用 unordered_map，注意会被卡哈希
    unordered_map<int,int> um;
    um[5]=1;
    printf("unordered_map: um[5]=%d\n",um[5]);

    // ---- priority_queue：默认大根堆，只能看堆顶 ----
    priority_queue<int> pq;
    int raw[6]={5,1,9,3,9,2};
    for(int i=0;i<6;i++)pq.push(raw[i]);
    printf("priority_queue 大根堆: 出堆 ");
    while(!pq.empty())printf("%d ",pq.top()),pq.pop();
    printf("\n");
    priority_queue<int,vector<int>,greater<int>> pqmin;// 小根堆：三个参数
    for(int i=0;i<6;i++)pqmin.push(raw[i]);
    printf("priority_queue 小根堆: 出堆 ");
    while(!pqmin.empty())printf("%d ",pqmin.top()),pqmin.pop();
    printf("\n");
    // 自定义结构体比较器
    priority_queue<Node,vector<Node>,CmpSmall> pqn;
    pqn.push({5,1}),pqn.push({3,2}),pqn.push({3,1}),pqn.push({8,0});
    printf("priority_queue 自定义: 出堆 ");
    while(!pqn.empty())printf("(%d,%d) ",pqn.top().val,pqn.top().id),pqn.pop();
    printf("\n");
    // 多关键字比较器
    priority_queue<Node,vector<Node>,CmpMulti> pqm;
    pqm.push({5,1}),pqm.push({3,2}),pqm.push({1,2}),pqm.push({9,0});
    printf("priority_queue 多关键字: 出堆 ");
    while(!pqm.empty())printf("(%d,%d) ",pqm.top().val,pqm.top().id),pqm.pop();
    printf("\n");

    // ---- stack：后进先出 ----
    stack<int> sk;
    for(int i=1;i<=5;i++)sk.push(i*i);
    printf("stack: size=%d top=%d 出栈 ",(int)sk.size(),sk.top());
    while(!sk.empty())printf("%d ",sk.top()),sk.pop();
    printf("\n");

    // ---- queue：先进先出，BFS 主力 ----
    queue<int> qu;
    for(int i=1;i<=5;i++)qu.push(i*i);
    printf("queue: size=%d front=%d back=%d 出队 ",(int)qu.size(),qu.front(),qu.back());
    while(!qu.empty())printf("%d ",qu.front()),qu.pop();
    printf("\n");
    // BFS 用队列的骨架（这里只演示队列用法，不建图）
    queue<pair<int,int>> qb;// (结点, 步数)
    qb.push({1,0});
    printf("queue<pair>: front=(%d,%d)\n",qb.front().first,qb.front().second);

    // ---- deque：双端队列，单调队列就靠它 ----
    deque<int> dq;
    dq.push_back(2),dq.push_back(3);
    dq.push_front(1);
    printf("deque: front=%d back=%d size=%d\n",dq.front(),dq.back(),(int)dq.size());
    dq.pop_front(),dq.pop_back();
    printf("deque: 两个都弹掉后 size=%d front=%d\n",(int)dq.size(),dq.front());
    dq[0]=99;// deque 支持随机访问，也能 sort
    printf("deque: 支持下标 dq[0]=%d\n",dq[0]);
    // 滑动窗口最大值（单调队列）小例子：数组 [1 3 -1 -3 5 3 6 7]，窗口 k=3
    int win[9]={1,3,-1,-3,5,3,6,7,0},k=3;
    deque<int> q;// 存下标，队列里的值单调递减
    printf("deque 单调队列滑窗最大值: ");
    for(int i=0;i<8;i++)
    {
        while(!q.empty()&&win[q.back()]<=win[i])q.pop_back();// 队尾小的都不可能当最大值
        q.push_back(i);
        if(q.front()<=i-k)q.pop_front();// 队头过期
        if(i>=k-1)printf("%d ",win[q.front()]);
    }
    printf("(want 3 3 5 5 6 7)\n");

    // ---- bitset：定长二进制，位运算 O(n/64) ----
    bitset<32> bs(13);
    printf("bitset: 13 = %s count=%d size=%d\n",bs.to_string().c_str(),(int)bs.count(),(int)bs.size());
    bs.set(20);// 置 1
    bs.flip(0);// 0 位取反
    bs.reset(2);// 置 0
    printf("bitset: 改位后 test(20)=%d test(2)=%d any=%d none=%d\n",(int)bs.test(20),(int)bs.test(2),(int)bs.any(),(int)bs.none());
    printf("bitset: 二进制串 %s\n",bs.to_string().c_str());
    printf("bitset: to_ulong=%lu\n",(unsigned long)bs.to_ulong());
    bitset<8> x(string("10110000")),y(string("00111100"));// 从 01 串构造
    printf("bitset: AND=%s OR=%s XOR=%s NOT=%s\n",(x&y).to_string().c_str(),(x|y).to_string().c_str(),(x^y).to_string().c_str(),(~x).to_string().c_str());
    printf("bitset: 左移2=%s 右移2=%s 比较 x>y=%d\n",(x<<2).to_string().c_str(),(x>>2).to_string().c_str(),(int)(x.to_ulong()>y.to_ulong()));
    bs.reset();
    if(bs.none())printf("bitset: reset 后全 0\n");

    // 自测1：vector 去重 + 排序与暴力对拍
    for(int t=1;t<=2000;t++)
    {
        int len=rand()%30+1;
        vector<int> a;
        for(int i=0;i<len;i++)a.push_back(rand()%10);
        vector<int> got=a;
        sort(got.begin(),got.end());
        got.erase(unique(got.begin(),got.end()),got.end());
        set<int> want(a.begin(),a.end());// set 天然有序去重，拿来当标准答案
        if(got.size()!=want.size())
        {
            printf("fail vector dedup size t=%d\n",t);
            return 0;
        }
        int idx=0,ok=1;
        for(int w2:want)
            if(got[idx++]!=w2)ok=0;
        if(!ok)
        {
            printf("fail vector dedup value t=%d\n",t);
            return 0;
        }
    }
    printf("vector sort+unique self-check OK\n");

    // 自测2：set 与暴力（排序去重数组）对拍
    for(int t=1;t<=2000;t++)
    {
        int len=rand()%20+1;
        vector<int> a;
        set<int> st2;
        for(int i=0;i<len;i++)
        {
            int x=rand()%8;
            a.push_back(x),st2.insert(x);
        }
        sort(a.begin(),a.end());
        a.erase(unique(a.begin(),a.end()),a.end());
        if(st2.size()!=a.size())
        {
            printf("fail set size t=%d\n",t);
            return 0;
        }
        int idx=0,ok=1;
        for(int w2:st2)
            if(w2!=a[idx++])ok=0;
        if(!ok)
        {
            printf("fail set order t=%d\n",t);
            return 0;
        }
        // lower_bound / upper_bound 与二分答案对拍
        for(int q2=0;q2<=9;q2++)
        {
            int lo=0,up=0;
            for(int w2:a)
            {
                if(w2<q2)lo++;
                if(w2<=q2)up++;
            }
            int gl=0,gu=0;
            for(int w2:st2)
            {
                if(w2<q2)gl++;
                if(w2<=q2)gu++;
            }
            if(gl!=lo||gu!=up)
            {
                printf("fail set bound q=%d\n",q2);
                return 0;
            }
        }
    }
    printf("set self-check OK\n");

    // 自测3：map 计数与数组桶对拍
    for(int t=1;t<=2000;t++)
    {
        int len=rand()%50+1;
        map<int,int> c1;
        int bucket[12]={0};
        vector<int> a;
        for(int i=0;i<len;i++)
        {
            int x=rand()%11-5;// 含负数，数组要加偏移，map 不用
            a.push_back(x);
            c1[x]++;
            bucket[x+5]++;
        }
        // 注意：下面查计数一定要用 find，写 c1[key] 会顺手插入 key=0 的新键，
        // 把 map 本身改掉（这个坑踩过一次）
        for(int key=-5;key<=5;key++)
        {
            map<int,int>::iterator it=c1.find(key);
            int got=(it==c1.end())?0:it->second;
            if(got!=bucket[key+5])
            {
                printf("fail map count key=%d\n",key);
                return 0;
            }
        }
        set<int> ds(a.begin(),a.end());// 种类数用 set 直接数，unique 不排序只去相邻重复
        // map 的键必须正好是「排序去重后」的每个值，出现次数要对得上
        vector<int> keys(a);
        sort(keys.begin(),keys.end());
        keys.erase(unique(keys.begin(),keys.end()),keys.end());
        int idx=0,ok=1;
        for(map<int,int>::iterator it=c1.begin();it!=c1.end();it++,idx++)
        {
            if(idx>=(int)keys.size()||it->first!=keys[idx]){ok=0;break;}
            int cnt=0;
            for(int v:a)if(v==keys[idx])cnt++;
            if(it->second!=cnt){ok=0;break;}
        }
        if(!ok||c1.size()!=ds.size())
        {
            printf("fail map distinct t=%d\n",t);
            return 0;
        }
    }
    printf("map count self-check OK\n");

    // 自测4：大根堆/小根堆与排序结果对拍
    for(int t=1;t<=2000;t++)
    {
        int len=rand()%100+1;
        vector<int> a;
        priority_queue<int> pq1;
        priority_queue<int,vector<int>,greater<int>> pq2;
        for(int i=0;i<len;i++)
        {
            int x=rand()%1000;
            a.push_back(x),pq1.push(x),pq2.push(x);
        }
        sort(a.begin(),a.end());
        int ok=1;
        for(int i=0;i<len;i++)// 大根堆出堆就是降序
        {
            if(pq1.top()!=a[len-1-i])ok=0;
            if(pq2.top()!=a[i])ok=0;// 小根堆出堆是升序
            pq1.pop(),pq2.pop();
        }
        if(!ok)
        {
            printf("fail priority_queue t=%d\n",t);
            return 0;
        }
    }
    printf("priority_queue self-check OK\n");

    // 自测5：自定义比较器 CmpSmall 出堆顺序 = 按 (val,id) 升序
    {
        vector<Node> a;
        priority_queue<Node,vector<Node>,CmpSmall> pq3;
        for(int i=0;i<5;i++)
            for(int j=0;j<3;j++)a.push_back({(int)(rand()%6),j}),pq3.push(a.back());
        sort(a.begin(),a.end(),[](const Node& p1,const Node& p2)
        {
            if(p1.val!=p2.val)return p1.val<p2.val;
            return p1.id<p2.id;
        });
        int ok=1;
        for(int i=0;i<(int)a.size();i++)
        {
            if(pq3.top().val!=a[i].val||pq3.top().id!=a[i].id)ok=0;
            pq3.pop();
        }
        if(!ok)
        {
            printf("fail custom comparator\n");
            return 0;
        }
        printf("priority_queue 自定义比较器 self-check OK\n");
    }

    // 自测6：单调队列滑窗最大值与 O(nk) 暴力对拍
    for(int t=1;t<=500;t++)
    {
        int len=rand()%40+1,kk=rand()%len+1;
        vector<int> a;
        for(int i=0;i<len;i++)a.push_back(rand()%100-50);
        deque<int> q2;
        int ok=1;
        for(int i=0;i<len;i++)
        {
            while(!q2.empty()&&a[q2.back()]<=a[i])q2.pop_back();
            q2.push_back(i);
            if(q2.front()<=i-kk)q2.pop_front();
            if(i>=kk-1)
            {
                int mx=-1e9;
                for(int j=i-kk+1;j<=i;j++)mx=max(mx,a[j]);// 暴力扫窗口
                if(a[q2.front()]!=mx)ok=0;
            }
        }
        if(!ok)
        {
            printf("fail deque monotonic t=%d\n",t);
            return 0;
        }
    }
    printf("deque 单调队列 self-check OK\n");

    // 自测7：bitset 位运算与手算对拍
    for(int t=1;t<=2000;t++)
    {
        unsigned int x2=rand(),y2=rand();
        bitset<32> bx(x2),by(y2);
        if((bx&by).to_ulong()!=(x2&y2)){printf("fail bitset AND\n");return 0;}
        if((bx|by).to_ulong()!=(x2|y2)){printf("fail bitset OR\n");return 0;}
        if((bx^by).to_ulong()!=(x2^y2)){printf("fail bitset XOR\n");return 0;}
        if(bx.count()!=(size_t)__builtin_popcount(x2)){printf("fail bitset count\n");return 0;}
        if((bx<<3).to_ulong()!=(x2<<3)){printf("fail bitset shl\n");return 0;}
    }
    printf("bitset self-check OK\n");

    // 自测8：stack 反转序列 / queue 保序
    {
        vector<int> a;
        for(int i=1;i<=20;i++)a.push_back(i);
        stack<int> sk2;
        for(int x:a)sk2.push(x);
        vector<int> r;
        while(!sk2.empty())r.push_back(sk2.top()),sk2.pop();
        reverse(r.begin(),r.end());
        if(r!=a)
        {
            printf("fail stack reverse\n");
            return 0;
        }
        queue<int> qu2;
        for(int x:a)qu2.push(x);
        vector<int> r2;
        while(!qu2.empty())r2.push_back(qu2.front()),qu2.pop();
        if(r2!=a)
        {
            printf("fail queue order\n");
            return 0;
        }
        printf("stack/queue self-check OK\n");
    }
    printf("all STL self-check OK\n");
    return 0;
}
