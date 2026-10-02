#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""统一入口：生成当前完整双栏手册，并使用已有 XeLaTeX 编译。

python _台账/生成PDF.py
python _台账/生成PDF.py --current  # 直接编译当前 tex，保留编辑器里的调整
"""
import argparse
import hashlib
import importlib.util
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parent.parent
TEX = ROOT / 'ICPC算法手册.tex'
BUILD = ROOT / 'build' / 'latex'
OUTPUT = ROOT / 'output' / 'pdf'

def compiler():
    configured = os.environ.get('XELATEX')
    if configured:
        return configured
    found = shutil.which('xelatex')
    if found:
        return found
    if os.name == 'nt':
        for year in range(2030, 2020, -1):
            p = Path(f'C:/texlive/{year}/bin/windows/xelatex.exe')
            if p.is_file():
                return str(p)
    raise SystemExit('XeLaTeX not found; set XELATEX to an existing compiler.')

def state():
    return tuple(hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else ''
                 for p in [BUILD / (TEX.stem + '.aux'), BUILD / (TEX.stem + '.toc')])

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--current', action='store_true')
    args = parser.parse_args()
    if not args.current:
        spec = importlib.util.spec_from_file_location('manual_source', ROOT / '_台账' / '生成LaTeX.py')
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        module.main()
    if not TEX.exists():
        raise SystemExit('Generate the .tex source first.')
    engine = compiler()
    BUILD.mkdir(parents=True, exist_ok=True)
    OUTPUT.mkdir(parents=True, exist_ok=True)
    # 任何一次编译失败都不复制旧 PDF 冒充本次结果。
    for attempt in range(3):
        before = state()
        result = subprocess.run([engine, '-no-shell-escape', '-interaction=nonstopmode',
                                 '-halt-on-error', f'-output-directory={BUILD}', str(TEX)],
                                cwd=ROOT, capture_output=True, encoding='utf-8', errors='replace')
        (BUILD / 'compile-output.txt').write_text(result.stdout + result.stderr, encoding='utf-8')
        if result.returncode:
            raise SystemExit('Compilation failed; see build/latex/compile-output.txt')
        if before == state():
            break
    else:
        raise SystemExit('Page references have not stabilized; compile the current source again.')
    pdf = BUILD / (TEX.stem + '.pdf')
    if not pdf.is_file():
        raise SystemExit('Compiler reported success but no PDF was produced.')
    shutil.copy2(pdf, OUTPUT / pdf.name)
    print('PDF ready: output/pdf/' + pdf.name.encode('ascii', 'backslashreplace').decode())

if __name__ == '__main__':
    main()
