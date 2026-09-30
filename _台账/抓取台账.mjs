// 洛谷题目台账抓取：扫描各难度全部列表页，抽出目标题号的 名称/难度/算法标签
// 用法: node _台账\抓取台账.mjs
import fs from 'node:fs';
import path from 'node:path';

const DIR = import.meta.dirname;
const read = f => fs.readFileSync(path.join(DIR, f), 'utf8').split(/\s+/).filter(Boolean);
const pIds = read('IDs_P.txt');
const oIds = read('IDs_other.txt');
const targets = new Set([...pIds, ...oIds]);

const PAGES = { 1: 12, 2: 34, 3: 45, 4: 52, 5: 58, 6: 13, 7: 72, 8: 39 };
const DIFF = { 1: '入门', 2: '普及-', 3: '普及', 4: '普及+/提高-', 5: '提高', 6: '提高+/省选-', 7: '省选/NOI-', 8: 'NOI/NOI+/CTSC' };
const sleep = ms => new Promise(r => setTimeout(r, ms));
const dec = s => s.replace(/&amp;/g, '&').replace(/&lt;/g, '<').replace(/&gt;/g, '>')
    .replace(/&quot;/g, '"').replace(/&#39;/g, "'").replace(/&nbsp;/g, ' ').replace(/\s+/g, ' ').trim();

let jar = null;
async function get(url, tries = 6) {
    for (let i = 0; i < tries; i++) {
        try {
            const r = await fetch(url, jar ? { headers: { Cookie: 'C3VK=' + jar } } : {});
            const t = await r.text();
            const m = t.match(/C3VK=([0-9a-f]+)/);
            if (m) { jar = m[1]; if (t.length < 1200) { await sleep(200); continue; } }
            return t;
        } catch (e) { await sleep(400 * (i + 1)); }
    }
    return null;
}

const tagMap = new Map();
const found = new Map();

const jobs = [];
for (let d = 1; d <= 8; d++) for (let p = 1; p <= PAGES[d]; p++) jobs.push([d, p]);

let idx = 0, done = 0, bad = 0;
async function worker() {
    while (idx < jobs.length) {
        const [d, p] = jobs[idx++];
        const html = await get(`https://www.luogu.com.cn/problem/list?difficulty=${d}&page=${p}&type=P`);
        if (!html) { bad++; }
        else {
            for (const row of html.split('<li>').slice(1)) {
                const pm = /\/problem\/([A-Za-z]+\d+[A-Za-z]*)"/.exec(row);
                if (!pm) continue;
                const pid = pm[1].toUpperCase();
                const nm = /<h3><a[^>]*>([^<]*)<\/a>/.exec(row);
                const tags = [];
                for (const m of row.matchAll(/\/problem\/list\?tag=(\d+)"[^>]*>([^<]+)</g)) {
                    const name = dec(m[2]);
                    if (!name) continue;
                    if (!tags.includes(name)) tags.push(name);
                    if (!tagMap.has(m[1])) tagMap.set(m[1], name);
                }
                if (targets.has(pid) && !found.has(pid)) {
                    found.set(pid, { id: pid, name: nm ? dec(nm[1]) : '', difficulty: d, tags: tags.join(';') });
                }
            }
        }
        done++;
        if (done % 40 === 0) console.log(`页进度 ${done}/${jobs.length}  命中 ${found.size}/${targets.size}  失败页 ${bad}`);
        await sleep(90);
    }
}
await Promise.all([worker(), worker(), worker(), worker()]);

// 兜底：列表里没找到的题号（含难评为 0 的题、非 P 题库），逐题抓题目页
const missing = [...targets].filter(id => !found.has(id));
console.log(`列表扫描完成，命中 ${found.size}，兜底逐个抓 ${missing.length} 个`);
for (const id of missing) {
    const html = await get('https://www.luogu.com.cn/problem/' + id);
    if (!html) { console.log('  抓取失败 ' + id); continue; }
    const dm = /"pid":"[^"]+","type":"[^"]*","name":"((?:[^"\\]|\\.)*)"[\s\S]{0,200}?"difficulty":(\d+)/.exec(html);
    const dm2 = /"difficulty":(\d+)/.exec(html);
    const nm = /"name":"((?:[^"\\]|\\.)*)"/.exec(html);
    const tm = /"tags":\[([0-9,]*)\]/.exec(html);
    const tags = [];
    if (tm && tm[1]) {
        for (const tid of tm[1].split(',').filter(Boolean)) {
            if (!tagMap.has(tid)) {
                const page = await get('https://www.luogu.com.cn/problem/list?tag=' + tid + '&type=P');
                const t2 = page && /<title>([^<]*)<\/title>/.exec(page);
                const cand = page && [...page.matchAll(/\/problem\/list\?tag=\d+"[^>]*>([^<]+)</g)].map(x => dec(x[1]));
                tagMap.set(tid, 'tag#' + tid);
                void t2; void cand;
            }
            tags.push(tagMap.get(tid));
        }
    }
    found.set(id, {
        id,
        name: nm ? JSON.parse('"' + nm[1] + '"') : '',
        difficulty: dm2 ? Number(dm2[1]) : 0,
        tags: tags.join(';'),
    });
    await sleep(120);
}

const rows = [...found.values()].sort((a, b) => {
    const na = /^P(\d+)$/.exec(a.id), nb = /^P(\d+)$/.exec(b.id);
    if (na && nb) return Number(na[1]) - Number(nb[1]);
    if (na) return -1;
    if (nb) return 1;
    return a.id.localeCompare(b.id);
});
const csv = ['id,名称,难度值,难度,标签',
    ...rows.map(r => [r.id, '"' + r.name.replace(/"/g, '""') + '"', r.difficulty, DIFF[r.difficulty] || '未评定', r.tags].join(','))];
fs.writeFileSync(path.join(DIR, '题目台账.csv'), '\ufeff' + csv.join('\n') + '\n', 'utf8');
fs.writeFileSync(path.join(DIR, 'tag_map.csv'), '\ufeff' + [...tagMap].map(([k, v]) => k + ',' + v).join('\n') + '\n', 'utf8');

console.log(`\n完成：${rows.length}/${targets.size} 条已写入 题目台账.csv`);
const still = [...targets].filter(id => !found.has(id));
if (still.length) console.log('仍未拿到: ' + still.join(' '));
console.log('标签字典 tag_map.csv 共 ' + tagMap.size + ' 条');
