#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
把模板库排版成可打印的 PDF。
用法:
  python _台账/生成PDF.py                # 标准版（单栏）：模板库 + 结论速查
  python _台账/生成PDF.py compact        # 紧凑版（双栏、小字号、窄边距）
  python _台账/生成PDF.py split          # 按方向拆成 9 本小册子（紧凑版）
  python _台账/生成PDF.py notes          # 只做结论速查
产物都在 build/ 下。
"""
import os, re, sys, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD = os.path.join(ROOT, 'build')
PRINT = os.path.join(BUILD, 'print')
API = os.path.join(ROOT, '_台账', 'api')
DIRS = ['01-基础与技巧', '02-数据结构', '03-字符串', '04-图论', '05-数学',
        '06-动态规划', '07-搜索', '08-计算几何', '09-其他']
TITLE = {'01-基础与技巧': '基础与技巧', '02-数据结构': '数据结构', '03-字符串': '字符串',
         '04-图论': '图论', '05-数学': '数学', '06-动态规划': '动态规划', '07-搜索': '搜索',
         '08-计算几何': '计算几何', '09-其他': '其他'}


def preamble(compact=False, subtitle=''):
    doc = ('\\documentclass[UTF8,a4paper,9pt,twocolumn]{ctexart}' if compact
           else '\\documentclass[UTF8,a4paper,10pt]{ctexart}')
    geo = ('\\usepackage[margin=1.05cm,top=1.15cm,bottom=1.15cm,columnsep=0.5cm]{geometry}' if compact
           else '\\usepackage[margin=1.9cm,top=2.2cm,bottom=2.0cm]{geometry}')
    code_size = '\\tiny' if compact else '\\scriptsize'
    api_size = '\\footnotesize' if compact else '\\small'
    num = 'numbers=none,' if compact else 'numbers=left, numberstyle=\\tiny\\color{gray}, numbersep=5pt,'
    return r'''%s
%s
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
\definecolor{kw}{RGB}{70,70,180}
\definecolor{str}{RGB}{163,21,21}
\definecolor{bg}{RGB}{249,249,249}
\definecolor{rule}{RGB}{210,210,210}

\lstdefinestyle{cpp}{
  language=C++,
  backgroundcolor=\color{bg},
  basicstyle=\ttfamily%s,
  keywordstyle=\color{kw}\bfseries,
  commentstyle=\color{cmt},
  stringstyle=\color{str},
  %s
  frame=single, framerule=0.3pt, rulecolor=\color{rule},
  breaklines=true, breakatwhitespace=false,
  tabsize=4, showstringspaces=false,
  columns=fullflexible, keepspaces=true,
  xleftmargin=0.6em, framexleftmargin=0.5em,
  aboveskip=2pt, belowskip=3pt,
  literate={~}{{\raisebox{0.5ex}{\texttildelow}}}1
}
\lstset{style=cpp}
\newcommand{\apifont}{%s}

\titleformat{\section}{\large\bfseries\color{kw}}{\thesection}{0.5em}{}
\titleformat{\subsection}{\normalsize\bfseries}{\thesubsection}{0.45em}{}
\setlist{nosep,leftmargin=1.1em}
\setlength{\parindent}{0pt}
\setlength{\parskip}{1pt}
\pagestyle{fancy}\fancyhf{}
\lhead{\scriptsize ICPC 算法模板库%s}\rhead{\scriptsize\leftmark}\cfoot{\scriptsize\thepage}
\renewcommand{\headrulewidth}{0.3pt}
''' % (doc, geo, code_size, num, api_size, subtitle)


def read(p):
    with open(p, encoding='utf-8', errors='replace') as f:
        return f.read()


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


def md_to_tex(md, section_level=2, compact=False):
    out, i = [], 0
    lines = md.split('\n')
    in_code, in_list = False, False
    while i < len(lines):
        ln = lines[i]
        if ln.startswith('```'):
            if in_list: out.append(r'\end{itemize}'); in_list = False
            if not in_code:
                in_code = True; out.append(r'\begin{lstlisting}[style=cpp]')
            else:
                in_code = False; out.append(r'\end{lstlisting}')
            i += 1; continue
        if in_code:
            out.append(ln); i += 1; continue
        if ln.strip().startswith('|') and i + 1 < len(lines) and re.match(r'^\s*\|[\s:\-|]+\|\s*$', lines[i + 1]):
            header = [c.strip() for c in ln.strip().strip('|').split('|')]
            if in_list: out.append(r'\end{itemize}'); in_list = False
            ncol = len(header)
            out.append(r'\begin{longtable}{%s}' % ('p{\\dimexpr0.97\\linewidth/%d-2\\tabcolsep\\relax}' % ncol) * ncol)
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
            cmd = {1: 'section', 2: 'subsection', 3: 'subsubsection', 4: 'paragraph'}.get(lvl, 'paragraph')
            if section_level == 2:
                cmd = {1: 'subsection', 2: 'subsubsection', 3: 'paragraph', 4: 'paragraph'}.get(lvl, 'paragraph')
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
    if in_code: out.append(r'\end{lstlisting}')
    return '\n'.join(out)


def api_blocks():
    d = {}
    if os.path.isdir(API):
        for f in os.listdir(API):
            if f.endswith('.md'):
                d[os.path.splitext(f)[0]] = read(os.path.join(API, f))
    return d


def api_block(api, d, fname, compact):
    txt = api.get(d, '')
    if not txt:
        return ''
    m = re.search(r'(?ms)^##\s+' + re.escape(fname) + r'\s*$(.*?)(?=^##\s|\Z)', txt)
    if not m:
        return ''
    body = md_to_tex(m.group(1).strip(), section_level=2, compact=compact)
    return '{\\apifont\n' + body + '\n}'


def emit_templates(dirs, texname, compact, standalone_title=None):
    """把若干目录排成一本 PDF"""
    os.makedirs(PRINT, exist_ok=True)
    api = api_blocks()
    title = standalone_title or 'ICPC 算法模板库'
    tex = [preamble(compact, '' if compact else r'\\[2pt]\large 147 个模板 · 附 API 说明'),
           r'\title{\bfseries %s}' % title, r'\author{}\date{\today}', r'\begin{document}',
           r'\maketitle']
    if not compact:
        tex.append(r'\begin{center}\small 模板正文已自动去掉测试代码；每节先给 API，再给实现。\end{center}')
    tex.append(r'\tableofcontents')
    if not compact:
        tex.append(r'\clearpage')
    total = 0
    for d in dirs:
        p = os.path.join(ROOT, d)
        if not os.path.isdir(p):
            continue
        files = sorted([f for f in os.listdir(p) if f.endswith('.cpp')])
        if not files:
            continue
        tex.append('\\section{%s（%d）}' % (TITLE[d], len(files)))
        for f in files:
            core = read(os.path.join(p, f)).rstrip() + '\n'
            dst = os.path.join(PRINT, d, f)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with open(dst, 'w', encoding='utf-8') as fh:
                fh.write(core)
            tex.append('\\subsection{%s}' % f.replace('.cpp', '').replace('_', r'\_'))
            ab = api_block(api, d, f, compact)
            if ab:
                tex.append(ab)
            tex.append(r'\lstinputlisting{%s}' % os.path.join('print', d, f).replace('\\', '/'))
            total += 1
    tex.append(r'\end{document}')
    out = os.path.join(BUILD, texname)
    with open(out, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(tex))
    print('%s：%d 个模板' % (texname, total))
    return out


def gen_notes(compact=False):
    parts = [preamble(compact), r'\title{\bfseries ICPC 思路结论速查}\date{\today}',
             r'\begin{document}\maketitle', r'\tableofcontents']
    if not compact:
        parts.append(r'\clearpage')
    for rel, title in [('结论速查.md', '实战判据'),
                       ('结论速查/ICPC常用结论.md', 'ICPC 常用结论'),
                       ('结论速查/CF-AtCoder逐题结论.md', 'CF / AtCoder 逐题结论')]:
        p = os.path.join(ROOT, rel)
        if os.path.exists(p):
            parts.append('\\section{%s}' % title)
            parts.append(md_to_tex(read(p), section_level=2, compact=compact))
    parts.append(r'\end{document}')
    out = os.path.join(BUILD, '结论速查.tex' if not compact else '结论速查-紧凑.tex')
    with open(out, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(parts))
    print('结论 tex：%s' % os.path.basename(out))
    return out


def pages(pdf):
    try:
        data = open(pdf, 'rb').read()
        n = len(re.findall(rb'/Type\s*/Page[^s]', data))
        return n or None
    except Exception:
        return None


def compile_tex(texfile):
    cwd = os.path.dirname(texfile)
    base = os.path.basename(texfile)
    for _ in range(2):
        subprocess.run(['latexmk', '-xelatex', '-interaction=nonstopmode', '-halt-on-error', base],
                       cwd=cwd, capture_output=True, text=True, encoding='utf-8', errors='replace')
    pdf = os.path.join(cwd, base.replace('.tex', '.pdf'))
    if os.path.exists(pdf):
        p = pages(pdf)
        print('  -> %s  %.1f MB  %s 页' % (os.path.basename(pdf), os.path.getsize(pdf) / 1e6,
                                           p if p else '?'))
    else:
        print('  -> %s 编译失败' % base)
        log = os.path.join(cwd, base.replace('.tex', '.log'))
        if os.path.exists(log):
            print('\n'.join([l for l in read(log).split('\n') if l.startswith('!')][:10]))
    return pdf


if __name__ == '__main__':
    os.makedirs(BUILD, exist_ok=True)
    what = sys.argv[1] if len(sys.argv) > 1 else 'std'
    if what == 'std':
        compile_tex(emit_templates(DIRS, '算法模板库.tex', False))
        compile_tex(gen_notes(False))
    elif what == 'compact':
        compile_tex(emit_templates(DIRS, '算法模板库-紧凑.tex', True, 'ICPC 算法模板库（紧凑版）'))
        compile_tex(gen_notes(True))
    elif what == 'split':
        for d in DIRS:
            compile_tex(emit_templates([d], '模板-%s.tex' % TITLE[d], True,
                                       'ICPC 模板 · %s' % TITLE[d]))
    elif what == 'notes':
        compile_tex(gen_notes(False))
    else:
        print('用法: python 生成PDF.py [std|compact|split|notes]')
