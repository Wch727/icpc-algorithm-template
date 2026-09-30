// 从 D:\code\Atcoder 的目录结构还原 AtCoder 题号，用洛谷 AT_ 镜像题页取 名称/难度/标签
// 用法: node _台账\抓AT台账.mjs
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const ROOT = 'D:\\code\\Atcoder';

// 目录名 -> AtCoder 比赛代号
function contestCode(folder) {
    const abc = /AtCoder Beginner Contest (\d+)/.exec(folder);
    if (abc) return 'abc' + abc[1];
    const arc = /AtCoder Regular Contest[- ]*(\d+)/.exec(folder);
    if (arc) return 'arc' + arc[1];
    if (/dp contest/i.test(folder)) return 'dp';
    return null;
}

const jobs = [];   // {luoguId, folder, letter}
for (const d of fs.readdirSync(ROOT, { withFileTypes: true })) {
    if (!d.isDirectory() || d.name === 'bin') continue;
    const code = contestCode(d.name);
    const files = fs.readdirSync(path.join(ROOT, d.name)).filter(f => /\.(cpp|CPP)$/i.test(f));
    for (const f of files) {
        const letter = f.replace(/\.(cpp|CPP)$/i, '');
        if (!/^[A-Za-z]\d?$/.test(letter)) continue;
        if (code) jobs.push({ luoguId: `AT_${code}_${letter.toLowerCase()}`, folder: d.name, letter: letter.toUpperCase() });
        else jobs.push({ luoguId: null, folder: d.name, letter, unknown: true });
    }
}
// 根目录散文件：243C.cpp -> 试 AT_abc243_c / AT_arc243_c
for (const f of fs.readdirSync(ROOT).filter(f => /\.(cpp|CPP)$/i.test(f))) {
    const m = /^(\d+)([A-Z])\d*$/i.exec(f.replace(/\.(cpp|CPP)$/i, ''));
    if (m) { jobs.push({ luoguId: `AT_abc${m[1]}_${m[2].toLowerCase()}`, folder: '根目录', letter: m[2].toUpperCase(), fallback: `AT_arc${m[1]}_${m[2].toLowerCase()}` }); }
}
console.log(`识别 ${jobs.filter(j => j.luoguId).length} 个可查题号` + (jobs.some(j => j.unknown) ? `，另有 ${jobs.filter(j => j.unknown).length} 个目录无法识别比赛` : ''));

let jar = null;
async function get(url, tries = 6) {
    for (let i = 0; i < tries; i++) {
        try {
            const r = await fetch(url, jar ? { headers: { Cookie: 'C3VK=' + jar } } : {});
            const t = await r.text();
            const m = t.match(/C3VK=([0-9a-f]+)/);
            if (m) { jar = m[1]; if (t.length < 1200) { await new Promise(x => setTimeout(x, 200)); continue; } }
            return t;
        } catch (e) { await new Promise(x => setTimeout(x, 400 * (i + 1))); }
    }
    return null;
}
const dec = s => s.replace(/&amp;/g, '&').replace(/&lt;/g, '<').replace(/&gt;/g, '>').replace(/&quot;/g, '"').replace(/&#39;/g, "'").replace(/\s+/g, ' ').trim();

const DIFF = { 1: '入门', 2: '普及-', 3: '普及', 4: '普及+/提高-', 5: '提高', 6: '提高+/省选-', 7: '省选/NOI-', 8: 'NOI/NOI+/CTSC' };
const rows = [], bad = [];

async function probe(id) {
    const html = await get('https://www.luogu.com.cn/problem/' + id);
    if (!html || html.length < 2000) return null;
    if (/题目不存在|没有找到|404/.test(html) && !/"difficulty"/.test(html)) return null;
    const dm = /"difficulty":(\d+)/.exec(html);
    const nm = /"name":"((?:[^"\\]|\\.)*)"/.exec(html);
    if (!dm && !nm) return null;
    const tags = [];
    for (const m of html.matchAll(/\/problem\/list\?tag=\d+"[^>]*>([^<]+)</g)) {
        const t = dec(m[1]);
        if (t && !tags.includes(t)) tags.push(t);
    }
    return { name: nm ? JSON.parse('"' + nm[1] + '"') : '', difficulty: dm ? Number(dm[1]) : 0, tags: tags.join(';') };
}

for (const j of jobs) {
    if (!j.luoguId) continue;
    let r = await probe(j.luoguId);
    let usedId = j.luoguId;
    if (!r && j.fallback) { r = await probe(j.fallback); usedId = j.fallback; }
    if (!r) { bad.push(j.luoguId); continue; }
    rows.push({ id: usedId, folder: j.folder, letter: j.letter, ...r });
}
rows.sort((a, b) => a.id.localeCompare(b.id));
const out = ['id,比赛,题号,题目名,难度值,难度,标签',
    ...rows.map(r => [r.id, '"' + r.folder + '"', r.letter, '"' + r.name.replace(/"/g, '""') + '"', r.difficulty, DIFF[r.difficulty] || '未评定', r.tags].join(','))];
fs.writeFileSync(path.join(DIR, 'at台账.csv'), '\ufeff' + out.join('\n') + '\n', 'utf8');

console.log(`\n成功 ${rows.length} 题写入 _台账/at台账.csv`);
if (bad.length) console.log('洛谷查不到的: ' + bad.join(' '));
const tagCnt = new Map();
for (const r of rows) for (const t of r.tags.split(';').filter(Boolean)) tagCnt.set(t, (tagCnt.get(t) || 0) + 1);
console.log('\n=== AtCoder 标签统计 ===');
console.log([...tagCnt].sort((a, b) => b[1] - a[1]).map(([t, n]) => `${n} ${t}`).join('\n'));
const d = new Map();
for (const r of rows) d.set(DIFF[r.difficulty] || '未评定', (d.get(DIFF[r.difficulty] || '未评定') || 0) + 1);
console.log('\n难度分布: ' + [...d].map(([k, v]) => k + ':' + v).join('  '));
