// 自动生成模板库索引：文件名 + 首行注释说明 + 对应题目（从台账反查）
// 用法: node _台账\生成索引.mjs   ->  生成 索引.md
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const ROOT = path.join(DIR, '..');
const DIRS = ['01-基础与技巧', '02-数据结构', '03-字符串', '04-图论', '05-数学', '06-动态规划', '07-搜索', '08-计算几何', '09-其他'];
const TITLE = {
    '01-基础与技巧': '基础与技巧', '02-数据结构': '数据结构', '03-字符串': '字符串', '04-图论': '图论',
    '05-数学': '数学', '06-动态规划': '动态规划', '07-搜索': '搜索', '08-计算几何': '计算几何', '09-其他': '其他',
};
const filesIn = d => fs.readdirSync(path.join(ROOT, d)).filter(f => /\.(cpp|md)$/.test(f));

// 反查：模板文件 -> 题目（来自 题目-模板对照.csv）
const fileToProbs = new Map();
const csvPath = path.join(DIR, '题目-模板对照.csv');
if (fs.existsSync(csvPath)) {
    const lines = fs.readFileSync(csvPath, 'utf8').replace(/^\ufeff/, '').trim().split(/\r?\n/).slice(1);
    for (const l of lines) {
        const m = l.match(/^([^,]+),"((?:[^"]|"")*)",([^,]*),([^,]*),(.*)$/);
        if (!m) continue;
        const id = m[1], name = m[2].replace(/""/g, '"'), diff = m[3];
        for (const f of m[5].split(/\s+/).filter(Boolean)) {
            if (!fileToProbs.has(f)) fileToProbs.set(f, []);
            const arr = fileToProbs.get(f);
            if (arr.length < 4) arr.push(`${id}(${diff})`);
        }
    }
}

// 读文件首行中文注释当说明
function describe(file) {
    const txt = fs.readFileSync(file, 'utf8');
    if (file.endsWith('.md')) return '去重的操作表格：常用写法、复杂度、比较器和迭代器失效规则。';
    for (const line of txt.split(/\r?\n/).slice(0, 40)) {
        const m = /^\s*\/\/\s*(.+)$/.exec(line);
        if (m && /[\u4e00-\u9fa5]/.test(m[1]) && !/^={2,}|^----|^自测|^\[/.test(m[1])) {
            let d = m[1].trim();
            if (d.length > 62) d = d.slice(0, 62) + '…';
            return d;
        }
    }
    return '';
}

const md = ['# 模板索引（自动生成）', '',
    `共 **${DIRS.reduce((s, d) => s + filesIn(d).filter(f => f.endsWith('.cpp')).length, 0)}** 份代码模板，另有 **${DIRS.reduce((s, d) => s + filesIn(d).filter(f => f.endsWith('.md')).length, 0)}** 份 STL 速查表。`,
    '「台账里带同类标签的题」是从 `_台账/题目-模板对照.csv` 反查出来的（最多列 4 道，只说明"你在哪些题上用到过这类算法"，不是这道题只能用这个模板）。', ''];

let total = 0;
for (const d of DIRS) {
    const files = filesIn(d).sort((a, b) => a.localeCompare(b, 'zh'));
    if (!files.length) continue;
    const refs = files.filter(f => f.endsWith('.md')).length;
    md.push(`## ${d} · ${TITLE[d]}（${refs ? `${files.length - refs} 份代码、${refs} 份速查表` : `${files.length} 个`}）`, '');
    md.push('| 模板 | 说明 | 台账里带同类标签的题（示例） |', '|---|---|---|');
    for (const f of files) {
        const rel = `${d}/${f}`;
        const desc = describe(path.join(ROOT, d, f)).replace(/\|/g, '\\|');
        const probs = (fileToProbs.get(rel) || []).join('、');
        md.push(`| [${f.replace(/\.(cpp|md)$/, '')}](<${rel}>) | ${desc} | ${probs} |`);
        total++;
    }
    md.push('');
}
fs.writeFileSync(path.join(ROOT, '索引.md'), md.join('\n'), 'utf8');

// 顺便统计：哪些文件没有对应题目（多为补充的进阶模板）
const noProb = [];
for (const d of DIRS) for (const f of fs.readdirSync(path.join(ROOT, d)).filter(x => x.endsWith('.cpp'))) {
    if (!fileToProbs.has(`${d}/${f}`)) noProb.push(`${d}/${f}`);
}
console.log(`索引.md 已生成：${total} 个代码或速查条目`);
console.log(`其中 ${noProb.length} 个模板在你台账里暂无对应题目（多为按 ICPC 常规补的）：`);
console.log('  ' + noProb.map(s => s.replace(/\.cpp$/, '')).join('  '));
