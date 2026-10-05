# STL 容器与算法速查

下标从 0 起，迭代器区间为 `[l,r)`；读取端点或弹出前保证非空，不解引用 `end()`。各容器共有的 `size()`、`empty()` 不逐一重复。下文 `n` 为元素数，`k` 为所取数量。

## vector 与区间修改

| 写法 | 作用、条件与复杂度 |
|---|---|
| `v.reserve(n)` / `v.resize(n,x)` | 前者只预留容量，不增加 size；后者改变 size，新增元素填 x。 |
| `v.erase(l,r)` / `v.insert(it,x)` | 删除半开迭代器区间 / 在 it 前插入。移动后续元素，O(n)。 |
| `v.erase(remove(v.begin(),v.end(),x),v.end())` | 删除所有等于 x 的项，O(n)；remove 只移动元素，须配合 erase。 |
| 迭代器失效 | vector 扩容使全部迭代器、指针、引用失效；未扩容的插入及删除使操作位置及之后失效。clear 不释放容量。 |

## 排序、查找与数值算法

| 写法 | 作用、条件与复杂度 |
|---|---|
| `sort(l,r,cmp)` / `stable_sort(l,r,cmp)` | 排序 / 保留等价元素原顺序。sort 为 O(n log n)；stable_sort 有足够辅助内存时相同，否则 O(n log² n)。 |
| `v.erase(unique(v.begin(),v.end()),v.end())` | 去相邻重复，O(n)。全局去重须先排序；unique 不改变 size。 |
| `lower_bound(l,r,x)` / `upper_bound(l,r,x)` | 非降序区间中首个 ≥x / >x 的迭代器；随机访问时 O(log n)。降序数据须同时传匹配比较器。 |
| `equal_range(l,r,x)` | 返回等于 x 的半开区间；vector 中两端相减就是出现次数。 |
| `nth_element(l,l+k-1,r)` | 1≤k≤n；第 k 小就位，两侧分别不大于 / 不小于它，内部无序，平均 O(n)。 |
| `partial_sort(l,l+k,r)` | 最小 k 项排好序，其余无序；0≤k≤n，O(n log k)，k≤1 时 O(n)。 |
| `next_permutation(l,r)` | 下一个字典序排列，O(n)；枚举全部排列须先升序。重复元素自然去重，返回 false 时重置为最小排列。 |
| `accumulate(l,r,0LL)` | 累加，O(n)；初值决定累加类型，写 0 会按 int 累加。 |
| `rotate(l,m,r)` | 把 `[m,r)` 移到 `[l,m)` 前，两段内部顺序不变，O(n)。 |

## set / multiset / map

| 写法 | 作用、条件与复杂度 |
|---|---|
| `s.lower_bound(x)` / `s.upper_bound(x)` | 首个 ≥x / >x；O(log n)。必须用成员函数，通用二分在树迭代器上移动为 O(n)。 |
| `it=s.lower_bound(x)`；`prev(it)` | it 是 ≥x 的后继（需 it!=end）；prev(it) 是 <x 的前驱（需 it!=begin），包括 it=end 的情况。 |
| `ms.erase(x)` / `ms.erase(it)` | 前者删除所有相等项，O(log n+个数)；后者只删一项，均摊 O(1)，it 须有效。 |
| `ms.count(x)` / `ms.equal_range(x)` | 计数 / 相等项区间，O(log n+个数) / O(log n)。遍历或 distance 该区间仍需 O(个数)。 |
| `mp[x]` / `mp.find(x)` | 前者在缺键时插入默认值；后者只查找，缺键返回 end。均为 O(log n)。 |
| `it=s.erase(it)` | 删除时取返回的下一迭代器；树容器插入不使旧迭代器失效，删除仅使被删元素失效。 |

## 比较与堆

| 写法 | 作用、条件与复杂度 |
|---|---|
| `pair` / `tuple` / `tie(a.x,a.y)` | 默认字典序；tie 可省掉手写多关键字比较。`auto [x,y]=p` 是复制，`auto &[x,y]=p` 可修改原对象。 |
| `return tie(a.x,a.y)<tie(b.x,b.y);` | sort / set 按 (x,y) 升序；比较器必须严格弱序，相等时返回 false，不能用 ≤。set 以 cmp(a,b) 与 cmp(b,a) 均为 false 判等。 |
| `priority_queue<T> q` | 默认大根堆；top 为 O(1)，push / pop 为 O(log n)，pop 不返回值。 |
| `priority_queue<T,vector<T>,greater<T>> q` | 小根堆；T 为 pair 时，先取 first 小，再取 second 小。 |
| `return tie(a.x,a.y)>tie(b.x,b.y);` | 堆的比较器表示 a 优先级低于 b；此式使 (x,y) 最小者在顶。堆不能按值删除或直接修改；可另记有效性并懒删除。 |

## 哈希、字符串与其他容器

| 写法 | 作用、条件与复杂度 |
|---|---|
| `h.max_load_factor(0.7)`；`h.reserve(n)` | unordered 容器先设负载因子，再按元素数预留；查增删平均 O(1)、最坏 O(n)，无顺序。不是抗碰撞保证。 |
| rehash 的失效规则 | rehash 使迭代器失效，元素引用 / 指针仍有效；插入可能触发 rehash，删除仅使被删元素失效。 |
| `s.substr(pos,len)` / `s.erase(pos,len)` | 第二参数是长度；到末尾为止，不是右端点。substr 复制结果，erase 移动后续字符。 |
| `s.find(t,pos)` | 从 pos 开始查找子串，失败用 `string::npos` 判断，不存入普通 int 后与 -1 混用。 |
| `deque` / `stack` / `queue` | deque 两端增删 O(1)，支持下标；stack 用 top，queue 用 front / back，push / pop 不返回元素。deque 两端插入使迭代器失效，旧元素引用仍有效；中间插删使两者均失效。 |
| `bitset<N>` | 位编号从低位 0 起，容量是编译期常量；`to_ullong()` 的数值装不下会抛异常。批量位运算与 DP 用法见 [bitset 技巧](../02-数据结构/bitset与字并行.cpp)。 |
