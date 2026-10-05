import fs from 'node:fs';
import path from 'node:path';
const root=path.resolve(import.meta.dirname,'..');
const dirs=fs.readdirSync(root).filter(x=>/^0[1-9]-/.test(x));
function lexical(s){return s.replace(/\/\*[\s\S]*?\*\//g,'').replace(/"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'/g,'').replace(/\/\/.*$/gm,'');}
const records=[];
for(const dir of dirs)for(const file of fs.readdirSync(path.join(root,dir)).filter(x=>x.endsWith('.cpp'))){
 const rel=dir+'/'+file,source=fs.readFileSync(path.join(root,rel),'utf8');
 let depth=0,globals=[];
 for(const line of source.split(/\r?\n/)){
  const clean=lexical(line).trim();
  if(depth===0&&clean.endsWith(';')&&!/^(#|using |typedef |const |static const |template|struct |class |enum )/.test(clean)&&!clean.includes('return ')&&!clean.includes('('))globals.push(line.trim());
  depth+=(clean.match(/\{/g)||[]).length-(clean.match(/\}/g)||[]).length;
 }
 records.push({rel,globals});
}
if(process.argv.includes('--audit')){
 const notes=fs.readFileSync(path.join(root,'说明/全局变量说明.md'),'utf8');
 const keys=[...notes.matchAll(/^## (.+\.cpp)\r?$/gm)].map(x=>x[1]);
 const counts={},missing=[],extra=keys.filter(x=>!records.some(r=>r.rel===x));
 const duplicates=keys.filter((x,i)=>keys.indexOf(x)!==i);
 for(const r of records){
  const dir=r.rel.split('/')[0],name=path.basename(r.rel);
  const api=fs.readFileSync(path.join(root,'_台账/api',dir+'.md'),'utf8');
  counts[dir]=(counts[dir]||0)+1;
  if(!keys.includes(r.rel))missing.push('变量说明：'+r.rel);
  if(!api.split(/\r?\n/).includes('## '+name))missing.push('API：'+r.rel);
 }
 process.stdout.write(JSON.stringify({templates:records.length,counts,missing,extra,duplicates}));
 if(missing.length||extra.length||duplicates.length)process.exitCode=1;
}else process.stdout.write(JSON.stringify(records));
