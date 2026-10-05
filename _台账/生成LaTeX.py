#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""生成自包含双栏手册；不截断实现，不依赖额外项目文件。

python _台账/生成LaTeX.py
配置：_台账/打印配置.json；输出：ICPC算法手册.tex
"""
import json
import re
from functools import lru_cache
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = ROOT / '_台账' / '打印配置.json'
OUT = ROOT / 'ICPC算法手册.tex'
CHAPTERS = [
    ('01-基础与技巧', '基础与技巧'), ('02-数据结构', '数据结构'),
    ('03-字符串', '字符串'), ('04-图论', '图论'), ('05-数学', '数学'),
    ('06-动态规划', '动态规划'), ('07-搜索', '搜索'),
    ('08-计算几何', '计算几何'), ('09-其他', '实现与调试'),
]

PREAMBLE = r'''% 自动生成；修改模板或打印配置后运行 _台账/生成LaTeX.py。
% 独立文件：XeLaTeX 编译两次即可，不需要 input 或外部代码文件。
\documentclass[UTF8,fontset=fandol,a4paper,twoside]{ctexart}
\usepackage[inner=11mm,outer=9mm,top=11mm,bottom=12mm,
    headheight=11pt,headsep=3mm,footskip=6mm]{geometry}
\usepackage{amsmath,amssymb,multicol,listings,xcolor,enumitem,fancyhdr,titlesec,tabularx}
\usepackage[hidelinks,unicode]{hyperref}
\setmonofont{lmmono10-regular.otf}[BoldFont=lmmonolt10-bold.otf]
\setCJKmonofont{FandolFang-Regular}[BoldFont=FandolHei-Regular]
\definecolor{ink}{gray}{0.18}
\setlength{\columnsep}{6mm}
\setlength{\columnseprule}{0.2pt}
\setlength{\parindent}{0pt}
\setlength{\parskip}{1pt}
\setlength{\multicolsep}{3pt}
\setlength{\abovedisplayskip}{3pt}
\setlength{\belowdisplayskip}{3pt}
\setlength{\abovedisplayshortskip}{2pt}
\setlength{\belowdisplayshortskip}{2pt}
\setlist[itemize]{nosep,leftmargin=1.1em,topsep=1pt}
\titleformat{\section}{\fontsize{11}{12}\selectfont\bfseries}{\thesection}{0.4em}{}
\titleformat{\subsection}{\fontsize{9}{10}\selectfont\bfseries}{\thesubsection}{0.4em}{}
\titlespacing*{\section}{0pt}{7pt}{3pt}
\titlespacing*{\subsection}{0pt}{5pt}{2pt}
\setcounter{tocdepth}{2}
\makeatletter
\renewcommand{\@pnumwidth}{2em}
\renewcommand{\l@section}{\@dottedtocline{1}{0em}{1.6em}}
\renewcommand{\l@subsection}{\@dottedtocline{2}{0.8em}{2.7em}}
\makeatother
\pagestyle{fancy}\fancyhf{}
\fancyhead[LE,RO]{\fontsize{7.5}{9}\selectfont\thepage}
\fancyhead[LO,RE]{\fontsize{7.5}{9}\selectfont ICPC 算法手册\quad\nouppercase{\leftmark}}
\renewcommand{\headrulewidth}{0.2pt}
\renewcommand{\sectionmark}[1]{\markboth{\thesection\ #1}{}}
\lstset{language=C++,basicstyle=\ttfamily\fontsize{@CODE@}{@LEAD@}\selectfont,
    keywordstyle=\bfseries,commentstyle=\color{ink},stringstyle=\color{ink},
    numbers=none,frame=none,columns=fullflexible,keepspaces=true,
    tabsize=4,showstringspaces=false,showspaces=false,escapeinside={(*@}{@*)},
    breaklines=true,breakatwhitespace=false,breakindent=1em,
    aboveskip=2pt,belowskip=3pt,
    literate={Σ}{{$\Sigma$}}1 {∑}{{$\sum$}}1 {Δ}{{$\Delta$}}1
    {α}{{$\alpha$}}1 {π}{{$\pi$}}1 {φ}{{$\varphi$}}1 {θ}{{$\theta$}}1
    {μ}{{$\mu$}}1 {σ}{{$\sigma$}}1 {Π}{{$\Pi$}}1 {∏}{{$\prod$}}1
    {≤}{{$\le$}}1 {≥}{{$\ge$}}1 {≠}{{$\ne$}}1 {≈}{{$\approx$}}1
    {×}{{$\times$}}1 {∈}{{$\in$}}1 {∞}{{$\infty$}}1
    {≡}{{$\equiv$}}1 {⊆}{{$\subseteq$}}1
    {→}{{$\to$}}1 {⇒}{{$\Rightarrow$}}1 {⊕}{{$\oplus$}}1
    {²}{{$^2$}}1 {³}{{$^3$}}1 {√}{{$\sqrt{\ }$}}1
}
\newcommand{\topic}[1]{\par\smallskip\textbf{#1}\par\nobreak}
\newcommand{\note}[1]{{\fontsize{8}{9.5}\selectfont #1\par}}
\begin{document}
\fontsize{8.3}{9.8}\selectfont
'''

FORMULAS = {
    '01-基础与技巧/单调队列.cpp': r'''
\topic{等长区间平移：差集配对}
\note{旧区间 $[s,s+L)$ 右移 $d$，$0\le d\le L$。重叠部分不变；移出 $[s,s+d)$，移入 $[s+L,s+L+d)$。记移出、移入边际收益为 $u_i,v_i$，则}
\[\Delta(s,d)=\sum_{i=s}^{s+d-1}(u_i+v_{i+L}).\]
\note{例如最大化恰被覆盖 $k$ 次的位置数，原覆盖次数为 $c_i$：$u_i=[c_i=k+1]-[c_i=k]$，$v_i=[c_i=k-1]-[c_i=k]$。令 $z_i=u_i+v_{i+L}$，允许 $d\le L$ 时变成长度至多 $L$ 的最大子段和，用前缀最小值队列。保留不移动的零收益；$d>L$ 为不交情形，须另算（2025 成都 K）。}
''',
    '06-动态规划/数位DP.cpp': r'''
\topic{最优计数：后续乘零}
\note{同一状态的历史，若后续目标乘非负系数 $w$，应同时维护最优值、最优方案数 \texttt{ways} 与全部可行方案数 \texttt{all}。$w>0$ 延用 \texttt{ways}，$w=0$ 改用 \texttt{all}：旧分数 2、5 各一种，乘零后有两种最优方案。负系数还需维护最小值。分支须无重复，且状态内历史的后续可行性相同；仅方案数取模，目标值不可取模后比较（2025 沈阳 D）。}
''',
    '04-图论/线段树优化建图.cpp': r'''
\topic{子树同深度约束压成区间}
\note{DFS 序用半开子树区间 $[tin_u,tout_u)$；按绝对深度分桶，桶内按 $tin$ 排序。目标深度桶中两次 \texttt{lower\_bound} 定位子树集合，用桶内编号作线段树区间坐标。各桶总长度 $O(n)$，不需开“深度乘点数”个节点。}
\note{禁止 $u$ 与区间内任一 $x$ 同选时，蕴含为 $u\to\neg x$ 和 $x\to\neg u$，分别压成点到区间、区间到点。辅助节点仅压缩可达性，不代表布尔变量，不可随意用 xor 1 取反；需排除 $u$ 时拆开它所在位置（2025 武汉 B）。}
''',
    '01-基础与技巧/高维差分.cpp': r'''
\note{令 $x=(x_1,\ldots,x_D)$，$e_i$ 为单位向量，$b\in\{0,1\}^D$，
$|b|=\sum b_i$。任一坐标为 0 时，数组及中间阶段的值均为 0。}
\textbf{差分与直接还原}
\begin{align*}
d(x)&=\sum_{b\in\{0,1\}^D}(-1)^{|b|}a(x-b),\\
a(x)&=d(x)+\sum_{b\ne0}(-1)^{|b|+1}a(x-b).
\end{align*}
\textbf{逐维递推：$O(DS)$，$S$ 为格数}
\begin{align*}
G_0(x)&=a(x),\\
G_i(x)&=G_{i-1}(x)-G_{i-1}(x-e_i),\quad G_D=d,\\
F_0(x)&=d(x),\\
F_i(x)&=F_{i-1}(x)+F_i(x-e_i),\quad F_D=a.
\end{align*}
\note{原地建表沿当前维倒序，还原正序。直接容斥还原为 $O(2^D S)$。}
\textbf{闭区域加 $v$：$2^D$ 个角点}
\[
p_i=\begin{cases}l_i,&b_i=0,\\r_i+1,&b_i=1,\end{cases}
\qquad d(p)\mathrel{+}=(-1)^{|b|}v.
\]
\note{各维预留第 0 层及右端点加 1。全零初始可直接修改；结果只还原一次。
下方三维代码是逐维递推的具体写法。}
''',
    '06-动态规划/概率期望DP.cpp': r'''
\textbf{自环移项、线性性、尾和}
\[
E=c+pE+\sum_jq_jE_j
\quad\Longrightarrow\quad E=\frac{c+\sum_jq_jE_j}{1-p}.
\]
\[
\mathbb E\Bigl[\sum_iX_i\Bigr]=\sum_i\mathbb E[X_i],\qquad
\mathbb E[T]=\sum_{k\ge0}\Pr(T>k).
\]
\note{第一式要求 $p<1$ 且期望有限；尾和适用于非负整数随机变量。
无需独立性即可使用期望线性性。}
\note{正推代码是截断近似；当前存活概率小并不保证期望尾项小。若未终止状态的剩余期望统一不超过 $B$，截断到 $K$ 后的误差才可界为 $B\Pr(T>K)$。终点可能永不到达时，须先判断期望是否有限。}
\note{若 $S$ 是已取得的项目集合，每步等概率抽取 $n$ 项中的一项：}
\[
E[S]=\frac{n+\sum_{i\notin S}E[S\cup\{i\}]}{n-|S|},
\quad E[\text{完成状态}]=0.
\]
\note{稳态 move-to-front 模型中，若 $p_i+p_j>0$，
一对项目的期望逆序贡献为 $p_ip_j/(p_i+p_j)$；零概率对贡献为 0。}
''',
    '05-数学/多项式求逆与ln_exp.cpp': r'''
\note{Bell 数的指数生成函数；最终系数乘 $n!$，模数须允许所需分母求逆。}
\[
\sum_{n\ge0}B_n\frac{x^n}{n!}=\exp(e^x-1).
\]
''',
    '05-数学/容斥原理与排列组合.cpp': r'''
\textbf{范德蒙德卷积}
\[
\sum_j\binom rj\binom s{k-j}=\binom{r+s}k.
\]
\note{对 $a_1\le\cdots\le a_n$，枚举所有子集，$a_i$ 作为子集第 $j$ 小元素的次数为
$\binom{i-1}{j-1}2^{n-i}$。若该位置权重依次为 $1,1,2,4,\ldots$，总系数为}
\[
\frac{3^{i-1}+1}{2}\,2^{n-i}.
\]
\note{两个不同指定对象分别禁放两个指定端点，$k\ge2$：
$k!-2(k-1)!+(k-2)!$。模意义除法须可逆。}
''',
    '08-计算几何/闵可夫斯基和.cpp': r'''
\topic{随机平移下的交面积期望}
\note{非退化凸多边形 $P,Q$，在交面积为正的平移向量域内均匀采样 $t$。该域为 $P+(-Q)$ 的内部，边界零测度，因此}
\[\mathbb E\,|P\cap(Q+t)|=\frac{|P|\,|Q|}{|P+(-Q)|}.\]
\note{交换积分：固定 $x\in P$，使 $x\in Q+t$ 的 $t$ 集合面积为 $|Q|$，总积分即 $|P||Q|$。将 $Q$ 的坐标取负后求闵可夫斯基和，逆时针顺序仍保留。不适用于任意矩形内均匀采样或随机旋转（2025 沈阳 G）。}
\note{平面凸区域面积 $S$、周长 $L$，先加半径 $r$ 的圆盘，再加半径 $R$ 的三维球：}
\begin{align*}
S'&=S+Lr+\pi r^2,\qquad L'=L+2\pi r,\\
V&=2RS'+\frac\pi2 R^2L'+\frac{4\pi}3R^3.
\end{align*}
\note{二维圆盘和三维球的半径不能直接相加。}
''',
}

SPECIAL_NOTES = r'''
\topic{WQS：数量限制变罚项}
令 $F(k)$ 为恰选 $k$ 次的最小代价，
$G(\lambda)=\min_k\{F(k)+\lambda k\}$。
若 $F$ 离散凸，且 $\lambda$ 支持目标 $K$（或 $K$ 位于并列最优数量之间），则
\[
F(K)=G(\lambda)-\lambda K.
\]
罚项增大时最优次数不增；并列时统一取更多次数。
仅次数单调不够：$F(0)=F(2)=0,F(1)=10$ 时只能恢复下凸包。
若单次 DP 为 $O(T)$，二分罚项为 $O(T\log W)$；须证明范围与凸性。
\topic{Burnside / P\'olya：对称下计数}
有限群 $G$ 作用于合法方案集合：
\[
\#\text{轨道}=\frac1{|G|}\sum_{g\in G}\operatorname{Fix}(g).
\]
自由用 $c$ 色染位置时，$\operatorname{Fix}(g)=c^{\#\text{轮换}}$；
有颜色数量限制时须重新数不动方案。$n\ge1$，令
\[
R=\sum_{i=0}^{n-1}c^{\gcd(n,i)}
=\sum_{d\mid n}\varphi(d)c^{n/d}.
\]
只认旋转的项链数为 $R/n$；允许翻转的手链数为 $(R+H)/(2n)$，其中
\[
H=\begin{cases}
nc^{(n+1)/2},&n\text{ 奇},\\
\frac n2\bigl(c^{n/2+1}+c^{n/2}\bigr),&n\text{ 偶}.
\end{cases}
\]
模 $M$ 除以 $|G|$：互素时用逆元；否则可先模 $M|G|$ 计算分子，再整数除以 $|G|$。
例如 $n=6,c=2$：项链 14，手链 13。
'''

def escape(s, breakable=False):
    m = {'\\': r'\textbackslash{}', '&': r'\&', '%': r'\%', '$': r'\$',
         '#': r'\#', '_': r'\_', '{': r'\{', '}': r'\}',
         '~': r'\textasciitilde{}', '^': r'\textasciicircum{}'}
    symbols = {'μ': r'\mu', 'σ': r'\sigma', 'φ': r'\varphi', 'Σ': r'\Sigma',
               'Π': r'\Pi', '∏': r'\prod', '≤': r'\le', '≥': r'\ge',
               '≡': r'\equiv', '⊆': r'\subseteq', '∈': r'\in',
               'π': r'\pi', 'α': r'\alpha', 'Δ': r'\Delta',
               '∞': r'\infty', '²': '^2', '³': '^3', '−': '-',
               '×': r'\times', '≠': r'\ne', '∑': r'\sum', 'τ': r'\tau',
               '₁': '_1', '₂': '_2', '⁺': '^+', '⁻': '^-'}
    out = []
    for c in s:
        out.append(r'\ensuremath{' + symbols[c] + '}' if c in symbols else m.get(c, c))
        if breakable and c in '[](),=+/-<>.':
            out.append(r'\allowbreak{}')
    return ''.join(out)

def inline(s):
    s = re.sub(r'\[([^\]]+)\]\([^)]+\)', r'\1', s)
    # 先分段再转义，避免误转义新插入的 LaTeX 命令。
    parts = re.split(r'(\*\*.*?\*\*|`[^`]*`)', s)
    out = []
    for p in parts:
        if p.startswith('**') and p.endswith('**'):
            out.append(r'\textbf{' + escape(p[2:-2], True) + '}')
        elif p.startswith('`') and p.endswith('`'):
            out.append(r'\texttt{' + escape(p[1:-1], True) + '}')
        else:
            out.append(escape(p, True))
    return ''.join(out)

def clean_code(s, rel, preserve_indent=False):
    drop = re.compile(r'^\s*(?:#include\s*<bits/stdc\+\+\.h>|using namespace std;|'
                      r'typedef long long ll;|using ll\s*=\s*long long;)\s*$')
    if rel == '01-基础与技巧/高维差分.cpp':
        s = s.split('// D 维统一公式：')[0]
    # 以下新增应用说明已单独排为正文，不在代码块重复打印。
    printed_prose = (
        '// 等长区间右移', '// 重叠部分不变', '// 恰覆盖 k 次', '// 前缀和后，允许', '// 注意 d=0',
        '// 最优方案计数陷阱', '// 此时需另存 all', '// 例如历史得分', '// 分支须无重复计数',
        '// 下方正推函数均为截断近似', '// 剩余误差是', '// 若终点可能永不到达',
        '// 随机平移交面积', '// 域为 P+(-Q)', '// 对每个 x属于P', '// 需先把 Q 各点取负',
        '// 子树同深度', '// 在目标深度桶中', '// 桶内顺序作为区间坐标',
        '// 禁止 u 与区间内 x 同选', '// 图上的辅助点不代表布尔变量',
    )
    lines = [l.rstrip() for l in s.splitlines()
             if not drop.match(l) and not l.startswith(printed_prose)]
    # xeCJK 的 listings 适配对部分符号不应用 literate；只处理注释中的数学字符。
    for i, line in enumerate(lines):
        if '//' not in line:
            continue
        code, sep, comment = line.partition('//')
        for char, symbol in {'σ': r'\sigma', '≡': r'\equiv', '⊆': r'\subseteq'}.items():
            comment = comment.replace(char, '(*@$' + symbol + '$@*)')
        lines[i] = code + sep + comment
    # 只收紧空行；函数体、声明、容量和算法注释均不截断。
    body = re.sub(r'\n\s*\n+', '\n', '\n'.join(lines))
    return body.strip('\n') if preserve_indent else body.strip()


def explain(text):
    if text.startswith('例：n 件物品恰选 k 件'):
        return (r'\note{n 件物品恰选 k 件，使总分子与总分母的比最大。'
                r'a 可表示价值，b 表示重量，要求每个 b 为正。'
                r'这是总和之比，不能按各物品比值直接取最大的 k 件。}'
                r'\[\frac{\sum a_i}{\sum b_i}\ge x'
                r'\quad\Longleftrightarrow\quad\sum(a_i-xb_i)\ge0.\]'
                r'\note{猜比值 x 后，取最大的 k 个新权值 a-xb：和非负表示有方案达到 x。'
                r'二分范围取单项比值的最小值与最大值；100 轮排序判定为 $O(100n\log n)$。}')
    if '贡献 a[j]*(j-L)*(R-j)' in text:
        return (r'\note{所有非空子数组最小值之和，$O(n)$。'
                r'对位置 j，L 是左侧首个严格更小位置，R 是右侧首个小于等于的位置。'
                r'两端分别有 j-L 与 R-j 种选择：}'
                r'\[\operatorname{contrib}_j=a_j(j-L)(R-j).\]'
                r'\note{乘积及总和须在 \texttt{ll} 范围内。}')
    if text.startswith('沿用 D:'):
        text = '按已知数量读取合法整数，输入须在 ll 范围内；EOF 返回 0，不能据此区分文件结束与整数零。'
    if text.startswith('无锁版本可按平台'):
        text = '无锁读写接口须按所用平台选取。'
    if text.startswith('DP 示例：f[0]=0'):
        return (r'\topic{有界前驱 DP}'
                r'\note{a 从 1 起，k 至少为 1；先过期、再转移、最后加入 i。'
                r'初始候选 0 不可漏，不能让 i 转移到自己。}'
                r'\[f_0=0,\qquad f_i=a_i+\max_{\max(0,i-k)\le j<i}f_j.\]')
    if text.startswith('[') and text.endswith(']'):
        return r'\topic{' + inline(text[1:-1]) + '}'
    return r'\note{' + inline(text) + '}'


def manual_blocks(source, rel):
    """各章节：模块说明独立排成正文，语句附近的注释保留在代码里。

    原 cpp 仍为复制与测试的入口；这里只重排手册，不修改实现。
    BigInt 的各区在同一个 struct 中，打印时可由正文隔开。
    """
    if rel == '01-基础与技巧/高维差分.cpp':
        source = source.split('// D 维统一公式：')[0]
    if rel == '01-基础与技巧/单调队列.cpp':
        source = source.split('// 等长区间右移：')[0]
    blocks, code, prose = [], [], []
    depth = 0

    def flush_code():
        body = clean_code('\n'.join(code), rel, preserve_indent=True)
        if body:
            blocks.append(('code', body))
        code.clear()

    def flush_prose():
        if prose:
            blocks.append(('text', ' '.join(prose)))
            prose.clear()

    for line in source.splitlines():
        comment = re.match(r'^\s*//\s*(.*)$', line)
        section = (rel.endswith('/高精度BigInt.cpp') and depth == 1
                   and comment and re.match(r'^\[\d+ ', comment[1]))
        if comment and (depth == 0 or section):
            # 第 0 区的标题排在 struct 开始之前，避免只有两行的开括号代码块。
            if not (section and comment[1].startswith('[0 ')):
                flush_code()
            prose.append(comment[1])
            continue
        if not line.strip():
            flush_prose()
            code.append('')
            continue
        flush_prose()
        code.append(line)
        # 忽略字符串、字符常量和行注释中的花括号。
        lexical = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', '', line)
        lexical = lexical.split('//', 1)[0]
        depth += lexical.count('{') - lexical.count('}')
    flush_code()
    flush_prose()
    if depth:
        raise ValueError('Unbalanced code sections in ' + rel)
    return blocks


def render_companion(source, md, rel):
    """说明引用源码模块；每个模块必须恰好收录一次。"""
    modules, current = {}, None
    for line in source.splitlines():
        marker = re.fullmatch(r'// @code ([a-zA-Z0-9_]+)', line)
        if marker:
            current = marker[1]
            if current in modules:
                raise ValueError('Duplicate source module in ' + rel + ': ' + current)
            modules[current] = []
        elif current is None:
            if line.strip():
                raise ValueError('Code before first module in ' + rel)
        else:
            modules[current].append(line)
    out, prose, used = [], [], set()
    def flush():
        if prose:
            out.append(math_notes('\n'.join(prose)))
            prose.clear()
    for line in md.splitlines():
        marker = re.fullmatch(r'<!-- code: ([a-zA-Z0-9_]+) -->', line)
        if marker:
            flush()
            name = marker[1]
            if name not in modules or name in used:
                raise ValueError('Unknown or repeated module in ' + rel + ': ' + name)
            used.add(name)
            body = clean_code('\n'.join(modules[name]), rel, preserve_indent=True)
            if r'\end{lstlisting}' in body:
                raise ValueError('Listing delimiter in ' + rel)
            out.extend([r'\begin{lstlisting}', body, r'\end{lstlisting}'])
        elif line.strip().startswith('<!-- code:'):
            raise ValueError('Malformed module reference in ' + rel)
        elif re.fullmatch(r'(?:\[[^]]+\]\([^)]+\)(?:\s*·\s*)?)+', line.strip()):
            continue  # 电子版导航链接，不进入打印正文。
        else:
            prose.append(line)
    flush()
    if used != modules.keys():
        raise ValueError('Unprinted modules in ' + rel + ': ' + ','.join(modules.keys()-used))
    return '\n'.join(out)


@lru_cache(maxsize=1)
def variable_notes():
    variable_file = ROOT / '说明' / '全局变量说明.md'
    result = {}
    for section in variable_file.read_text(encoding='utf-8-sig').split('\n## ')[1:]:
        title, _, body = section.partition('\n')
        title = title.strip()
        if title in result or not body.strip():
            raise ValueError('Duplicate or empty variable note: ' + title)
        result[title] = math_notes(body.strip()) + '\n'
    actual = {p.relative_to(ROOT).as_posix() for d, _ in CHAPTERS
              for p in (ROOT / d).glob('*.cpp')}
    if result.keys() != actual:
        raise ValueError('Variable notes mismatch: ' + ', '.join(sorted(result.keys() ^ actual)))
    return result


def render_template(source, rel):
    variable_text = variable_notes()[rel]
    companion = ROOT / '说明' / Path(rel).with_suffix('.md')
    if companion.exists():
        return variable_text + render_companion(source, companion.read_text(encoding='utf-8-sig'), rel)
    blocks = manual_blocks(source, rel)
    out = []
    for kind, body in blocks:
        if kind == 'text':
            out.append(explain(body))
        else:
            if r'\end{lstlisting}' in body:
                raise ValueError('Listing delimiter in ' + rel)
            out.extend([r'\begin{lstlisting}', body, r'\end{lstlisting}'])
    return variable_text + '\n'.join(out)

def notes(md, common=False):
    out = []
    if common:
        a, sep, b = md.partition('## WQS 与 Burnside / Pólya')
        md = a
        tail = b.partition('## 扩展欧拉、LGV 与建模补充')[2] if sep else ''
    else:
        tail = ''
    started = False
    for l in md.splitlines():
        if l.startswith('## '):
            started = True
            out.append(r'\topic{' + escape(l[3:]) + '}')
        elif l.startswith('### '):
            out.append(r'\par\textbf{' + escape(re.sub(r'^\d+\.\s*', '', l[4:])) + r'}\quad')
        elif started and l.startswith('- '):
            if '**你的题**' in l or '**模板**' in l:
                continue
            out.append(inline(l[2:].removeprefix('**结论**：')) + r'\par')
    if common:
        out.append(SPECIAL_NOTES)
        out.append(r'\topic{扩展欧拉、LGV 与建模补充}')
        for l in tail.splitlines():
            if l.startswith('- '):
                out.append(inline(l[2:]) + r'\par\smallskip')
    return '\n'.join(out)

def reference(md):
    """两列表格按小节排版；保留短写法，不转成示例函数。"""
    out = []
    table = False
    for line in md.splitlines():
        if table and not line.startswith('|'):
            out.append(r'\hline\end{tabularx}\par\endgroup')
            table = False
        if line.startswith('# ') or not line.strip():
            continue
        if line.startswith('## '):
            out.append(r'\topic{' + escape(line[3:]) + '}')
        elif line.startswith('|'):
            cells = [s.strip() for s in line.strip('|').split('|')]
            if all(re.fullmatch(r':?-+:?', s) for s in cells):
                continue
            if not table:
                out.extend([r'\begingroup\fontsize{7.8}{9}\selectfont',
                            r'\renewcommand{\arraystretch}{1.05}',
                            r'\begin{tabularx}{\linewidth}{@{}>{\raggedright\arraybackslash}p{0.45\linewidth}'
                            r'@{\hspace{4pt}}>{\raggedright\arraybackslash}X@{}}\hline',
                            r'\textbf{' + escape(cells[0]) + '} & '
                            r'\textbf{' + escape(cells[1]) + r'}\\\hline'])
                table = True
            else:
                out.append(inline(cells[0]) + ' & ' + inline(cells[1]) + r'\\[2pt]')
        else:
            out.append(r'\note{' + inline(line) + '}')
    if table:
        out.append(r'\hline\end{tabularx}\par\endgroup')
    return '\n'.join(out)


def math_notes(md):
    """紧凑技巧正文：Markdown 标题、段落及受控的 $...$ 数学公式。"""
    out = []
    for line in md.splitlines():
        if not line.strip() or line.startswith('# '):
            continue
        if line.startswith('## '):
            out.append(r'\topic{' + escape(line[3:]) + '}')
        elif line.startswith('### '):
            out.append(r'\par\textbf{' + escape(line[4:]) + r'}\quad')
        else:
            if line.startswith('- '):
                line = line[2:]
            parts = re.split(r'(\$[^$]+\$)', line)
            if sum(p.count('$') for p in parts) % 2:
                raise ValueError('Unclosed math formula: ' + line)
            out.append(r'\note{' + ''.join(p if p.startswith('$') and p.endswith('$')
                                          else inline(p) for p in parts) + '}')
    return '\n'.join(out)


def ordered_files(cfg):
    """各章按常用程度与关联性编排；未列出的新模板稳定追加到章末。"""
    order = cfg.get('chapter_order', {})
    if not isinstance(order, dict):
        raise ValueError('chapter_order must be an object')
    unknown = set(order) - {d for d, _ in CHAPTERS}
    if unknown:
        raise ValueError('Unknown ordered chapters: ' + ','.join(sorted(unknown)))
    result = []
    for directory, _ in CHAPTERS:
        files = sorted(p for p in (ROOT / directory).iterdir()
                       if p.suffix in ('.cpp', '.md'))
        names = order.get(directory, [])
        if not isinstance(names, list) or any(not isinstance(x, str) for x in names):
            raise ValueError('chapter_order entries must be filename lists: ' + directory)
        if len(names) != len(set(names)):
            raise ValueError('Duplicate ordered files: ' + directory)
        available = {p.name: p for p in files}
        missing = set(names) - set(available)
        if missing:
            raise ValueError('Unknown ordered files in ' + directory + ': ' + ','.join(sorted(missing)))
        result.extend(available[name] for name in names)
        result.extend(p for p in files if p.name not in set(names))
    return result


def main():
    cfg = json.loads(CFG.read_text(encoding='utf-8-sig'))
    excluded = set(cfg.get('exclude', []))
    all_files = ordered_files(cfg)
    all_rel = {p.relative_to(ROOT).as_posix() for p in all_files}
    unknown = excluded - all_rel
    if unknown:
        raise ValueError('Unknown excluded files: ' + ','.join(sorted(unknown)))
    total = len(all_files) - len(excluded)
    code_total = sum(p.suffix == '.cpp' and p.relative_to(ROOT).as_posix() not in excluded
                     for p in all_files)
    ref_total = total - code_total
    pre = PREAMBLE.replace('@CODE@', str(cfg['code_font_pt'])).replace('@LEAD@', str(cfg['code_leading_pt']))
    tex = [pre, r'\begin{center}{\fontsize{15}{17}\selectfont\bfseries ICPC 算法手册}\quad'
           r'\small 2026-10-05\end{center}',
           r'\note{双栏完整实现版\quad ' + str(code_total) + ' 份代码模板、' + str(ref_total) +
           r' 份速查表。各模板独立使用；同名全局量和函数按题目取舍，不将整本直接拼接编译。'
           r'公共头文件与 \texttt{ll} 定义仅在此列出，其他容量、类型和依赖保留在各模板中。}',
           r'\begin{lstlisting}', '#include<bits/stdc++.h>\nusing namespace std;\ntypedef long long ll;',
           r'\end{lstlisting}', r'\begin{multicols}{2}\tableofcontents\end{multicols}',
           r'\clearpage\begin{multicols}{2}']
    emitted = []
    for directory, title in CHAPTERS:
        chosen = [p for p in all_files if p.parent.name == directory
                  and p.relative_to(ROOT).as_posix() not in excluded]
        if not chosen:
            continue
        tex.append(r'\section{' + title + '}')
        for p in chosen:
            rel = p.relative_to(ROOT).as_posix()
            tex.append('% SOURCE: ' + rel)
            tex.append(r'\subsection{' + escape(p.stem) + '}')
            if p.suffix == '.md':
                tex.append(reference(p.read_text(encoding='utf-8-sig')))
                emitted.append(rel)
                continue
            tex.append(FORMULAS.get(rel, ''))
            tex.append(render_template(p.read_text(encoding='utf-8-sig'), rel))
            emitted.append(rel)
    if cfg.get('include_notes', True):
        tex.append(r'\section{模型判据与常用结论}')
        tex.append(notes((ROOT / '结论速查.md').read_text(encoding='utf-8-sig')))
        tex.append(notes((ROOT / '结论速查' / 'ICPC常用结论.md').read_text(encoding='utf-8-sig'), True))
        tex.append(r'\section{赛事建模与技巧}')
        tex.append(math_notes((ROOT / '结论速查' / '赛事建模与技巧.md').read_text(encoding='utf-8-sig')))
    tex.extend([r'\end{multicols}', r'\end{document}'])
    OUT.write_text('\n'.join(tex) + '\n', encoding='utf-8')
    print(f'Generated {OUT.name.encode("ascii", "backslashreplace").decode()}: '
          f'{code_total} complete templates, {ref_total} reference tables')
    print(f'{OUT.stat().st_size} bytes; omitted {len(excluded)}; no external inputs')
    assert len(emitted) == total

if __name__ == '__main__':
    main()
