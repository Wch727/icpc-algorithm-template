# 与 OI Wiki 的缺项对照

> 历史快照：数量、待办和编译情况仅描述该阶段，不代表当前版本。当前状态与打印方式统一见 [维护说明](../维护说明.md)。本地题解与缓存未随仓库发布。

核对日期：2026-10-02。对照 [OI Wiki 算法目录](https://oi-wiki.org/) 与当前 153 份 cpp、接口说明和结论速查；重点检查候选项是否已有实际实现，不只比较文件名。本表列值得考虑的主要缺项，不把网站所有页面都当成独立模板，也不计算虚假的覆盖百分比。

补充优先级是结合 ICPC 用途、现有替代方案和打印篇幅做的判断，不是 OI Wiki 的推荐顺序。此次只整理缺项，没有添加算法代码。

后续状态：用户确认先补优先 6 项及 WQS、Burnside 说明，这部分已完成，见 [优先六项补充说明](优先六项补充说明.md)。以下保留补充前的差异快照；该阶段模板数为 159，第二批仍待按需补充。

## 第一批值得考虑

| 内容 | 当前缺口及用途 | 建议收录方式 |
|---|---|---|
| 01 Trie | 当前 Trie 是 62 字符版，没有整数按位插入、删除、最大异或查询；线性基求集合子集异或，也不能代替两数异或查询 | 一份短实现，先保留非持久化版；参见 [Trie](https://oi-wiki.org/string/trie/) |
| BM + 快速线性递推 | 现有矩阵递推要求已知系数，复杂度 O(k³ log n)；缺从前若干项恢复递推、用多项式取模求第 n 项 | 同文件两区：先 [BM](https://oi-wiki.org/math/berlekamp-massey/)，再 [快速递推](https://oi-wiki.org/math/poly/linear-recurrence/)；先采用朴素多项式乘法的 O(k² log n) 第 n 项实现 |
| 类欧几里得 / floor_sum | 整除分块解决的是另一种和式；当前没有 Σ floor((ai+b)/m) 的快速求和 | 保留普通 floor_sum，先不扩展高次带权版本；参见 [类欧几里得](https://oi-wiki.org/math/number-theory/euclidean/) |
| 树同构 / 树哈希 | 有重心和树上遍历，但没有判断两棵树形状相同的完整流程 | 精确 AHU 与随机哈希选一种作为主体；无根树要处理根的选择。参见 [AHU](https://oi-wiki.org/graph/tree-ahu/) |
| 自适应 Simpson | 当前没有数值积分，复杂面积或体积题缺工具 | 一份短实现，说明误差、分段与递归限制；参见 [数值积分](https://oi-wiki.org/math/numerical/integral/) |
| 左偏树 / 可并堆 | 手写堆不支持高效合并两个堆；已有 Treap 和 LCT 也不直接提供这个接口 | merge、插入、取顶、删顶即可；参见 [左偏树](https://oi-wiki.org/ds/leftist-tree/) |
| Slope Trick | 现有斜率优化维护直线集合，不是维护凸分段线性函数 | 堆维护拐点的基本操作加一例，注明适用函数形态；参见 [Slope Trick](https://oi-wiki.org/dp/opt/slope-trick/) |
| WQS 二分 | 缺把“恰好选 k 次/分 k 段”的数量约束变为罚项的 DP 优化方法 | 优先收录思路、凸性条件、同值计数规则和恢复答案公式；具体 DP 依题而定。参见 [WQS](https://oi-wiki.org/dp/opt/wqs-binary-search/) |

BM 能恢复有限前缀的递推关系，但不能凭任意短前缀断言后续也满足它；用于未知无限序列时，需要递推阶数上界等依据。WQS 也不能只因计数随罚项单调就直接恢复所有恰好 k 的最优值，须满足相应凸性条件。

## 第二批：根据题型取舍

| 章节 | 尚缺内容 | 现有实现与收录取舍 |
|---|---|---|
| 02 | [Segment Tree Beats](https://oi-wiki.org/ds/seg-beats/) | 当前加、乘、开方树没有区间 chmin/chmax；实现较长，先收最需要的操作组合 |
| 02 | 树状数组套权值线段树等树套树 | 整体二分处理离线带修改第 k 小，主席树处理静态区间；尚缺相应在线带修改实现。若暂时没有在线需求，可先不补 |
| 02 | 线段树分裂 | 现有文件只有合并；按位置区间拆出部分权值树可增加一个函数，必须明确节点所有权 |
| 02 | 带修改莫队、树上莫队、回滚莫队 | 当前莫队只求普通静态区间不同数；按使用频率选变体，不必把 [所有莫队变体](https://oi-wiki.org/) 都打印 |
| 03 | [Lyndon 分解 / Duval](https://oi-wiki.org/string/lyndon/) | 当前最小表示法不能直接输出 Lyndon 分解；实现短，应用说明可紧凑 |
| 03 | [广义 SAM](https://oi-wiki.org/string/general-sam/) | 当前 SAM 为单串构建；多串字典及其子串集合需要另行处理，普通拼接并不自动等价 |
| 04 | [斯坦纳树](https://oi-wiki.org/graph/steiner-tree/) | 缺少连接少量指定终端的最小代价子图：子集 DP + 最短路；普通 MST 不能直接替代 |
| 04 | [Stoer–Wagner](https://oi-wiki.org/graph/stoer-wagner/) | 有 s-t 最大流/最小割，缺无向图全局最小割专用实现 |
| 04 | [Hopcroft–Karp](https://oi-wiki.org/graph/graph-matching/bigraph-match/) | 缺专用实现，但匈牙利与 Dinic 已能解二分图匹配；属于性能和常数上的补充，优先级低于全新能力 |
| 05 | [exLucas](https://oi-wiki.org/math/number-theory/lucas/) | 当前组合数和 Lucas 要求素数模数；合数模组合数缺实现，CRT 本身不能代替素数幂部分 |
| 05 | 多项式幂、平方根、除法/取模 | 当前仅卷积、逆、ln、exp；幂可以复用 ln/exp，但仍须处理零前缀等条件；平方根和除法也需补接口 |
| 05 | [多点求值与快速插值](https://oi-wiki.org/math/poly/multipoint-eval-interpolation/) | 当前拉格朗日插值不能覆盖同样的大规模批处理需求；暂缓，依赖多项式除法与乘积树 |
| 05 | [Burnside / Pólya](https://oi-wiki.org/math/combinatorics/polya/) | 缺旋转、翻转等对称下的轨道计数说明；更适合定理加项链例子，不一定单独封装成类 |
| 05 | [Stirling 数](https://oi-wiki.org/math/combinatorics/stirling/) | 结论速查已有两类递推性质，缺专门实现；普通 O(n²) 表很容易现场写，先不为此新增大模板 |
| 05 | [Min_25 筛](https://oi-wiki.org/math/number-theory/min-25/) | 杜教筛当前求 phi/mu 前缀，不能直接代替一般目标积性函数求和；模板较长，针对数论训练再补 |

## 可以只补说明的内容

WQS、Burnside/Pólya、扩展欧拉定理的指数处理、LGV 引理、Prüfer 序列的计数用途、同余最短路建模等，优先写判据和前提。已有 Dijkstra、行列式、组合数等代码可以复用，不必每个结论再放一份完整实现。这些条目在 [OI Wiki 目录](https://oi-wiki.org/) 中有相应专题。

几何还可以按 ICPC 使用需要补圆交面积、圆与多边形交面积等实用函数；当前圆文件只含交点和公切线。本项是根据现有接口提出的延伸，不按 OI Wiki 一级标题做缺项统计。

## 暂缓与已覆盖

后缀树、Main–Lorentz、支配树、一般图带权匹配、动态点分治、Top Tree、ETT、PQ 树、洲阁筛、单纯形、多项式复合等仍有未覆盖内容。它们属于更专门的训练方向；对当前压缩纸张的目标，建议先不整批加入。对应专题可从 [OI Wiki 目录](https://oi-wiki.org/) 查到，线性规划另见 [单纯形法](https://oi-wiki.org/math/simplex/)。

不重补用户已经主动删掉的排序、普通二分、一维前缀和等基础内容。SAM、PAM、SA、AC、LCT、可持久化序列 Treap、圆方树、虚树、长链剖分、CDQ、整体二分、回滚连通性、FWT、SOS、模/异或高斯、上下界流等已有实现，不再算成缺项。

两个容易误判的地方：费用流已经有势能 + Dijkstra；向量基础已经有一般多边形内点判断。文件名没有出现算法名不表示没有覆盖。

若只追加一轮，建议先选 01 Trie、BM + 快速递推、floor_sum、树同构、自适应 Simpson、左偏树；WQS 先补方法说明，Slope Trick 则按你是否准备练这类题决定。
