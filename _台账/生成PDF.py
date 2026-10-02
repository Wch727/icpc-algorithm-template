#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
把模板库排版成可打印 PDF。四种版本：
  python _台账/生成PDF.py ref      # 【推荐】参考版：单栏、字号正常，只印 API + 核心代码节选
  python _台账/生成PDF.py compact  # 紧凑版：双栏小字，印完整模板（省纸但字小）
  python _台账/生成PDF.py split    # 按方向拆成 9 本小册子（参考版规格）
  python _台账/生成PDF.py std      # 标准版：单栏，印完整模板
  python _台账/生成PDF.py notes    # 只做结论速查
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

REF_CAP = 40          # 参考版每个算法最多印多少行核心代码


def preamble(mode='std'):
    """mode: ref / compact / std"""
    if mode == 'compact':
        doc = '\\documentclass[UTF8,a4paper,9pt,twocolumn]{ctexart}'
        geo = '\\usepackage[margin=1.05cm,top=1.15cm,bottom=1.15cm,columnsep=0.5cm]{geometry}'
        code_size, num, api = '\\tiny', 'numbers=none,', '\\footnotesize'
    elif mode == 'ref':
        doc = '\\documentclass[UTF8,a4paper,11pt]{ctexart}'
        geo = '\\usepackage[margin=1.5cm,top=1.6cm,bottom=1.5cm]{geometry}'
        code_size, num, api = '\\small', 'numbers=left, numberstyle=\\scriptsize\\color{gray}, numbersep=6pt,', '\\normalsize'
    else:
        doc = '\\documentclass[UTF8,a4paper,10pt]{ctexart}'
        geo = '\\usepackage[margin=1.9cm,top=2.2cm,bottom=2.0cm]{geometry}'
        code_size, num, api = '\\scriptsize', 'numbers=left, numberstyle=\\tiny\\color{gray}, numbersep=5pt,', '\\small'
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
\definecolor{bg}{RGB}{250,250,250}
\definecolor{rule}{RGB}{205,205,205}

\lstdefinestyle{cpp}{
  language=C++,
  backgroundcolor=\color{bg},
  basicstyle=\ttfamily%s,
  keywordstyle=\color{kw}\bfseries,
  commentstyle=\color{cmt},
  stringstyle=\color{str},
  %s
  frame=single, framerule=0.4pt, rulecolor=\color{rule},
  breaklines=true, breakatwhitespace=false,
  tabsize=4, showstringspaces=false,
  columns=fullflexible, keepspaces=true,
  xleftmargin=1.0em, framexleftmargin=0.8em,
  aboveskip=4pt, belowskip=5pt,
  literate={~}{{\raisebox{0.5ex}{\texttildelow}}}1
}
\lstset{style=cpp}
\newcommand{\apifont}{%s}

\titleformat{\section}{\Large\bfseries\color{kw}}{\thesection}{0.6em}{}
\titleformat{\subsection}{\large\bfseries}{\thesubsection}{0.5em}{}
\setlist{nosep,leftmargin=1.3em}
\setlength{\parindent}{0pt}
\setlength{\parskip}{2pt}
\pagestyle{fancy}\fancyhf{}
\lhead{\small ICPC 算法模板库}\rhead{\small\leftmark}\cfoot{\small\thepage}
\renewcommand{\headrulewidth}{0.3pt}
''' % (doc, geo, code_size, num, api)


def read(p):
    with open(p, encoding='utf-8', errors='replace') as f:
        return f.read()


# ---------------- 核心代码节选（参考版用） ----------------
DROP = re.compile(r'^\s*(#include|#pragma|using namespace|typedef\s+long\s+long\s+ll;|typedef\s+unsigned|const\s+int\s+N\s*=)')
KEEP_DECL = re.compile(r'^\s*(struct|class|template|inline|constexpr|namespace)\b')


def core_excerpt(code, cap=REF_CAP, fname=''):
    """去掉头文件/空行/长注释，保留结构体声明与函数体；超过 cap 行则截断"""
    lines = code.split('\n')
    out, i = [], 0
    while i < len(lines):
        ln = lines[i]
        s = ln.strip()
        i += 1
        if s == '':
            continue
        if DROP.match(ln):
            continue
        # 纯注释行：短的（说明性）保留，长的丢掉
        if s.startswith('//'):
            if len(s) <= 46:
                out.append(ln.rstrip())
            continue
        out.append(ln.rstrip())
    # 连续空行已去掉；把只剩 "{" 或 "}" 的行保留（结构清晰）
    text = '\n'.join(out).strip('\n')
    dropped = False
    if len(out) > cap:
        # 尽量在函数/结构体边界处截断
        cut = cap
        for j in range(cap, max(cap - 12, 0), -1):
            if j < len(out) and out[j].strip() in ('}', '};'):
                cut = j + 1
                break
        text = '\n'.join(out[:cut]).rstrip()
        dropped = True
    if dropped:
        text += '\n// ……（完整实现见 %s）' % fname
    return text


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


def md_to_tex(md, section_level=2):
    out, i = [], 0
    lines = md.split('\n')
    in_code, in_list = False, False
    while i < len(lines):
        ln = lines[i]
        if ln.strip().startswith('```'):
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


def api_block(api, d, fname):
    txt = api.get(d, '')
    if not txt:
        return ''
    m = re.search(r'(?ms)^##\s+' + re.escape(fname) + r'\s*$(.*?)(?=^##\s|\Z)', txt)
    return md_to_tex(m.group(1).strip(), section_level=2) if m else ''


def emit_templates(dirs, texname, mode, title=None):
    os.makedirs(PRINT, exist_ok=True)
    api = api_blocks()
    head = ('ICPC 算法模板库' if mode != 'ref' else 'ICPC 算法模板库 · 参考版')
    if title:
        head = title
    tex = [preamble(mode), r'\title{\bfseries %s}' % head, r'\author{}\date{\today}', r'\begin{document}', r'\maketitle']
    if mode == 'ref':
        tex.append(r'\begin{center}\small 每个算法给：功能 / 接口（参数怎么传）/ 复杂度 / 易错 / 结论 + 核心代码节选。'
                   r'完整实现见仓库里的 \texttt{0X-*} 目录。\end{center}')
    tex.append(r'\tableofcontents')
    if mode != 'compact':
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
            code = read(os.path.join(p, f))
            body = core_excerpt(code, REF_CAP, '%s/%s' % (d, f)) if mode == 'ref' else code.rstrip() + '\n'
            dst = os.path.join(PRINT, d, f)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with open(dst, 'w', encoding='utf-8') as fh:
                fh.write(body if body.endswith('\n') else body + '\n')
            tex.append('\\subsection{%s}' % f.replace('.cpp', '').replace('_', r'\_'))
            ab = api_block(api, d, f)
            if ab:
                tex.append('{\\apifont\n' + ab + '\n}' if mode == 'ref' else ab)
            tex.append(r'\lstinputlisting{%s}' % os.path.join('print', d, f).replace('\\', '/'))
            total += 1
    tex.append(r'\end{document}')
    out = os.path.join(BUILD, texname)
    with open(out, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(tex))
    print('%s：%d 个模板' % (texname, total))
    return out


def gen_notes(mode='std'):
    parts = [preamble(mode), r'\title{\bfseries ICPC 思路结论速查}\date{\today}', r'\begin{document}\maketitle', r'\tableofcontents']
    if mode != 'compact':
        parts.append(r'\clearpage')
    for rel, title in [('结论速查.md', '实战判据'), ('结论速查/ICPC常用结论.md', 'ICPC 常用结论'),
                       ('结论速查/CF-AtCoder逐题结论.md', 'CF / AtCoder 逐题结论')]:
        p = os.path.join(ROOT, rel)
        if os.path.exists(p):
            parts.append('\\section{%s}' % title)
            parts.append(md_to_tex(read(p), section_level=2))
    parts.append(r'\end{document}')
    out = os.path.join(BUILD, '结论速查%s.tex' % ('-紧凑' if mode == 'compact' else ''))
    with open(out, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(parts))
    print('结论 tex：%s' % os.path.basename(out))
    return out


def compile_tex(texfile):
    cwd = os.path.dirname(texfile)
    base = os.path.basename(texfile)
    for _ in range(2):
        subprocess.run(['latexmk', '-xelatex', '-interaction=nonstopmode', '-halt-on-error', base],
                       cwd=cwd, capture_output=True, text=True, encoding='utf-8', errors='replace')
    pdf = os.path.join(cwd, base.replace('.tex', '.pdf'))
    log = os.path.join(cwd, base.replace('.tex', '.log'))
    pg = '?'
    if os.path.exists(log):
        m = re.findall(r'Output written on .*\((\d+) pages?,', read(log))
        if m: pg = m[-1]
    if os.path.exists(pdf):
        print('  -> %s  %.1f MB  %s 页' % (os.path.basename(pdf), os.path.getsize(pdf) / 1e6, pg))
    else:
        print('  -> %s 编译失败' % base)
        if os.path.exists(log):
            print('\n'.join([l for l in read(log).split('\n') if l.startswith('!')][:10]))


if __name__ == '__main__':
    os.makedirs(BUILD, exist_ok=True)
    what = sys.argv[1] if len(sys.argv) > 1 else 'std'
    if what == 'ref':
        compile_tex(emit_templates(DIRS, '算法模板库-参考版.tex', 'ref'))
    elif what == 'refnotes':
        compile_tex(emit_templates(DIRS, '算法模板库-参考版.tex', 'ref'))
        compile_tex(gen_notes('ref'))
    elif what == 'std':
        compile_tex(emit_templates(DIRS, '算法模板库.tex', 'std'))
        compile_tex(gen_notes('std'))
    elif what == 'compact':
        compile_tex(emit_templates(DIRS, '算法模板库-紧凑.tex', 'compact'))
        compile_tex(gen_notes('compact'))
    elif what == 'split':
        for d in DIRS:
            compile_tex(emit_templates([d], '参考版-%s.tex' % TITLE[d], 'ref', 'ICPC 模板参考 · %s' % TITLE[d]))
    elif what == 'notes':
        compile_tex(gen_notes('std'))
    else:
        print('用法: python 生成PDF.py [ref|refnotes|compact|split|std|notes]')
