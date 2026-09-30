// 补齐：AtCoder 标签（扫洛谷 AT 列表页） + CF 缺失题的标签/rating（contest.standings）
// 用法: node _台账\补齐标签.mjs
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
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
const dec = s => s.replace(/&amp;/g, '&').replace(/&#039;/g, "'").replace(/&quot;/g, '"').replace(/&lt;/g, '<').replace(/&gt;/g, '>').replace(/\s+/g, ' ').trim();

// ---------- 1) AtCoder：扫 AT 列表页拿标签 ----------
const atCsv = fs.readFileSync(path.join(DIR, 'at台账.csv'), 'utf8').replace(/^\ufeff/, '').trim().split(/\r?\n/);
const atHead = atCsv.shift().split(',');
const atRows = atCsv.map(l => {
    const m = l.match(/^([^,]+),"([^"]*)",([^,]*),"((?:[^"]|"")*)",(\d+),([^,]*),(.*)$/);
    return m ? { id: m[1], folder: m[2], letter: m[3], name: m[4], dv: m[5], dn: m[6], tags: m[7] } : null;
}).filter(Boolean);
const targets = new Set(atRows.map(r => r.id));
console.log(`AtCoder 目标 ${targets.size} 题，开始扫洛谷 AT 列表（共 163 页）`);

const tagOf = new Map();
let idx = 0, done = 0;
async function worker() {
    while (idx < 163) {
        const page = ++idx;
        const html = await get(`https://www.luogu.com.cn/problem/list?type=AT&page=${page}`);
        if (html) {
            for (const row of html.split('<li>').slice(1)) {
                const pm = /\/problem\/(AT_[A-Za-z0-9_]+)"/.exec(row);
                if (!pm || !targets.has(pm[1])) continue;
                const tags = [...row.matchAll(/problem\/list\?tag=\d+"[^>]*>([^<]+)</g)].map(m => dec(m[1])).filter(Boolean);
                if (tags.length && !tagOf.has(pm[1])) tagOf.set(pm[1], [...new Set(tags)].join(';'));
            }
        }
        done++;
        if (done % 40 === 0) console.log(`  AT 列表进度 ${done}/163，已补标签 ${tagOf.size}`);
    }
}
await Promise.all([worker(), worker(), worker(), worker()]);

let filled = 0;
for (const r of atRows) {
    if (tagOf.has(r.id)) { r.tags = tagOf.get(r.id); filled++; }
}
fs.writeFileSync(path.join(DIR, 'at台账.csv'), '\ufeff' + [atHead.join(','),
    ...atRows.map(r => [r.id, '"' + r.folder + '"', r.letter, '"' + r.name.replace(/"/g, '""') + '"', r.dv, r.dn, r.tags].join(','))].join('\n') + '\n', 'utf8');
console.log(`AtCoder：补上标签 ${filled}/${atRows.length} 题`);

// ---------- 2) Codeforces：standings 补缺失题 ----------
const cfCsv = fs.readFileSync(path.join(DIR, 'cf台账.csv'), 'utf8').replace(/^\ufeff/, '').trim().split(/\r?\n/);
const cfHead = cfCsv.shift().split(',');
const cfRows = cfCsv.map(l => {
    const m = l.match(/^([^,]+),"([^"]*)",([^,]*),"((?:[^"]|"")*)",([^,]*),(.*)$/);
    return m ? { id: m[1], contest: m[2], index: m[3], name: m[4], rating: m[5], tags: m[6] } : null;
}).filter(Boolean);
const missing = cfRows.filter(r => !r.tags && !r.rating);
const needContests = [...new Set(missing.map(r => Number(/^CF(\d+)/.exec(r.id)[1])))];
console.log(`\nCF 缺失 ${missing.length} 题，涉及比赛 ${needContests.join(' ')}`);
const fix = new Map();
for (const cid of needContests) {
    const r = await fetch(`https://codeforces.com/api/contest.standings?contestId=${cid}&from=1&count=1`, { headers: { 'User-Agent': 'Mozilla/5.0' } });
    const j = await r.json();
    if (j.status !== 'OK') { console.log(`  contest ${cid} standings 失败: ${j.comment || j.status}`); continue; }
    for (const p of j.result.problems) fix.set(cid + '|' + p.index.toUpperCase(), { name: p.name, rating: p.rating ?? '', tags: (p.tags || []).join(';') });
}
let cfix = 0;
for (const r of cfRows) {
    if (r.tags || r.rating) continue;
    const m = /^CF(\d+)(.+)$/.exec(r.id);
    const p = fix.get(Number(m[1]) + '|' + m[2].toUpperCase());
    if (p) { r.name = p.name; r.rating = p.rating; r.tags = p.tags; cfix++; }
}
fs.writeFileSync(path.join(DIR, 'cf台账.csv'), '\ufeff' + [cfHead.join(','),
    ...cfRows.map(r => [r.id, '"' + r.contest + '"', r.index, '"' + String(r.name).replace(/"/g, '""') + '"', r.rating, r.tags].join(','))].join('\n') + '\n', 'utf8');
console.log(`Codeforces：补上 ${cfix} 题`);

// ---------- 3) 汇总 ----------
const atTag = new Map(), cfTag = new Map();
for (const r of atRows) for (const t of r.tags.split(';').filter(Boolean)) atTag.set(t, (atTag.get(t) || 0) + 1);
for (const r of cfRows) for (const t of r.tags.split(';').filter(Boolean)) cfTag.set(t, (cfTag.get(t) || 0) + 1);
console.log('\n=== AtCoder 标签统计（' + atTag.size + ' 种）===');
console.log([...atTag].sort((a, b) => b[1] - a[1]).map(([t, n]) => `${n} ${t}`).join('  '));
console.log('\n=== 两库合计新增标签（合并 CF+AT）===');
const both = new Map();
for (const [k, v] of [...atTag, ...cfTag]) both.set(k, (both.get(k) || 0) + v);
console.log([...both].sort((a, b) => b[1] - a[1]).map(([t, n]) => `${n} ${t}`).join('  '));
