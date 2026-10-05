// 自动生成公开模板索引：文件名、用途与独立说明，不读取个人台账。
// 用法: node _台账\生成索引.mjs   ->  生成 索引.md
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const ROOT = path.join(DIR, '..');
const DIRS = ['01-基础与技巧', '02-数据结构', '03-字符串', '04-图论', '05-数学', '06-动态规划', '07-搜索', '08-计算几何', '09-其他'];
const TITLE = {
    '01-基础与技巧': '基础与技巧', '02-数据结构': '数据结构', '03-字符串': '字符串', '04-图论': '图论',
    '05-数学': '数学', '06-动态规划': '动态规划', '07-搜索': '搜索', '08-计算几何': '计算几何', '09-其他': '实现与调试',
};
const filesIn = d => fs.readdirSync(path.join(ROOT, d)).filter(f => /\.(cpp|md)$/.test(f));

// 读文件首行中文注释当说明
function describe(file) {
    const txt = fs.readFileSync(file, 'utf8');
    if (file.endsWith('.md')) return '去重的操作表格：常用写法、复杂度、比较器和迭代器失效规则。';
    const companion = path.join(ROOT, '说明', path.relative(ROOT, file).replace(/\.cpp$/, '.md'));
    if (fs.existsSync(companion)) {
        return fs.readFileSync(companion, 'utf8').split(/\r?\n/)
            .find(line => line.trim() && !/^(#|\[|<!--)/.test(line.trim())) || '';
    }
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
    '按用途查找实现；调用条件与复杂度见[接口说明](_台账/api/README.md)。', ''];

let total = 0;
for (const d of DIRS) {
    const files = filesIn(d).sort((a, b) => a.localeCompare(b, 'zh'));
    if (!files.length) continue;
    const refs = files.filter(f => f.endsWith('.md')).length;
    md.push(`## ${d} · ${TITLE[d]}（${refs ? `${files.length - refs} 份代码、${refs} 份速查表` : `${files.length} 个`}）`, '');
    md.push('| 模板 | 说明 |', '|---|---|');
    for (const f of files) {
        const rel = `${d}/${f}`;
        let desc = describe(path.join(ROOT, d, f)).replace(/\|/g, '\\|');
        const companion = `说明/${rel.replace(/\.cpp$/, '.md')}`;
        if (fs.existsSync(path.join(ROOT, companion))) desc += ` [说明](<${companion}>)`;
        md.push(`| [${f.replace(/\.(cpp|md)$/, '')}](<${rel}>) | ${desc} |`);
        total++;
    }
    md.push('');
}
fs.writeFileSync(path.join(ROOT, '索引.md'), md.join('\n'), 'utf8');

console.log(`索引.md 已生成：${total} 个代码或速查条目`);
