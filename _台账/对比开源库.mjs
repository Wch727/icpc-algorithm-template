// 和主流开源算法模板库对比覆盖面
// 用法: node _台账\对比开源库.mjs
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const ROOT = path.join(DIR, '..');
const UA = { 'User-Agent': 'Mozilla/5.0', 'Accept': 'application/vnd.github+json' };

const REPOS = [
    ['KACTL（ICPC Notebook 标杆）', 'kth-competitive-programming/kactl', 'main'],
    ['AtCoder Library', 'atcoder/ac-library', 'master'],
    ['cplib-cpp（日本选手库）', 'hitonanode/cplib-cpp', 'master'],
    ['OI-wiki 模板整理（CP-algo 风格）', 'cp-algorithms/cp-algorithms', 'main'],
];

async function tree(repo, branch) {
    const url = `https://api.github.com/repos/${repo}/git/trees/${branch}?recursive=1`;
    try {
        const r = await fetch(url, { headers: UA });
        if (!r.ok) return { err: 'HTTP ' + r.status, files: [] };
        const j = await r.json();
        return { files: (j.tree || []).filter(t => t.type === 'blob').map(t => t.path.toLowerCase()) };
    } catch (e) { return { err: e.message, files: [] }; }
}

// 我们库里的东西（文件名 + 文件内容，因为我们的文件名是中文）
const ourNames = [];
for (const d of fs.readdirSync(ROOT)) {
    if (!/^\d\d-/.test(d)) continue;
    for (const f of fs.readdirSync(path.join(ROOT, d))) {
        if (!f.endsWith('.cpp')) continue;
        ourNames.push(f.toLowerCase());
        try { ourNames.push(fs.readFileSync(path.join(ROOT, d, f), 'utf8').toLowerCase()); } catch (e) { }
    }
}
const ourAll = ourNames.join(' ');

// 中文别名：我们的文件名是中文，得用中文关键词匹配
const ZH = {
    '线段树 / 懒标记': ['线段树'], '树状数组 BIT': ['树状数组'], '平衡树 Treap/Splay': ['平衡树', 'treap', 'splay'],
    'Link-Cut Tree': ['lct', '链接剖分'], '主席树 / 可持久化': ['主席树', '可持久化'],
    '线段树合并 / 动态开点': ['线段树合并', '动态开点'], '树链剖分 HLD': ['树链剖分'],
    '树分治 / 点分治': ['点分治'], '虚树 / 圆方树': ['虚树', '圆方树'], '莫队 / 分块': ['莫队', '分块'],
    'KD-Tree': ['kd-tree'], '并查集 / 带权': ['并查集'], '字符串哈希': ['字符串哈希'],
    'KMP / Z / Manacher': ['kmp', 'manacher', 'z函数'], '后缀数组 / 后缀自动机': ['后缀数组', '后缀自动机'],
    'AC 自动机': ['ac自动机'], '回文自动机 PAM': ['回文自动机'],
    'Dijkstra / SPFA / Bellman': ['dijkstra', 'spfa', 'bellman'], 'Floyd / 传递闭包': ['floyd'],
    '最小生成树 / 最小树形图': ['最小生成树', '最小树形图'], '网络流 Dinic / ISAP': ['dinic', '网络流'],
    '费用流 MCMF': ['费用流'], '上下界网络流': ['上下界'], '二分图匹配（匈牙利/KM）': ['匈牙利', 'km算法', '二分图'],
    '一般图匹配（带花树）': ['带花树'], '2-SAT': ['2-sat'], 'Tarjan 强连通 / 割点': ['tarjan', '强连通', '割点'],
    '欧拉路 / 哈密顿': ['欧拉'], 'LCA / 倍增': ['lca', '倍增'], '树的重心 / 直径': ['重心', '直径'],
    '线性筛 / 积性函数': ['线性筛', '欧拉函数'], '数论分块 / 杜教筛': ['整除分块', '杜教筛', '莫比乌斯'],
    'exgcd / 逆元 / CRT': ['扩展欧几里得', '逆元', '中国剩余'], 'BSGS / 离散对数': ['bsgs'],
    'Miller-Rabin / Pollard-Rho': ['millerrabin', 'pollard'], 'FFT / NTT / 多项式': ['fft', 'ntt', '多项式'],
    '高斯消元 / 行列式 / 线性基': ['高斯消元', '行列式', '线性基'], '博弈论 SG / Nim': ['博弈论', 'nim'],
    '计算几何：凸包': ['凸包'], '计算几何：半平面交': ['半平面交'],
    '计算几何：旋转卡壳 / 最小圆覆盖': ['旋转卡壳', '最小圆覆盖', '最近点对'],
    '矩阵快速幂 / 线性递推': ['矩阵快速幂', '矩阵优化'], '插头 DP / 轮廓线': ['插头dp', '轮廓线'],
    '动态 DP / 斜率优化': ['动态dp', '斜率优化', '李超树'], '莫比乌斯反演': ['莫比乌斯'],
    'Simpson / 数值积分': [], '模拟退火 / 爬山': ['模拟退火', '爬山'], '分数规划 / 三分': ['分数规划', '三分'],
};

const TOPICS = [
    ['线段树 / 懒标记', ['segment', 'lazy', 'segtree']],
    ['树状数组 BIT', ['fenwick', 'bit', 'binary indexed']],
    ['平衡树 Treap/Splay', ['treap', 'splay', 'bbst', 'balanced']],
    ['Link-Cut Tree', ['linkcut', 'link-cut', 'lct']],
    ['主席树 / 可持久化', ['persistent', 'chairman']],
    ['线段树合并 / 动态开点', ['segment tree merging', 'dynamic segment', 'merge']],
    ['树链剖分 HLD', ['heavy-light', 'hld', 'heavylight', 'tree decomposition']],
    ['树分治 / 点分治', ['centroid', 'divide and conquer on tree', 'tree divide']],
    ['虚树 / 圆方树', ['virtual tree', 'auxiliary tree', 'block-cut', 'bcc', 'cactus']],
    ['莫队 / 分块', ['mo\'s', 'mo algorithm', 'sqrt decomposition', 'block']],
    ['KD-Tree', ['kdtree', 'kd-tree', 'kd tree']],
    ['并查集 / 带权', ['dsu', 'union find', 'disjoint']],
    ['字符串哈希', ['hash']],
    ['KMP / Z / Manacher', ['kmp', 'z-function', 'zfunction', 'manacher', 'palindrom']],
    ['后缀数组 / 后缀自动机', ['suffix array', 'suffixarray', 'suffix automaton', 'sam']],
    ['AC 自动机', ['aho', 'ac automaton', 'acautomaton']],
    ['回文自动机 PAM', ['eertree', 'palindromic tree', 'pam']],
    ['Dijkstra / SPFA / Bellman', ['dijkstra', 'spfa', 'bellman']],
    ['Floyd / 传递闭包', ['floyd', 'warshall']],
    ['最小生成树 / 最小树形图', ['kruskal', 'prim', 'mst', 'arborescence', 'edmonds']],
    ['网络流 Dinic / ISAP', ['dinic', 'isap', 'maxflow', 'max flow', 'push-relabel']],
    ['费用流 MCMF', ['mincost', 'min cost', 'mcmf', 'cost flow']],
    ['上下界网络流', ['lower bound', '上下界']],
    ['二分图匹配（匈牙利/KM）', ['hungarian', 'kuhn', 'bipartite matching', 'assignment']],
    ['一般图匹配（带花树）', ['blossom', 'general matching']],
    ['2-SAT', ['2-sat', '2sat']],
    ['Tarjan 强连通 / 割点', ['scc', 'tarjan', 'articulation', 'bridge', 'strongly']],
    ['欧拉路 / 哈密顿', ['euler', 'hamilton']],
    ['LCA / 倍增', ['lca', 'binary lifting']],
    ['树的重心 / 直径', ['centroid', 'diameter']],
    ['线性筛 / 积性函数', ['sieve', 'linear sieve', 'totient', 'mobius']],
    ['数论分块 / 杜教筛', ['divisor block', 'du jiao', 'dirichlet']],
    ['exgcd / 逆元 / CRT', ['euclid', 'inverse', 'crt', 'chinese remainder']],
    ['BSGS / 离散对数', ['bsgs', 'discrete log', 'baby-step']],
    ['Miller-Rabin / Pollard-Rho', ['miller', 'pollard', 'factorization', 'prime test']],
    ['FFT / NTT / 多项式', ['fft', 'ntt', 'polynomial', 'convolution']],
    ['高斯消元 / 行列式 / 线性基', ['gauss', 'determinant', 'linear basis', 'basis']],
    ['博弈论 SG / Nim', ['nim', 'grundy', 'sg ', 'game theory']],
    ['计算几何：凸包', ['convex hull', 'hull', 'graham', 'andrew']],
    ['计算几何：半平面交', ['half-plane', 'halfplane', 'half plane']],
    ['计算几何：旋转卡壳 / 最小圆覆盖', ['rotating calipers', 'calipers', 'minimum enclosing', 'smallest circle']],
    ['矩阵快速幂 / 线性递推', ['matrix', 'linear recurrence', 'kitamasa', 'berlekamp']],
    ['插头 DP / 轮廓线', ['plug dp', 'broken profile', 'profile dp']],
    ['动态 DP / 斜率优化', ['dynamic dp', 'ddp', 'convex hull trick', 'li chao', 'slope']],
    ['莫比乌斯反演', ['mobius']],
    ['Simpson / 数值积分', ['simpson', 'integration']],
    ['模拟退火 / 爬山', ['annealing', 'hill climb']],
    ['分数规划 / 三分', ['fractional programming', 'ternary']],
];

const repos = {};
for (const [label, repo, br] of REPOS) {
    const t = await tree(repo, br);
    repos[label] = t;
    console.log(`${label}: ${t.err ? '抓取失败 ' + t.err : t.files.length + ' 个文件'}`);
}
const labels = Object.keys(repos);

const rows = [];
for (const [topic, keys] of TOPICS) {
    const all = keys.concat(ZH[topic] || []);
    const ours = all.some(k => ourAll.includes(k)) ? '✔' : '—';
    const cells = labels.map(l => {
        const fs_ = repos[l].files;
        return keys.some(k => fs_.some(p => p.includes(k))) ? '✔' : '—';
    });
    rows.push([topic, ours, ...cells]);
}

console.log('\n主题 | 我们 | ' + labels.map(l => l.split('（')[0]).join(' | '));
console.log('---|---|---|---|---|---');
for (const r of rows) console.log(r.join(' | '));

const miss = rows.filter(r => r[1] === '—' && r.slice(2).some(x => x === '✔'));
console.log('\n=== 他们有、我们还没有的 ===');
console.log(miss.length ? miss.map(r => '  ' + r[0]).join('\n') : '  无');

const only = rows.filter(r => r[1] === '✔' && r.slice(2).every(x => x === '—'));
console.log('\n=== 我们有、这几家里都没匹配上的（多为中文命名的自研实现）===');
console.log('  ' + only.map(r => r[0]).join('  '));
