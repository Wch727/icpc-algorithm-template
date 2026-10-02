// 覆盖分析：把题目台账的标签统计出来，对照模板库给出缺口清单
// 用法: node _台账\覆盖分析.mjs   ->  生成 _台账\标签覆盖报告.md
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const ROOT = path.join(DIR, '..');
const raw = fs.readFileSync(path.join(DIR, '题目台账.csv'), 'utf8').replace(/^\ufeff/, '');
const rows = raw.trim().split(/\r?\n/).slice(1).map(l => {
    const m = l.match(/^([^,]+),("(?:[^"]|"")*"|[^,]*),(\d+),([^,]*),(.*)$/);
    if (!m) return null;
    return {
        id: m[1],
        name: m[2].replace(/^"|"$/g, '').replace(/""/g, '"'),
        diff: +m[3], diffName: m[4],
        tags: m[5].split(';').map(s => s.trim()).filter(Boolean),
    };
}).filter(Boolean);

// 非算法标签（年份/赛事/来源/省份/评测相关）过滤
const NOISE = /^(\d{4}|NOIP.*|NOI.*|CSP.*|ICPC.*|USACO.*|COCI.*|PA（.*|JOI（.*|COI（.*|CCC（.*|ROI（.*|GESP.*|蓝桥杯.*|洛谷.*|语言月赛|各省省选|集训队互测|高校校赛|BalticOI.*|CERC|WF|IOI|AtCoder.*|Google Code Jam|Codeforces.*|Special Judge|O2优化|模板题|提交答案|交互题|暂无评定|Ad-hoc|分类讨论|均摊分析|福建省历届夏令营|CTSC\/CTS|省选|根号分治|基础算法|暴力数据结构|二维偏序|离线处理)$/;
const PROV = /^(北京|天津|上海|重庆|河北|山西|辽宁|吉林|黑龙江|江苏|浙江|安徽|福建|江西|山东|河南|湖北|湖南|广东|广西|海南|四川|贵州|云南|陕西|甘肃|青海|台湾|内蒙古|西藏|宁夏|新疆|香港|澳门|济南|南京|青岛|杭州|昆明|西安|哈尔滨|成都|首尔|横滨|雅加达)$/;

const cnt = new Map();
for (const r of rows) for (const t of r.tags) {
    if (NOISE.test(t) || PROV.test(t)) continue;
    cnt.set(t, (cnt.get(t) || 0) + 1);
}

// 标签 -> 库内对应文件（'—' 表示暂缺）
const MAP = {
    '模拟': '（无需模板）', '枚举': '（无需模板）', '贪心': '01-基础与技巧/反悔贪心.cpp', '排序': '01-基础与技巧/快速排序与快速选择.cpp',
    '二分': '01-基础与技巧/二分答案.cpp', '三分': '01-基础与技巧/三分法.cpp', '递归': '07-搜索/DFS与剪枝.cpp',
    '搜索': '07-搜索/DFS与剪枝.cpp', '深度优先搜索 DFS': '07-搜索/DFS与剪枝.cpp', '广度优先搜索 BFS': '07-搜索/BFS最短路.cpp',
    '迭代加深搜索': '07-搜索/迭代加深与IDA星.cpp', '启发式迭代加深搜索 IDA*': '07-搜索/迭代加深与IDA星.cpp',
    'A* 算法': '07-搜索/A星与K短路.cpp', '启发式搜索': '07-搜索/A星与K短路.cpp', '剪枝': '07-搜索/DFS与剪枝.cpp',
    '记忆化搜索': '01-基础与技巧/记忆化搜索.cpp', '折半搜索 meet in the middle': '07-搜索/折半搜索.cpp',
    '分治': '01-基础与技巧/分治.cpp', 'cdq 分治': '01-基础与技巧/分治.cpp',
    '动态规划 DP': '06-动态规划/（九个文件）', '线性 DP': '06-动态规划/最长上升子序列LIS.cpp',
    '背包 DP': '06-动态规划/背包(01完全多重).cpp', '区间 DP': '06-动态规划/区间DP.cpp', '树形 DP': '06-动态规划/树形DP.cpp',
    '状压 DP': '06-动态规划/状压DP.cpp', '数位 DP': '06-动态规划/数位DP.cpp', '动态规划优化': '06-动态规划/单调队列优化DP.cpp',
    '单调队列': '01-基础与技巧/单调队列.cpp', '单调栈': '01-基础与技巧/单调栈.cpp', '队列': '01-基础与技巧/STL容器速查.cpp',
    '栈': '01-基础与技巧/STL容器速查.cpp', 'STL': '01-基础与技巧/STL容器速查.cpp', '优先队列': '02-数据结构/手写堆.cpp',
    '堆': '02-数据结构/手写堆.cpp', '链表': '02-数据结构/链表.cpp', '线性数据结构': '02-数据结构/链表.cpp',
    '树形数据结构': '04-图论/树的遍历与dfs序.cpp', '树的遍历': '04-图论/树的遍历与dfs序.cpp', '树的重心': '04-图论/树的重心与直径.cpp',
    '树的直径': '04-图论/树的重心与直径.cpp', '并查集': '02-数据结构/并查集.cpp', '连通块': '02-数据结构/并查集.cpp',
    '树状数组': '02-数据结构/树状数组.cpp', '线段树': '02-数据结构/线段树(区间加区间和).cpp', '线段树合并': '02-数据结构/线段树合并.cpp',
    'ST 表': '02-数据结构/ST表.cpp', '分块': '02-数据结构/分块.cpp', '莫队': '02-数据结构/莫队算法.cpp',
    '颜色段均摊（珂朵莉树 ODT）': '02-数据结构/珂朵莉树ODT.cpp', '平衡树': '02-数据结构/平衡树Treap.cpp',
    '字典树 Trie': '02-数据结构/Trie字典树.cpp', '哈希表': '02-数据结构/哈希表.cpp', '哈希 hashing': '03-字符串/字符串哈希.cpp',
    '笛卡尔树': '02-数据结构/笛卡尔树.cpp', '轮廓线 DP': '06-动态规划/插头DP(轮廓线).cpp',
    '可持久化': '02-数据结构/主席树(可持久化线段树).cpp', '主席树': '02-数据结构/主席树(可持久化线段树).cpp',
    '树套树': '02-数据结构/树套树(树状数组套主席树).cpp', '扫描线': '02-数据结构/扫描线(矩形面积并).cpp',
    '字符串': '03-字符串/（六个文件）', 'KMP 算法': '03-字符串/KMP.cpp', 'Manacher 算法': '03-字符串/Manacher.cpp',
    '后缀自动机 SAM': '03-字符串/后缀自动机SAM.cpp', '回文自动机': '03-字符串/回文自动机PAM.cpp',
    '图论': '04-图论/（十九个文件）', '图论建模': '04-图论/存图(vector邻接表).cpp', '图遍历': '04-图论/存图(vector邻接表).cpp',
    '最短路': '04-图论/Dijkstra.cpp', 'Floyd 算法': '04-图论/Floyd.cpp', '拓扑排序': '04-图论/拓扑排序.cpp',
    '生成树': '04-图论/最小生成树Kruskal.cpp', '强连通分量': '04-图论/Tarjan强连通分量.cpp', 'Tarjan': '04-图论/Tarjan强连通分量.cpp',
    '最近公共祖先 LCA': '04-图论/LCA(倍增).cpp', '倍增': '04-图论/LCA(倍增).cpp', '二分图': '04-图论/二分图判定.cpp',
    '网络流': '04-图论/网络流Dinic.cpp', '最小割': '04-图论/网络流Dinic.cpp', '费用流': '04-图论/费用流MCMF.cpp',
    '基环树': '04-图论/基环树.cpp', '欧拉回路': '04-图论/欧拉路径与欧拉回路.cpp', '差分约束': '04-图论/差分约束.cpp',
    '虚树': '04-图论/虚树.cpp', '点分治': '04-图论/点分治.cpp', '启发式合并': '04-图论/树上启发式合并DSUonTree.cpp',
    '数学': '05-数学/（二十个文件）', '数论': '05-数学/线性筛与欧拉函数.cpp', '线性筛法': '05-数学/线性筛与欧拉函数.cpp',
    '素数判断': '05-数学/线性筛与欧拉函数.cpp', '最大公约数 gcd': '05-数学/扩展欧几里得.cpp', 'Bézout 定理': '05-数学/扩展欧几里得.cpp',
    '扩展欧几里德算法': '05-数学/扩展欧几里得.cpp', '逆元': '05-数学/乘法逆元.cpp', '组合数学': '05-数学/组合数与Lucas定理.cpp',
    '排列组合': '05-数学/组合数与Lucas定理.cpp', 'Lucas 定理': '05-数学/组合数与Lucas定理.cpp', 'Catalan 数': '05-数学/卡特兰数.cpp',
    '容斥原理': '05-数学/容斥原理与排列组合.cpp', '莫比乌斯反演': '05-数学/莫比乌斯反演与整除分块.cpp', '整除分块': '05-数学/莫比乌斯反演与整除分块.cpp',
    '欧拉函数': '05-数学/线性筛与欧拉函数.cpp', '原根': '05-数学/原根与阶.cpp', '高斯消元': '05-数学/高斯消元.cpp',
    '线性代数': '05-数学/行列式与矩阵树定理.cpp', '行列式': '05-数学/行列式与矩阵树定理.cpp', '矩阵乘法': '05-数学/矩阵快速幂.cpp',
    '矩阵加速': '05-数学/矩阵快速幂.cpp', '矩阵运算': '05-数学/矩阵快速幂.cpp', '线性递推': '05-数学/矩阵快速幂.cpp',
    'Fibonacci 数列': '05-数学/矩阵快速幂.cpp', '快速傅里叶变换 FFT': '05-数学/FFT.cpp', '快速数论变换 NTT': '05-数学/NTT.cpp',
    '博弈论': '05-数学/博弈论(Nim与SG).cpp', '概率论': '06-动态规划/概率期望DP.cpp', '期望': '06-动态规划/概率期望DP.cpp',
    '进制': '01-基础与技巧/进制转换.cpp', '前缀和': '01-基础与技巧/前缀和与差分.cpp', '差分': '01-基础与技巧/前缀和与差分.cpp',
    '离散化': '01-基础与技巧/离散化.cpp', '双指针 two-pointer': '01-基础与技巧/双指针.cpp',
    '递推': '06-动态规划/递推与线性DP.cpp', '构造': '（思维题，无固定模板）',
    '位运算': '01-基础与技巧/位运算技巧.cpp', 'bitset': '09-其他/bitset优化技巧.cpp', '高精度': '01-基础与技巧/高精度BigInt.cpp',
    '反悔贪心': '01-基础与技巧/反悔贪心.cpp', 'Dilworth 定理': '06-动态规划/最长上升子序列LIS.cpp',
    '计算几何': '08-计算几何/（五个文件）', '平面几何': '08-计算几何/向量基础.cpp', '凸包': '08-计算几何/凸包Andrew.cpp',
    '极角排序': '08-计算几何/凸包Andrew.cpp', '模拟退火': '07-搜索/模拟退火与爬山.cpp', '决策单调性': '06-动态规划/四边形不等式与决策单调性.cpp',
    '四边形不等式': '06-动态规划/四边形不等式与决策单调性.cpp', '动态 DP': '06-动态规划/动态DP.cpp', '斜率优化': '06-动态规划/斜率优化DP.cpp',
    '平面图': '04-图论/（平面图欧拉公式）', '鸽笼原理': '（思维题，无需模板）', '不定方程': '05-数学/扩展欧几里得.cpp',
    '分支结构': '（语法基础，无需模板）', '顺序结构': '（语法基础，无需模板）', '数组': '（语法基础，无需模板）',
};

const exists = p => { try { return fs.existsSync(path.join(ROOT, p.split('（')[0].trim())); } catch { return false; } };

const list = [...cnt].sort((a, b) => b[1] - a[1]);
const gaps = [], covered = [];
for (const [tag, n] of list) {
    const file = MAP[tag];
    if (file) covered.push([tag, n, file]); else gaps.push([tag, n]);
}

const diffCnt = new Map();
for (const r of rows) diffCnt.set(r.diffName, (diffCnt.get(r.diffName) || 0) + 1);
const order = ['入门', '普及-', '普及', '普及+/提高-', '提高', '提高+/省选-', '省选/NOI-', 'NOI/NOI+/CTSC'];

const md = [];
md.push('# 洛谷做题台账 · 标签覆盖报告\n');
md.push(`统计范围：\`D:\\code\` 里 ${rows.length} 道有代码的洛谷题目（题号取自文件名）。数据文件：\`_台账/题目台账.csv\`。\n`);
md.push('## 难度分布\n');
md.push('| 难度 | 题数 |\n|---|---|');
for (const d of order) if (diffCnt.has(d)) md.push(`| ${d} | ${diffCnt.get(d)} |`);
md.push(`\n合计 **${rows.length}** 题。\n`);
md.push('## 算法标签 → 模板对照\n');
md.push('| 算法标签 | 题数 | 库内对应 |\n|---|---|---|');
for (const [tag, n, file] of covered) md.push(`| ${tag} | ${n} | ${file}${exists(file) ? '' : ' ⚠️待补'} |`);
md.push('\n## 台账里出现、但库里还没有对应模板的标签\n');
md.push(gaps.length ? gaps.map(([t, n]) => `- ${t}（${n} 题）`).join('\n') : '- 无');
md.push('\n');
fs.writeFileSync(path.join(DIR, '标签覆盖报告.md'), md.join('\n'), 'utf8');

// ---- 题目级对照：每道题的算法标签 → 模板文件 ----
const perRow = [];
let fullCovered = 0, withAlgo = 0;
const usedFiles = new Set();
for (const r of rows) {
    const algo = r.tags.filter(t => !(NOISE.test(t) || PROV.test(t)));
    const files = [];
    for (const t of algo) {
        const f = MAP[t];
        if (!f) continue;
        if (f.startsWith('（')) continue;          // 无需模板 / 思维题
        files.push(f);
        for (const one of f.split('、')) {
            const clean = one.split('（')[0].trim();
            if (clean && fs.existsSync(path.join(ROOT, clean))) usedFiles.add(clean);
        }
    }
    const uniq = [...new Set(files)];
    if (algo.length) withAlgo++;
    if (uniq.length) fullCovered++;
    perRow.push({ ...r, algo, files: uniq.join(' ') });
}
const perOut = ['id,名称,难度,算法标签,对应模板'];
for (const r of perRow) {
    perOut.push([r.id, '"' + r.name.replace(/"/g, '""') + '"', r.diffName, r.algo.join(';'), r.files].join(','));
}
fs.writeFileSync(path.join(DIR, '题目-模板对照.csv'), '\ufeff' + perOut.join('\n') + '\n', 'utf8');

console.log(`报告已生成：${covered.length} 个标签有模板对应，${gaps.length} 个标签暂缺`);
const noAlgoCnt = perRow.filter(r => !r.algo.length).length;
const noTplCnt = perRow.filter(r => r.algo.length && !r.files).length;
// 无模板的那批，统计它们到底只涉及哪些标签
const onlyTags = new Set();
for (const r of perRow) if (r.algo.length && !r.files) r.algo.forEach(t => onlyTags.add(t));
console.log(`题目级结论：共 ${rows.length} 题 →`);
console.log(`  有具体模板可对应 : ${fullCovered} 题（用到 ${usedFiles.size} 个模板文件）`);
console.log(`  只涉及无模板可写的标签 : ${noTplCnt} 题（标签仅 ${[...onlyTags].join('、')}）`);
console.log(`  洛谷本身没打算法标签 : ${noAlgoCnt} 题`);
console.log('暂缺：' + (gaps.map(([t, n]) => t + '(' + n + ')').join('、') || '无'));
