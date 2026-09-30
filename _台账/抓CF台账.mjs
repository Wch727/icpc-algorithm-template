// 从 D:\code\CodeForces 的目录结构还原题号，用 Codeforces 官方 API 取 标签 + rating
// 用法: node _台账\抓CF台账.mjs
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const ROOT = 'D:\\code\\CodeForces';

// 1) 目录 -> 字母
const contests = [];
for (const d of fs.readdirSync(ROOT, { withFileTypes: true })) {
    if (!d.isDirectory() || d.name === 'bin') continue;
    const letters = fs.readdirSync(path.join(ROOT, d.name))
        .filter(f => /^[A-Za-z]\d?\.(cpp|CPP)$/i.test(f))
        .map(f => f.replace(/\.(cpp|CPP)$/i, '').toUpperCase())
        .sort();
    if (letters.length) contests.push({ folder: d.name, letters });
}
// 根目录散文件：1091Adiv2.cpp -> 1091A ; 580C.cpp -> 580C
const loose = fs.readdirSync(ROOT).filter(f => /\.(cpp|CPP)$/i.test(f)).map(f => f.replace(/\.(cpp|CPP)$/i, ''));
for (const name of loose) {
    const m = /^(\d{3,4})([A-Z]\d?)/i.exec(name);
    if (m) contests.push({ folder: '根目录散文件', letters: [], loose: { contest: Number(m[1]), index: m[2].toUpperCase() } });
}

console.log('共识别 ' + contests.filter(c => c.letters.length).length + ' 场比赛');

// 2) 官方 API
const getJSON = async (url) => {
    const r = await fetch(url, { headers: { 'User-Agent': 'Mozilla/5.0' } });
    return await r.json();
};
const cl = await getJSON('https://codeforces.com/api/contest.list?gym=false');
const ps = await getJSON('https://codeforces.com/api/problemset.problems');
if (cl.status !== 'OK' || ps.status !== 'OK') { console.log('API 返回异常', cl.status, ps.status); process.exit(1); }
console.log(`contest.list: ${cl.result.length} 场    problemset: ${ps.result.problems.length} 题`);

const byId = new Map();
for (const c of cl.result) byId.set(c.id, c.name);
const probKey = (cid, idx) => cid + '|' + idx.toUpperCase();
const probMap = new Map();
for (const p of ps.result.problems) {
    probMap.set(probKey(p.contestId, p.index), {
        name: p.name, rating: p.rating ?? '', tags: (p.tags || []).join(';'),
    });
}

// 3) 目录名 -> contestId（用名字里的数字与官方名字匹配）
function matchContest(folder) {
    const nums = [...folder.matchAll(/(\d{3,4})/g)].map(m => Number(m[1]));
    const edu = /Educational/i.test(folder);
    for (const n of nums) {
        for (const c of cl.result) {
            if (!byId.get(c.id).includes(String(n))) continue;
            if (edu !== /Educational/i.test(c.name)) continue;
            return c;
        }
    }
    return null;
}

const rows = [];
const unmatched = [];
for (const ct of contests) {
    if (ct.loose) {
        const p = probMap.get(probKey(ct.loose.contest, ct.loose.index));
        rows.push({ id: 'CF' + ct.loose.contest + ct.loose.index, contest: ct.folder, ...(p || { name: '?', rating: '', tags: '' }) });
        continue;
    }
    const c = matchContest(ct.folder);
    if (!c) { unmatched.push(ct.folder); continue; }
    for (const L of ct.letters) {
        const p = probMap.get(probKey(c.id, L));
        rows.push({
            id: 'CF' + c.id + L, contest: c.name, index: L,
            name: p ? p.name : '?', rating: p ? p.rating : '', tags: p ? p.tags : '',
        });
    }
}
rows.sort((a, b) => {
    const na = Number(/CF(\d+)/.exec(a.id)[1]), nb = Number(/CF(\d+)/.exec(b.id)[1]);
    return na - nb || a.id.localeCompare(b.id);
});
const out = ['id,比赛,题号,题目名,rating,标签',
    ...rows.map(r => [r.id, '"' + r.contest + '"', r.index || '', '"' + String(r.name).replace(/"/g, '""') + '"', r.rating, r.tags].join(','))];
fs.writeFileSync(path.join(DIR, 'cf台账.csv'), '\ufeff' + out.join('\n') + '\n', 'utf8');

console.log(`\n共 ${rows.length} 题写入 _台账/cf台账.csv`);
const noData = rows.filter(r => !r.tags && !r.rating);
console.log(`其中 API 里查不到的: ${noData.length} 题` + (noData.length ? ' -> ' + noData.map(r => r.id).join(' ') : ''));
if (unmatched.length) console.log('目录名没匹配上比赛的: ' + unmatched.join(' | '));
const tagCnt = new Map();
for (const r of rows) for (const t of String(r.tags).split(';').filter(Boolean)) tagCnt.set(t, (tagCnt.get(t) || 0) + 1);
console.log('\n=== CF 标签统计（按题数）===');
console.log([...tagCnt].sort((a, b) => b[1] - a[1]).map(([t, n]) => `${n} ${t}`).join('\n'));
const ratings = rows.filter(r => r.rating).map(r => r.rating).sort((a, b) => a - b);
console.log('\nrating 范围: ' + (ratings.length ? ratings[0] + ' ~ ' + ratings[ratings.length - 1] + '（中位 ' + ratings[ratings.length >> 1] + '）' : '无'));
