#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
并行验证器（比 自检.ps1 快很多）：
  1. 每个纯模板用 g++ -fsyntax-only 检查（不生成目标文件，快）
  2. 每个测试文件编译 + 运行（带超时），并检查输出里的 FAIL/FAILED
用法:
  python _台账/verify.py            # 全部
  python _台账/verify.py 04-图论    # 只查某个目录
"""
import os, re, sys, subprocess, concurrent.futures as cf, uuid

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GXX = r'C:\mingw64\bin\g++.exe'
BIN = os.path.join(ROOT, 'bin')
DIRS = ['01-基础与技巧', '02-数据结构', '03-字符串', '04-图论', '05-数学',
        '06-动态规划', '07-搜索', '08-计算几何', '09-其他']
ONLY = sys.argv[1] if len(sys.argv) > 1 else ''
JOBS = 4
RUN_ID = uuid.uuid4().hex[:8]

os.makedirs(BIN, exist_ok=True)

def tpl_check(args):
    d, f = args
    p = os.path.join(ROOT, d, f)
    r = subprocess.run([GXX, '-fsyntax-only', p, '-std=c++2b', '-O1', '-Wall'],
                       capture_output=True, text=True, encoding='utf-8', errors='replace')
    ok = r.returncode == 0
    src = open(p, encoding='utf-8', errors='replace').read()
    style = []
    if '#include<bits/stdc++.h>' not in src.replace(' ', ''): style.append('无bits头')
    if 'using namespace std;' not in src: style.append('无using')
    if re.search(r'(?m)^\t', src): style.append('有Tab')
    if 'std::' in src: style.append('有std::')
    return ('模板', f'{d}/{f}', 'OK' if ok else 'FAIL', '-', ','.join(style) or 'OK',
            '' if ok else (r.stderr.strip().split('\n')[0][:90] if r.stderr.strip() else ''))

def test_check(args):
    i, d, f = args
    p = os.path.join(ROOT, '测试', d, f)
    exe = os.path.join(BIN, f'_v{RUN_ID}_{i}.exe')
    r = subprocess.run([GXX, p, '-std=c++2b', '-O1', '-Wall', '-Wl,--stack,67108864', '-o', exe],
                       capture_output=True, text=True, encoding='utf-8', errors='replace')
    if not os.path.exists(exe):
        err = [l for l in r.stderr.split('\n') if 'error' in l]
        return ('测试', f'{d}/{f}', 'FAIL', '-', '-', (err[0][:90] if err else '编译失败'))
    run = 'OK'; note = ''
    try:
        pr = subprocess.run([exe], capture_output=True, text=True, timeout=20,
                            encoding='utf-8', errors='replace', stdin=subprocess.DEVNULL)
        allout = (pr.stdout or '') + (pr.stderr or '')
        if pr.returncode != 0: run = f'RE({pr.returncode})'
        elif re.search(r'FAILED|\bFAIL\b|\bWA\b|失败\s*[1-9]|不通过|答案错误', allout): run = 'FAILED-OUT'
        note = re.sub(r'\s+', ' ', allout).strip()[:90]
    except subprocess.TimeoutExpired:
        run = 'TIMEOUT'
    finally:
        try:
            if os.path.exists(exe): os.remove(exe)
        except PermissionError:
            pass  # Windows can briefly retain the executable after process exit.
    return ('测试', f'{d}/{f}', 'OK', run, '-', note)

def main():
    dt = [d for d in DIRS if (not ONLY or d == ONLY) and os.path.isdir(os.path.join(ROOT, d))]
    tpl = [(d, f) for d in dt for f in sorted(os.listdir(os.path.join(ROOT, d))) if f.endswith('.cpp')]
    tests = []
    for d in dt:
        td = os.path.join(ROOT, '测试', d)
        if os.path.isdir(td):
            for f in sorted(os.listdir(td)):
                if f.endswith('.cpp'):
                    tests.append((len(tests), d, f))

    rows = []
    with cf.ThreadPoolExecutor(JOBS) as ex:
        rows += list(ex.map(tpl_check, tpl))
        rows += list(ex.map(test_check, tests))

    bad = [r for r in rows if r[2] != 'OK' or (r[0] == '测试' and r[3] != 'OK') or (r[0] == '模板' and r[4] != 'OK')]
    for r in bad:
        print(f'  [{r[0]}] {r[1]}  编译={r[2]} 运行={r[3]} 风格={r[4]}  {r[5]}')
    nt = sum(1 for r in rows if r[0] == '模板'); ns = sum(1 for r in rows if r[0] == '测试')
    print(f'\n模板 {nt} 个 + 测试 {ns} 个，异常 {len(bad)} 个')
    sys.exit(bool(bad))

if __name__ == '__main__':
    main()
