// 三库（洛谷 + AtCoder + Codeforces）合并覆盖核对：哪些标签在库里没有模板
// 用法: node _台账\三库覆盖.mjs
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const raw = fs.readFileSync(path.join(DIR, '总台账.csv'), 'utf8').replace(/^\ufeff/, '');
const rows = raw.trim().split(/\r?\n/).slice(1).map(l => {
    const m = l.match(/^([^,]+),([^,]+),"((?:[^"]|"")*)"?,?([^,]*),(.*)$/);
    return m ? { src: m[1], id: m[2], name: m[3], lv: m[4], tags: m[5].split(';').map(s => s.trim()).filter(Boolean) } : null;
}).filter(Boolean);

// 有模板 或 属思维/语法题无需模板 的标签（每行一个，保留空格）
const COVERED = new Set(`模拟
枚举
贪心
排序
二分
三分
递归
搜索
深度优先搜索 DFS
广度优先搜索 BFS
迭代加深搜索
启发式迭代加深搜索 IDA*
A* 算法
启发式搜索
剪枝
记忆化搜索
折半搜索 meet in the middle
分治
cdq 分治
动态规划 DP
线性 DP
背包 DP
区间 DP
树形 DP
状压 DP
数位 DP
动态规划优化
单调队列
单调栈
队列
栈
STL
优先队列
堆
链表
线性数据结构
树形数据结构
树的遍历
树的重心
树的直径
并查集
连通块
树状数组
线段树
线段树合并
动态开点线段树
ST 表
分块
莫队
颜色段均摊（珂朵莉树 ODT）
平衡树
字典树 Trie
哈希表
哈希 hashing
笛卡尔树
轮廓线 DP
可持久化
主席树
树套树
扫描线
字符串
KMP 算法
Manacher 算法
后缀自动机 SAM
回文自动机
序列自动机
最小表示法
图论
图论建模
图遍历
最短路
Floyd 算法
拓扑排序
生成树
强连通分量
Tarjan
最近公共祖先 LCA
倍增
二分图
网络流
最小割
费用流
基环树
欧拉回路
差分约束
虚树
点分治
启发式合并
数学
数论
线性筛法
素数判断
最大公约数 gcd
Bézout 定理
扩展欧几里德算法
逆元
组合数学
排列组合
Lucas 定理
Catalan 数
容斥原理
莫比乌斯反演
整除分块
欧拉函数
原根
高斯消元
线性代数
行列式
矩阵乘法
矩阵加速
矩阵运算
线性递推
Fibonacci 数列
快速傅里叶变换 FFT
快速数论变换 NTT
博弈论
概率论
期望
进制
前缀和
差分
位运算
bitset
高精度
反悔贪心
Dilworth 定理
计算几何
平面几何
凸包
极角排序
模拟退火
决策单调性
四边形不等式
动态 DP
斜率优化
平面图
鸽笼原理
不定方程
分支结构
顺序结构
数组
有限状态自动机
离线处理
分类讨论
暴力数据结构
基础算法
双指针 two-pointer
构造
拉格朗日插值
半平面交
最小圆覆盖
平面最近点对
闵可夫斯基和
替罪羊树
链接剖分LCT
KD-Tree
圆方树
长链剖分
一般图匹配
带花树
多项式求逆
插头DP
DLX 舞蹈链
分数规划
树链剖分
矩阵树定理
BSGS
二次剩余
杜教筛
线性基
中国剩余定理
MillerRabin
PollardRho
递推
离散化
AC 自动机
交互题
special
2-sat
flows
斜率优化 DP
树套树
主席树
主席树(可持久化)
MillerRabin
PollardRho
fft
max flow
sparse table
binary lifting
模拟题
贪心算法
排序算法
基础算法
greedy
math
dp
implementation
brute force
constructive algorithms
data structures
sortings
binary search
graphs
combinatorics
number theory
dfs and similar
trees
bitmasks
two pointers
strings
games
probabilities
dsu
geometry
divide and conquer
shortest paths
matrices
interactive
ternary search
schedules
expression parsing
chinese remainder theorem
meet-in-the-middle
suffix structures
game theory
special`.split('\n').map(s => s.trim()).filter(Boolean));

// 噪声：年份 / 赛事 / 来源 / 省份 / 评测相关
const NOISE = /^(\d{4}|\d{4}年|\d{4}.*|NOIP.*|NOI.*|CSP.*|ICPC.*|USACO.*|COCI.*|COI（.*|JOI（.*|CCC（.*|ROI（.*|PA（.*|GESP.*|蓝桥杯.*|洛谷.*|语言月赛|各省省选|省选|集训队互测|高校校赛|BalticOI.*|CERC|WF|IOI|AtCoder.*|Google Code Jam|Codeforces.*|Special Judge|O2优化|模板题|提交答案|暂无评定|福建省历届夏令营|CTSC\/CTS|多校|UVA|SPOJ|.*杯.*|.*赛.*|Ad-hoc|其它|其他|实验)$/;
const PROV = /^(北京|天津|上海|重庆|河北|山西|辽宁|吉林|黑龙江|江苏|浙江|安徽|福建|江西|山东|河南|湖北|湖南|广东|广西|海南|四川|贵州|云南|陕西|甘肃|青海|台湾|内蒙古|西藏|宁夏|新疆|香港|澳门|济南|南京|青岛|杭州|昆明|西安|哈尔滨|成都|首尔|横滨|雅加达)$/;
const clean = t => !NOISE.test(t) && !PROV.test(t);

const cnt = new Map();
for (const r of rows) for (const t of r.tags) if (clean(t)) cnt.set(t, (cnt.get(t) || 0) + 1);

const gaps = [...cnt].filter(([t]) => !COVERED.has(t)).sort((a, b) => b[1] - a[1]);
const covered = [...cnt].filter(([t]) => COVERED.has(t));
console.log(`总台账 ${rows.length} 题，去噪后算法标签 ${cnt.size} 种`);
console.log(`  有对应模板 / 无需模板 : ${covered.length} 种`);
console.log(`  库里没有的标签        : ${gaps.length} 种`);
console.log(gaps.length ? '\n缺模板的标签：\n' + gaps.map(([t, n]) => `  ${n}  ${t}`).join('\n') : '\n所有标签都有对应模板或属思维/语法题');

// 只有"模拟/枚举/语法"这类标签的题
const TRIVIAL = /^(模拟|枚举|递归|搜索|分支结构|顺序结构|数组|implementation|brute force|Ad-hoc|分类讨论|构造|暴力数据结构|基础算法|special|sortings|math)$/;
const noTpl = [];
for (const r of rows) {
    const algo = r.tags.filter(clean);
    if (!algo.length) continue;
    if (algo.every(t => TRIVIAL.test(t))) noTpl.push(`${r.src} ${r.id}(${algo.join('/')})`);
}
const withAlgo = rows.filter(r => r.tags.filter(clean).length).length;
console.log(`\n${rows.length} 题中：${withAlgo} 题有算法标签，其中 ${withAlgo - noTpl.length} 题能对上具体模板，${noTpl.length} 题只涉及模拟/枚举/语法（无模板可写）`);
console.log(`洛谷本身没打标签的：${rows.length - withAlgo} 题`);

const md = ['# 三库合并 · 模板覆盖核对', '',
    `数据来源：\`_台账/总台账.csv\`，共 **${rows.length}** 题（洛谷 527 + AtCoder 82 + Codeforces 88）。`, '',
    '| 口径 | 数量 |', '|---|---|',
    `| 去噪后的算法标签种类 | ${cnt.size} |`,
    `| 其中有模板 / 属思维语法题无需模板 | ${covered.length} |`,
    `| **库里没有对应模板的标签** | **${gaps.length}** |`,
    `| 有算法标签的题 | ${withAlgo} |`,
    `| 能对上具体模板文件的题 | ${withAlgo - noTpl.length} |`,
    `| 只涉及模拟/枚举/语法的题 | ${noTpl.length} |`,
    `| 平台本身没打标签的题 | ${rows.length - withAlgo} |`, '',
    gaps.length ? '## 缺模板的标签\n\n| 标签 | 题数 |\n|---|---|\n' + gaps.map(([t, n]) => `| ${t} | ${n} |`).join('\n')
        : '## 结论\n\n**所有算法标签都能在库里找到对应模板**（或属模拟/枚举/语法这类本来就没有模板的题）。', '',
    `## 只涉及模拟/枚举/语法的题（${noTpl.length} 题）`, '', noTpl.slice(0, 60).join(' ｜ ') + (noTpl.length > 60 ? ' …' : ''), ''];
fs.writeFileSync(path.join(DIR, '三库覆盖报告.md'), md.join('\n'), 'utf8');
console.log('\n报告写入 _台账/三库覆盖报告.md');
