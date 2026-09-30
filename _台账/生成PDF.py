#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
把模板库排版成可打印的 PDF：
  1) 算法模板库.pdf  —— 每个模板：API 说明 + 模板正文（自动裁掉自测代码）
  2) 结论速查.pdf    —— 结论速查 + ICPC 常用结论 + CF/AtCoder 逐题结论 + 台账统计
用法:
  python _台账/生成PDF.py            # 生成两份
  python _台账/生成PDF.py templates  # 只生成模板库
  python _台账/生成PDF.py notes      # 只生成结论
"""
import os, re, sys, shutil, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD = os.path.join(ROOT, 'build')
PRINT = os.path.join(BUILD, 'print')
API = os.path.join(ROOT, '_台账', 'api')
MARK = '// ===== 以下为自测代码'

DIRS = ['01-基础与技巧', '02-数据结构', '03-字符串', '04-图论', '05-数学',
        '06-动态规划', '07-搜索', '08-计算几何', '09-其他']
TITLE = {'01-基础与技巧': '基础与技巧', '02-数据结构': '数据结构', '03-字符串': '字符串',
         '04-图论': '图论', '05-数学': '数学', '06-动态规划': '动态规划', '07-搜索': '搜索',
         '08-计算几何': '计算几何', '09-其他': '其他'}

PREAMBLE = r'''\documentclass[UTF8,a4paper,10pt]{ctexart}
\usepackage[margin=1.9cm,top=2.2cm,bottom=2.0cm]{geometry}
\usepackage{listings}
\usepackage{xcolor}
\usepackage{amsmath,amssymb}
\usepackage{booktabs}
\usepackage{longtable}
\usepackage{array}
\usepackage{enumitem}
\usepackage{fancyhdr}
\usepackage[hidelinks]{hyperref}
\usepackage{titlesec}

\definecolor{cmt}{RGB}{106,153,85}
\definecolor{kw}{RGB}{86,86,196}
\definecolor{str}{RGB}{163,21,21}
\definecolor{bg}{RGB}{248,248,248}
\definecolor{rule}{RGB}{205,205,205}

\lstdefinestyle{cpp}{
  language=C++,
  backgroundcolor=\color{bg},
  basicstyle=\ttfamily\scriptsize,
  keywordstyle=\color{kw}\bfseries,
  commentstyle=\color{cmt},
  stringstyle=\color{str},
  numbers=left, numberstyle=\tiny\color{gray}, numbersep=5pt,
  frame=single, framerule=0.4pt, rulecolor=\color{rule},
  breaklines=true, breakatwhitespace=false,
  tabsize=4, showstringspaces=false,
  columns=fullflexible, keepspaces=true,
  xleftmargin=1.2em, framexleftmargin=1.0em,
  aboveskip=4pt, belowskip=6pt,
  literate={~}{{\raisebox{0.5ex}{\texttildelow}}}1
}
\lstset{style=cpp}

\titleformat{\section}{\Large\bfseries\color{kw}}{\thesection}{0.6em}{}
\titleformat{\subsection}{\large\bfseries}{\thesubsection}{0.6em}{}
\setlist{nosep,leftmargin=1.4em}
\setlength{\parindent}{0pt}
\setlength{\parskip}{2pt}
\pagestyle{fancy}\fancyhf{}
\lhead{\small ICPC 算法模板库}\rhead{\small\leftmark}\cfoot{\small\thepage}
\renewcommand{\headrulewidth}{0.3pt}
'''

def read(p):
    with open(p, encoding='utf-8', errors='replace') as f:
        return f.read()

def strip_selftest(code):
    """裁掉自测代码：优先用分隔标记，没有就从第一个 int main( 截断"""
    i = code.find(MARK)
    if i >= 0:
        return code[:i].rstrip() + '\n'
    m = re.search(r'(?m)^int\s+main\s*\(', code)
    if m:
        head = code[:m.start()].rstrip()
        # 顺手把紧贴在 main 之前的纯测试辅助函数去掉（以 rnd/brute/check/test_ 开头）
        lines = head.split('\n')
        while lines:
            tail = lines[-1].strip()
            if tail == '' or tail == '}' or re.match(r'^(void|int|bool|ll|long long|double)\s+(rnd|rndint|brute|check|test_|dbg)', tail):
                lines.pop()
            else:
                break
        return '\n'.join(lines).rstrip() + '\n'
    return code.rstrip() + '\n'

# ---------------- Markdown -> LaTeX ----------------
def esc(t):
    t = t.replace('\\', r'\textbackslash{}')
    for a, b in [('&', r'\&'), ('%', r'\%'), ('$', r'\$'), ('#', r'\#'),
                 ('_', r'\_'), ('{', r'\{'), ('}', r'\}')]:
        t = t.replace(a, b)
    t = t.replace('~', r'\textasciitilde{}').replace('^', r'\textasciicircum{}')
    t = re.sub(r'\*\*(.+?)\*\*', r'\\textbf{\1}', t)
    t = re.sub(r'`([^`]+)`', r'\\texttt{\1}', t)
    return t

def md_to_tex(md, section_level=1):
    out, i = [], 0
    lines = md.split('\n')
    in_code, in_list, in_table = False, False, False
    while i < len(lines):
        ln = lines[i]
        if ln.startswith('```'):
            if in_list: out.append(r'\end{itemize}'); in_list = False
            if in_table: out.append(r'\end{longtable}'); in_table = False
            if not in_code:
                in_code = True; out.append(r'\begin{lstlisting}[style=cpp]')
            else:
                in_code = False; out.append(r'\end{lstlisting}')
            i += 1; continue
        if in_code:
            out.append(ln); i += 1; continue
        # 表格
        if ln.strip().startswith('|') and i + 1 < len(lines) and re.match(r'^\s*\|[\s:\-|]+\|\s*$', lines[i+1]):
            header = [c.strip() for c in ln.strip().strip('|').split('|')]
            if in_list: out.append(r'\end{itemize}'); in_list = False
            ncol = len(header)
            out.append(r'\begin{longtable}{%s}' % ('p{\\dimexpr0.98\\linewidth/%d-2\\tabcolsep\\relax}' % ncol) * ncol)
            out.append(r'\toprule ' + ' & '.join(esc(h) for h in header) + r' \\ \midrule\endfirsthead')
            out.append(r'\toprule ' + ' & '.join(esc(h) for h in header) + r' \\ \midrule\endhead')
            i += 2
            while i < len(lines) and lines[i].strip().startswith('|'):
                cells = [c.strip() for c in lines[i].strip().strip('|').split('|')]
                cells += [''] * (ncol - len(cells))
                out.append(' & '.join(esc(c) for c in cells[:ncol]) + r' \\')
                i += 1
            out.append(r'\bottomrule\end{longtable}')
            continue
        m = re.match(r'^(#{1,4})\s+(.*)$', ln)
        if m:
            if in_list: out.append(r'\end{itemize}'); in_list = False
            lvl = len(m.group(1))
            if section_level == 1:
                cmd = {1: 'section', 2: 'subsection', 3: 'subsubsection', 4: 'paragraph'}[lvl]
            else:
                cmd = {1: 'subsection', 2: 'subsubsection', 3: 'paragraph', 4: 'paragraph'}[lvl]
            out.append('\\%s{%s}' % (cmd, esc(m.group(2))))
            i += 1; continue
        m = re.match(r'^\s*[-*]\s+(.*)$', ln)
        if m:
            if not in_list: out.append(r'\begin{itemize}'); in_list = True
            out.append(r'\item ' + esc(m.group(1))); i += 1; continue
        if ln.strip() == '':
            if in_list: out.append(r'\end{itemize}'); in_list = False
            out.append(''); i += 1; continue
        if in_list: out.append(r'\end{itemize}'); in_list = False
        out.append(esc(ln)); i += 1
    if in_list: out.append(r'\end{itemize}')
    if in_table: out.append(r'\end{longtable}')
    if in_code: out.append(r'\end{lstlisting}')
    return '\n'.join(out)

# ---------------- 生成模板库 tex ----------------
def gen_templates():
    os.makedirs(PRINT, exist_ok=True)
    api = {}
    if os.path.isdir(API):
        for f in os.listdir(API):
            if f.endswith('.md'):
                api[os.path.splitext(f)[0]] = read(os.path.join(API, f))

    def api_block(d, fname):
        txt = api.get(d, '')
        if not txt:
            return ''
        m = re.search(r'(?ms)^##\s+' + re.escape(fname) + r'\s*$(.*?)(?=^##\s|\Z)', txt)
        return md_to_tex(m.group(1).strip(), section_level=2) if m else ''

    tex = [PREAMBLE,
           r'\title{\bfseries ICPC 算法模板库\\[2pt]\large 147 个模板 · 附 API 说明}',
           r'\author{}\date{\today}', r'\begin{document}', r'\maketitle',
           r'\begin{center}\small 模板正文已自动去掉自测代码；每节先给 API，再给实现。\end{center}',
           r'\tableofcontents\clearpage']

    total = 0
    for d in DIRS:
        p = os.path.join(ROOT, d)
        if not os.path.isdir(p): continue
        files = sorted([f for f in os.listdir(p) if f.endswith('.cpp')], key=lambda s: s)
        if not files: continue
        tex.append('\\section{%s（%d 个）}' % (TITLE[d], len(files)))
        for f in files:
            src = read(os.path.join(p, f))
            core = strip_selftest(src)
            rel = os.path.join(d, f)
            dst = os.path.join(PRINT, d, f)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with open(dst, 'w', encoding='utf-8') as fh:
                fh.write(core)
            tex.append('\\subsection{%s}' % f.replace('.cpp', '').replace('_', r'\_'))
            ab = api_block(d, f)
            if ab:
                tex.append(ab)
            else:
                tex.append(r'\textit{（API 说明见 \_台账/api/ 目录）}')
            tex.append(r'\lstinputlisting{%s}' % os.path.join('print', d, f).replace('\\', '/'))
            tex.append(r'\begin{flushright}\scriptsize 测试与对拍：\texttt{测试/%s/%s}\end{flushright}'
                       % (d.replace('_', r'\_'), f.replace('.cpp', '_test.cpp').replace('_', r'\_')))
            total += 1
    tex.append(r'\end{document}')
    out = os.path.join(BUILD, '算法模板库.tex')
    with open(out, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(tex))
    print('模板库 tex 生成：%s（%d 个模板）' % (out, total))
    return out

# ---------------- 生成结论 tex ----------------
def gen_notes():
    parts = [PREAMBLE,
             r'\title{\bfseries ICPC 思路结论速查}\date{\today}\begin{document}\maketitle\tableofcontents\clearpage']
    notes = [('结论速查.md', '实战判据'), ('结论速查/ICPC常用结论.md', 'ICPC 常用结论'),
             ('结论速查/CF-AtCoder逐题结论.md', 'CF / AtCoder 逐题结论')]
    for rel, title in notes:
        p = os.path.join(ROOT, rel)
        if not os.path.exists(p):
            continue
        parts.append('\\section{%s}' % title)
        parts.append(md_to_tex(read(p), section_level=2))
    parts.append(r'\end{document}')
    out = os.path.join(BUILD, '结论速查.tex')
    with open(out, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(parts))
    print('结论 tex 生成：%s' % out)
    return out

def compile_tex(texfile):
    cwd = os.path.dirname(texfile)
    base = os.path.basename(texfile)
    for _ in range(2):
        r = subprocess.run(['latexmk', '-xelatex', '-interaction=nonstopmode', '-halt-on-error', base],
                           cwd=cwd, capture_output=True, text=True, encoding='utf-8', errors='replace')
    pdf = os.path.join(cwd, base.replace('.tex', '.pdf'))
    ok = os.path.exists(pdf)
    print(('  -> %s %s' % (pdf, 'OK' if ok else 'FAILED')))
    if not ok:
        log = os.path.join(cwd, base.replace('.tex', '.log'))
        if os.path.exists(log):
            errs = [l for l in read(log).split('\n') if l.startswith('!')][:12]
            print('\n'.join(errs))
    return pdf

if __name__ == '__main__':
    os.makedirs(BUILD, exist_ok=True)
    what = sys.argv[1] if len(sys.argv) > 1 else 'all'
    if what in ('all', 'templates'):
        compile_tex(gen_templates())
    if what in ('all', 'notes'):
        compile_tex(gen_notes())
